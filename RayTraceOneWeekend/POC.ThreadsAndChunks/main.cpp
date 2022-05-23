#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>

#include "Chunk.h"
#include "ThreadPool.h"

using namespace std::chrono_literals;

void renderThread(std::shared_ptr<Chunk>);
std::vector<std::shared_ptr<Chunk>> buildChunks(int chunkSize, int imageWidth, int imageHeight);
void printChunks(std::vector<std::shared_ptr<Chunk>>);
int* combineChunks(std::vector<std::shared_ptr<Chunk>> chunks, int imageWidth, int imageHeight);
void draw(std::vector<std::shared_ptr<Chunk>> chunks, int imageWidth, int imageHeight);

int main()
{
	std::cout << "\n.: Proof of Concept - Threads and Chunks :." << std::endl;

	const int numThreads{ 4 };
	const int imageWidth{ 40 };
	const int imageHeight{ 20 };
	const int chunkSize{ 2 };

	std::cout << "\nimageWidth: " << imageWidth << "\timageHeight: " << imageHeight;

	//
	// build and process the chunks
	//
	std::vector<std::shared_ptr<Chunk>> chunks = buildChunks(chunkSize, imageWidth, imageHeight);	
	std::random_shuffle(chunks.begin(), chunks.end());

	//
	// assign chunks to threads
	//
	std::cout << "\nNum Chunks: " << chunks.size();
	{
		ThreadPool<Chunk> threadPool{ numThreads };
		for (auto& c : chunks)
		{
			Task<Chunk> t{ renderThread, c };
			threadPool.enqueue(t);
		}
		while (threadPool.hasJobs()) 
		{
			draw(chunks, imageWidth, imageHeight);
		}
	} // kill scope so threadPool destructor gets called
	//std::cout << "\n\nThreadPool is done.";

	//
	// print the chunks for kicks
	//
	//printChunks(chunks);

	// image data allocation and set initial value
	draw(chunks, imageWidth, imageHeight);
	//int* imageData = combineChunks(chunks, imageWidth, imageHeight);

	//std::cout << std::endl;
	//std::cout << std::endl;

	////
	//// print image data	
	////
	//if (true)
	//{
	//	for (int y = 0; y < imageHeight; ++y)
	//	{
	//		std::cout << std::endl;
	//		for (int x = 0; x < imageWidth; ++x)
	//		{
	//			auto index = x + y * imageWidth;
	//			std::cout << imageData[index] << " ";
	//			//std::cout << index << " ";
	//		}
	//	}
	//}

	//delete[] imageData;

	std::cout << std::endl;

	return EXIT_SUCCESS;
}


void draw(std::vector<std::shared_ptr<Chunk>> chunks, int imageWidth, int imageHeight)
{	
	int* imageData = combineChunks(chunks, imageWidth, imageHeight);

	std::cout << std::endl;
	std::cout << std::endl;

	//
	// print image data	
	//
	if (true)
	{
		for (int y = 0; y < imageHeight; ++y)
		{
			std::cout << std::endl;
			for (int x = 0; x < imageWidth; ++x)
			{
				auto index = x + y * imageWidth;
				std::cout << std::setw(3) << imageData[index] << " ";
				//std::cout << index << " ";
			}
		}
	}

	delete[] imageData;
}

void renderThread(std::shared_ptr<Chunk> chunk)
{
	std::this_thread::sleep_for(100ms);
	for (int y = 0; y < chunk->chunkHeight; ++y)
	{
		for (int x = 0; x < chunk->chunkWidth; ++x)
		{
			//int xx = chunk.xStart + x;
			//int yy = chunk.yStart + y;

			auto index = x + y * chunk->chunkWidth;
			chunk->data[index] = chunk->chunkId;
		}
	}
}

std::vector<std::shared_ptr<Chunk>> buildChunks(int chunkSize, int imageWidth, int imageHeight)
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

			auto chunk = std::make_shared<Chunk>(id, xStart, xEnd, yStart, yEnd);
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

int* combineChunks(std::vector<std::shared_ptr<Chunk>> chunks, int imageWidth, int imageHeight)
{
	int* imageData = new int[imageWidth * imageHeight];
	for (int i = 0; i < imageWidth * imageHeight; ++i)
		imageData[i] = 0;

	//
	// combine chunks
	//
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