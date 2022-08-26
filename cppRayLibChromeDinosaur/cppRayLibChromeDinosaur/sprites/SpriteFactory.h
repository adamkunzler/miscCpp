#ifndef SPRITEFACTORY_H
#define SPRITEFACTORY_H

#include <iostream>
#include <map>
#include <stdexcept>

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
	void registerSprite(SpriteType type, const char* filename, Vector2 dimensions);

	Sprite getSprite(SpriteType type);
};

#endif