#ifndef SPRITEFACTORY_H
#define SPRITEFACTORY_H

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "raylib.h"

#include "SpriteConfig.h"
#include "Sprite.h"

class Sprite;

std::ostream& operator<<(std::ostream& os, const SpriteType& type);

class SpriteFactory
{
private:
	std::map<SpriteType, SpriteConfig> _spriteConfigs;
	
public:
	void registerSprite(SpriteType type, std::string filename, Vector2 dimensions, int numFrames);

	Sprite getSprite(SpriteType type);
};

#endif