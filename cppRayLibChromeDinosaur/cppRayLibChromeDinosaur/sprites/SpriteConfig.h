#ifndef SPRITECONFIG_H
#define SPRITECONFIG_H

#include <vector>
#include <memory>

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
	std::shared_ptr<Texture2D> texture = nullptr;
	std::vector<Rectangle> frames;
	Vector2 frameDimensions{ 0, 0 };
	SpriteType type { SpriteType::UNKNOWN };

	SpriteConfig() = default;

	SpriteConfig(SpriteType type, const std::shared_ptr<Texture2D>& texture, Vector2 frameDimensions);

private:

	void generateFrames();
};

#endif