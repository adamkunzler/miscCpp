#ifndef GLOBALS_H
#define GLOBALS_H

#include "sprites/SpriteFactory.h"

class Globals
{
private:	
	static Globals _instance;
	
	std::unique_ptr<SpriteFactory> _spriteFactory;

public:
	

public:		
	static Globals& instance();
	
	Globals();

	SpriteFactory& spriteFactory();
};


Vector2& operator+=(Vector2& a, const Vector2& b);

#endif