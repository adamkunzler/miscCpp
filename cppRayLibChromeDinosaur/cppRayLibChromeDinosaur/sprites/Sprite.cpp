#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::Sprite(const SpriteConfig& config)
	: _config{ config }
{
	_numberOfFrames = (int)config.frames.size();
}

bool Sprite::isType(SpriteType type)
{
	return type == _config.type;
}

const Vector2& Sprite::dimension() const 
{ 
	return _config.frameDimensions; 
}

void Sprite::update()
{
	if (!isAnimated) return;

	_frameCounter++;
	if (_frameCounter >= (60 / frameSpeed)) // 60 fps / sprite fps
	{
		// enough time has passed for current sprite frame:
		//	- reset counter 			
		_frameCounter = 0;

		//	- increment current frame			
		_currentFrame++;
		if (_currentFrame >= _numberOfFrames) _currentFrame = 0;
	}
}

void Sprite::render(const Vector2& position, const float maxWidth, const bool flipHoriz) const
{
	float ratio = _config.frameDimensions.x / maxWidth;
	float height = _config.frameDimensions.y / ratio;

	Rectangle src = _config.frames.at(_currentFrame);
	Rectangle dest{ position.x, position.y, maxWidth, height };
	Vector2 origin{ maxWidth / 2.f, height / 2.f };

	if (flipHoriz) src.width = -src.width;

	DrawTexturePro(*_config.texture, src, dest, origin, 0.f, WHITE);	
}

int Sprite::getFrameCount() const
{
	return (int)_config.frames.size();
}

int Sprite::getCurrentFrame() const
{
	return _currentFrame;
}

void Sprite::setCurrentFrame(int frame)
{
	if (frame >= _config.frames.size()) return;

	_currentFrame = frame;
}


