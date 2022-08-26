#include "SpriteConfig.h"

SpriteConfig::SpriteConfig(SpriteType type, const std::shared_ptr<Texture2D>& texture, Vector2 frameDimensions)
	: type{ type }, texture{ texture }, frameDimensions{ frameDimensions }
{
	generateFrames();
}

void SpriteConfig::generateFrames()
{
	for (int x = 0; x < texture->width; x += (int)frameDimensions.x)
		frames.push_back(Rectangle{ (float)x, 0, frameDimensions.x, frameDimensions.y });
}