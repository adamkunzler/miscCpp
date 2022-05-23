//#include <iostream>
//#include <fstream>
//#include <string>
//#include <chrono>
//#include <thread>
//
//#include "rtweekend.h"
//
//#include "camera.h"
//#include "color.h"
//#include "hittableList.h"
//#include "sphere.h"
//#include "material.h"
//
//template <
//	class result_t = std::chrono::milliseconds,
//	class clock_t = std::chrono::steady_clock,
//	class duration_t = std::chrono::milliseconds>
//	auto since(std::chrono::time_point<clock_t, duration_t> const& start)
//{
//	return std::chrono::duration_cast<result_t>(clock_t::now() - start);
//}
//
//color rayColor(const ray& r, const Hittable& world, int depth)
//{
//	HitRecord rec;
//
//	// If we've exceeded the ray bounce limit, no more light is gathered.
//	if (depth <= 0)
//		return color(0, 0, 0);
//
//	if (world.hit(r, 0.001, infinity, rec))
//	{
//		//return 0.5 * (rec.normal + color(1, 1, 1));
//
//		//point3 target = rec.p + random_in_hemisphere(rec.normal);
//		//return 0.5 * rayColor(ray(rec.p, target - rec.p), world, depth - 1);
//
//		ray scattered;
//		color attenuation;
//		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
//			return attenuation * rayColor(scattered, world, depth - 1);
//		return color(0, 0, 0);
//	}
//
//	// BACKGROUND
//	vec3 unitDirection = unitVector(r.direction());
//
//	// lerp => blendedValue=(1−t)⋅startValue+t⋅endValue
//	auto t = 0.5 * (unitDirection.y() + 1.0);
//	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
//}
//
//HittableList random_scene() {
//	HittableList world;
//
//	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
//	world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));
//
//	//for (int a = -11; a < 11; a++) {
//	//	for (int b = -11; b < 11; b++) {
//	//		auto choose_mat = random_double();
//	//		point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
//
//	//		if ((center - point3(4, 0.2, 0)).length() > 0.9) {
//	//			shared_ptr<material> sphere_material;
//
//	//			if (choose_mat < 0.8) {
//	//				// diffuse
//	//				auto albedo = color::random() * color::random();
//	//				sphere_material = make_shared<lambertian>(albedo);
//	//				world.add(make_shared<Sphere>(center, 0.2, sphere_material));
//	//			}
//	//			else if (choose_mat < 0.95) {
//	//				// metal
//	//				auto albedo = color::random(0.5, 1);
//	//				auto fuzz = random_double(0, 0.5);
//	//				sphere_material = make_shared<metal>(albedo, fuzz);
//	//				world.add(make_shared<Sphere>(center, 0.2, sphere_material));
//	//			}
//	//			else {
//	//				// glass
//	//				sphere_material = make_shared<dielectric>(1.5);
//	//				world.add(make_shared<Sphere>(center, 0.2, sphere_material));
//	//			}
//	//		}
//	//	}
//	//}
//
//	auto material1 = make_shared<dielectric>(1.5);
//	world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));
//
//	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
//	world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));
//
//	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
//	world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));
//
//	return world;
//}
//
//void renderThread(color* imageData, const camera& cam, const HittableList& world, int samplesPerPixel, int maxDepth, int imageWidth, int imageHeight, int startY, int endY)
//{
//	//std::cout << "Thread: " << "\twidth: " << width << "\tstartY: " << startY << "\tendY:" << endY << std::endl;
//	int height = endY - startY;
//	for (int y = 0; y < height; y++)
//	{
//		for (int x = 0; x < imageWidth; x++)
//		{
//			int yy = startY + y;
//
//			auto u = double(x) / (imageWidth - 1);
//			auto v = double(yy) / (imageHeight - 1);
//
//			color pixel_color(0, 0, 0);
//			for (int s = 0; s < samplesPerPixel; ++s) {
//				auto u = (x + random_double()) / (imageWidth - 1);
//				auto v = (yy + random_double()) / (imageHeight - 1);
//				ray r = cam.get_ray(u, v);
//				pixel_color += rayColor(r, world, maxDepth);
//			}
//
//			imageData[yy * imageWidth + x] = pixel_color;
//		}
//	}
//}
//
//int main()
//{
//	auto start = std::chrono::steady_clock::now();
//
//	// IMAGE
//	const auto aspect_ratio = 3.0 / 2.0;
//	const int image_width = 400; //1200
//	const int image_height = 260;// static_cast<int>(image_width / aspect_ratio);
//	const int samples_per_pixel = 100; //500
//	const int max_depth = 10; //50
//
//	// WORLD
//	auto world = random_scene();
//
//	// CAMERA
//	point3 lookfrom(13, 2, 3);
//	point3 lookat(0, 0, 0);
//	vec3 vup(0, 1, 0);
//	auto dist_to_focus = 10.0;
//	auto aperture = 0.1;
//
//	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
//
//
//	// RENDER	
//	color* image = new color[image_width * image_height];
//
//
//	size_t numThreads = 16;
//	std::vector<std::thread> threads;
//	int yStart = 0;
//	int yStep = image_height / numThreads;
//	for (int i = 0; i < numThreads; i++)
//	{
//		int yEnd = yStart + yStep > image_height
//			? image_height
//			: yStart + yStep;
//
//		std::thread th(renderThread, image, cam, world, samples_per_pixel, max_depth, image_width, image_height, yStart, yEnd);
//		threads.push_back(std::move(th));
//		//std::cout << "Thread: " << "\twidth: " << image_width << "\tstartY: " << yStart << "\tendY:" << yEnd << std::endl;
//
//		yStart += yStep;
//	}
//
//	// wait for threads to finish working			
//	for (std::thread& th : threads)
//		if (th.joinable())
//			th.join();
//
//	// WRITE TO PPM FILE
//	std::string filename = "image_" + std::to_string(image_width) + "x" + std::to_string(image_height) + "_" + std::to_string(samples_per_pixel) + "spp_" + std::to_string(max_depth) + "d.ppm";
//	std::ofstream imageFile{ filename, std::ios::out | std::ios::trunc };
//	imageFile << "P3\n" << image_width << " " << image_height << "\n255\n";
//
//	for (int j = image_height - 1; j >= 0; --j)
//		for (size_t i = 0; i < image_width; ++i)
//			writeColor(imageFile, image[j * image_width + i], samples_per_pixel);
//
//	imageFile.close();
//	delete[] image;
//
//	std::cerr << "\n\nElapsed(ms)=" << since(start).count() << std::endl;
//	std::cerr << "\nDone.\n";
//	return EXIT_SUCCESS;
//}