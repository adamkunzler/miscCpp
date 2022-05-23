#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittableList.h"
#include "sphere.h"
#include "material.h"

#include "Chunk.h"
#include "ThreadPool.h"

using namespace std::chrono_literals;

template <
	class result_t = std::chrono::milliseconds,
	class clock_t = std::chrono::steady_clock,
	class duration_t = std::chrono::milliseconds>
	auto since(std::chrono::time_point<clock_t, duration_t> const& start)
{
	return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}

color rayColor(const ray& r, const Hittable& world, int depth);
HittableList random_scene();
//void renderThread(color* imageData, const camera& cam, const HittableList& world, int samplesPerPixel, int maxDepth, int imageWidth, int imageHeight, int startY, int endY);
void renderThread(std::shared_ptr<Chunk>);
std::vector<std::shared_ptr<Chunk>> buildChunks(int chunkSize, const camera& cam, const HittableList& world, int samplesPerPixel, int maxDepth, int imageWidth, int imageHeight);
void printChunks(std::vector<std::shared_ptr<Chunk>>);
color* combineChunks(std::vector<std::shared_ptr<Chunk>> chunks, int imageWidth, int imageHeight);


int main()
{
	auto start = std::chrono::steady_clock::now();

	// IMAGE
	const int numThreads{ 16 }; //8=77009
	const int chunkSize{ 100 };

	const auto aspect_ratio = 1.5;
	const int samples_per_pixel = 250; //500
	const int max_depth = 50; //50

	const int image_width = 1200; //1200
	const int image_height = 800;// static_cast<int>(image_width / aspect_ratio);

	//
	// WORLD
	//
	auto world = random_scene();

	//
	// CAMERA
	//
	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;

	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

	//
	// CHUNKS
	//
	std::cout << "\n Building chunks...";
	std::vector<std::shared_ptr<Chunk>> chunks =
		buildChunks(chunkSize, cam, world, samples_per_pixel, max_depth, image_width, image_height);
	std::cout << "\n " << chunks.size() << " chunks created.";
	//printChunks(chunks);

	// assign chunks to threads		
	std::cout << "\n Assigning chunks to threads...";
	{
		ThreadPool<Chunk> threadPool{ numThreads };
		for (auto& c : chunks)
		{
			Task<Chunk> t{ renderThread, c };
			threadPool.enqueue(t);
		}		
		std::cout << "\n Waiting for ThreadPool to finish..." << std::endl;
		while (threadPool.hasJobs()) 
		{
			std::this_thread::sleep_for(1s);
			std::cout << "\r Chunks left: " << threadPool.numJobsInQueue() << "                 " << std::flush;
		}
	} // kill scope so threadPool destructor gets called

	std::cout << "\n ThreadPool finished.";

	//
	// RENDER TO PPM
	// 
	std::cout << "\n Combining chunks...";
	color* imageData = combineChunks(chunks, image_width, image_height);
	
	//
	// WRITE TO PPM FILE
	//
	std::cout << "\n Writing data to PPM...";
	std::string filename = "image_" + std::to_string(image_width) + "x" + std::to_string(image_height) + "_" + std::to_string(samples_per_pixel) + "spp_" + std::to_string(max_depth) + "d.ppm";
	std::ofstream imageFile{ filename, std::ios::out | std::ios::trunc };
	imageFile << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j)
		for (size_t i = 0; i < image_width; ++i)
			writeColor(imageFile, imageData[j * image_width + i], samples_per_pixel);

	imageFile.close();

	//
	// CLEANUP
	//
	delete[] imageData;

	std::cerr << "\n\nElapsed(ms)=" << since(start).count() << std::endl;
	std::cerr << "\nDone.\n";
	return EXIT_SUCCESS;
}

color rayColor(const ray& r, const Hittable& world, int depth)
{
	HitRecord rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	if (world.hit(r, 0.001, infinity, rec))
	{
		//return 0.5 * (rec.normal + color(1, 1, 1));

		//point3 target = rec.p + random_in_hemisphere(rec.normal);
		//return 0.5 * rayColor(ray(rec.p, target - rec.p), world, depth - 1);

		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * rayColor(scattered, world, depth - 1);
		return color(0, 0, 0);
	}

	// BACKGROUND
	vec3 unitDirection = unitVector(r.direction());

	// lerp => blendedValue=(1−t)⋅startValue+t⋅endValue
	auto t = 0.5 * (unitDirection.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

HittableList random_scene() {
	HittableList world;

	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

	return world;
}

void renderThread(std::shared_ptr<Chunk> chunk)
{
	for (int y = 0; y < chunk->chunkHeight; ++y)
	{
		for (int x = 0; x < chunk->chunkWidth; ++x)
		{
			int xx = chunk->xStart + x;
			int yy = chunk->yStart + y;

			auto u = double(xx) / (chunk->imageWidth - 1);
			auto v = double(yy) / (chunk->imageHeight - 1);

			color pixel_color(0, 0, 0);
			for (int s = 0; s < chunk->samplesPerPixel; ++s) {
				auto u = (xx + random_double()) / (chunk->imageWidth - 1);
				auto v = (yy + random_double()) / (chunk->imageHeight - 1);
				ray r = chunk->cam.get_ray(u, v);
				pixel_color += rayColor(r, chunk->world, chunk->maxDepth);
			}

			auto index = x + y * chunk->chunkWidth;
			chunk->data[index] = pixel_color;
		}
	}
}

std::vector<std::shared_ptr<Chunk>> buildChunks(int chunkSize, const camera& cam, const HittableList& world, int samplesPerPixel, int maxDepth, int imageWidth, int imageHeight)
{
	std::vector<std::shared_ptr<Chunk>> chunks;

	int id = 1;

	int xStart = 0;
	int xStep = chunkSize;

	int yStart = 0;
	int yStep = chunkSize;

	for (int i = 0; i < imageHeight / chunkSize; i++)
	{
		xStart = 0;
		int yEnd = yStart + yStep > imageHeight
			? imageHeight
			: yStart + yStep;

		for (int j = 0; j < imageWidth / chunkSize; j++)
		{
			int xEnd = xStart + xStep > imageWidth
				? imageWidth
				: xStart + xStep;

			auto chunk = std::make_shared<Chunk>(id, xStart, xEnd, yStart, yEnd, cam, world, samplesPerPixel, maxDepth, imageWidth, imageHeight);
			chunks.push_back(chunk);

			id++;
			xStart += xStep;
		}
		yStart += yStep;
	}

	return chunks;
}

void printChunks(std::vector<std::shared_ptr<Chunk>> chunks)
{
	for (const auto& c : chunks)
	{
		std::cout << "\n\nChunk #" << c->chunkId << "\n\txStart: " << c->xStart << "\txEnd: " << c->xEnd << "\n\tyStart: " << c->yStart << "\tyEnd: " << c->yEnd << std::endl;

		for (int y = 0; y < c->chunkHeight; ++y)
		{
			std::cout << std::endl << "\t";
			for (int x = 0; x < c->chunkWidth; ++x)
			{
				std::cout << c->data[x + y * c->chunkWidth] << " ";
			}
		}

	}
}

color* combineChunks(std::vector<std::shared_ptr<Chunk>> chunks, int imageWidth, int imageHeight)
{
	color* imageData = new color[imageWidth * imageHeight];
	for (int i = 0; i < imageWidth * imageHeight; ++i)
		imageData[i] = color{ 0, 0, 0 };

	for (const auto& c : chunks)
	{
		for (int y = 0; y < c->chunkHeight; ++y)
		{
			for (int x = 0; x < c->chunkWidth; ++x)
			{
				auto ix = c->xStart + x;
				auto iy = c->yStart + y;
				imageData[ix + iy * imageWidth] = c->data[x + y * c->chunkHeight];
			}
		}

	}

	return imageData;
}