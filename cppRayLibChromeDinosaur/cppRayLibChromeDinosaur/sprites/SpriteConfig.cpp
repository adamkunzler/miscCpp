#include "SpriteConfig.h"

SpriteConfig::SpriteConfig(SpriteType type, std::string filename, Vector2 size, int numFrames)
	: type{ type }, filename{ filename }, size{ size }
{
	generate(numFrames);
}

void SpriteConfig::generate(const int numFrames)
{
	// load and resize sprite sheet
	auto image = LoadImage(filename.c_str());
	ImageResize(&image, size.x * numFrames, size.y);
	texture = std::make_shared<Texture2D>(LoadTextureFromImage(image));

	// calculate frame rects and bounding boxes
	for (int frame = 0; frame < numFrames; frame++)
	{
		// calculate rectangle of frame
		Rectangle frameRect{ frame * size.x, 0, size.x, size.y };
		
		// calculate bounding box
		auto copy = ImageFromImage(image, frameRect);
		auto frameBoundingBoxRect = GetImageAlphaBorder(copy, 0.f);

		frames.push_back(frameRect);
		boundingBoxes.push_back(frameBoundingBoxRect);
	}
		
}