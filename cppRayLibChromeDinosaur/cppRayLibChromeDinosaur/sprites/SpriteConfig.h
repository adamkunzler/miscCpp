#ifndef SPRITECONFIG_H
#define SPRITECONFIG_H

#include <vector>
#include <memory>
#include <string>

#include "raylib.h"

enum class SpriteType
{
	UNKNOWN,
	DINO_IDLE,
	DINO_JUMP,
	DINO_WALK,
	DINO_RUN,
	DINO_DEAD
};

class SpriteConfig
{
public:
	std::string filename;
	
	std::shared_ptr<Texture2D> texture = nullptr;
	SpriteType type{ SpriteType::UNKNOWN };

	std::vector<Rectangle> frames;			// rectangle of frame in sprite texture
	Vector2 size{ 0, 0 };					// width and height of a frame
	std::vector<Rectangle> boundingBoxes;	// bounding box for each frame
	

	SpriteConfig() = default;

	SpriteConfig(SpriteType type, std::string filename, Vector2 size, int numFrames);

private:

	void generate(const int numFrames);
};

#endif