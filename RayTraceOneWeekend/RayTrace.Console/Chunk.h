#ifndef CHUNK_H
#define CHUNK_H

class Chunk
{
public:
	Chunk(
		int chunkId,
		int xStart,
		int xEnd,
		int yStart,
		int yEnd,

		const camera& cam,
		const HittableList& world,
		int samplesPerPixel,
		int maxDepth,
		int imageWidth,
		int imageHeight
	)
		: chunkId{ chunkId },
		xStart{ xStart },
		xEnd{ xEnd },
		yStart{ yStart },
		yEnd{ yEnd },
		cam{ cam },
		world{ world },
		samplesPerPixel{ samplesPerPixel },
		maxDepth{ maxDepth },
		imageWidth{ imageWidth },
		imageHeight{ imageHeight }
	{
		chunkHeight = yEnd - yStart;
		chunkWidth = xEnd - xStart;

		data = new color[chunkHeight * chunkWidth];
		for (int i = 0; i < chunkHeight * chunkWidth; ++i)
			data[i] = color{ 0,0,0 };
	}

	~Chunk()
	{
		//std::cout << "\nDelete CHUNK #" << chunkId << std::endl;
		delete[] data;
	}

public:
	int chunkId;

	int chunkWidth;
	int chunkHeight;

	int xStart;
	int xEnd;

	int yStart;
	int yEnd;

	color* data;


	const camera& cam;
	const HittableList& world;
	int samplesPerPixel;
	int maxDepth;
	int imageWidth;
	int imageHeight;
};

#endif CHUNK_H