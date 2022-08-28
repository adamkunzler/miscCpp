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

void Sprite::render(const Vector2& position) const
{
	
	Rectangle src = _config.frames.at(_currentFrame);
	Rectangle dest{ position.x, position.y, src.width, src.height };
	Vector2 origin{ src.width / 2.f, src.height / 2.f };
	
	DrawTexturePro(*_config.texture, src, dest, origin, 0.f, WHITE);

	if (true)
	{
		// sprite bounds
		DrawRectangleLines(dest.x - origin.x, dest.y - origin.y, dest.width, dest.height, SKYBLUE);

		// sprite position (center)
		DrawCircle(position.x, position.y, 5.f, PURPLE);

		// sprite bounding box
		DrawRectangleLines(
			_config.boundingBoxes.at(_currentFrame).x + dest.x - origin.x,
			_config.boundingBoxes.at(_currentFrame).y + dest.y - origin.y,
			_config.boundingBoxes.at(_currentFrame).width,
			_config.boundingBoxes.at(_currentFrame).height,
			GREEN);
	}
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


