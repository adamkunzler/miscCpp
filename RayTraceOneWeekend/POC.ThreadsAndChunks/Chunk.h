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
		int yEnd
	)
		: chunkId{ chunkId },
		xStart{ xStart },
		xEnd{ xEnd },
		yStart{ yStart },
		yEnd{ yEnd }
	{
		chunkHeight = yEnd - yStart;
		chunkWidth = xEnd - xStart;

		data = new int[chunkHeight * chunkWidth];
		for (int i = 0; i < chunkHeight * chunkWidth; ++i)
			data[i] = 0;
	}

	int* getData()
	{
		return data;
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

	int* data;
};

#endif CHUNK_H