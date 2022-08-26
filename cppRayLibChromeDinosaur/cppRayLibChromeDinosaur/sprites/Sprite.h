#ifndef SPRITE_H
#define SPRITE_H

#include "raylib.h"

#include "SpriteConfig.h"

//
// Represents a series of images from a single texture laid out horizontally with the same dimensions
//
class Sprite
{
private:
	SpriteConfig _config;
	
	int _currentFrame{ 0 };
	int _numberOfFrames{ 0 };
	int _frameCounter{ 0 }; // time elapsed for current frame
	
public:	
	int frameSpeed{ 12 }; // e.g. 3 frames per second
	bool isAnimated{ true };

public:

	Sprite();

	Sprite(const SpriteConfig& config);
	
	bool isType(SpriteType type);

	const Vector2& dimension() const;

	void update();

	void render(const Vector2& position, const float maxWidth, const bool flipHoriz = false) const;

	int getFrameCount() const;

	int getCurrentFrame() const;
	void setCurrentFrame(int frame);
};


#endif