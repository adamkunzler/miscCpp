#ifndef GLOBALS_H
#define GLOBALS_H

#include "sprites/SpriteFactory.h"

class Globals
{
private:	
	static Globals _instance;
	
	std::unique_ptr<SpriteFactory> _spriteFactory;

public:		
	static Globals& instance();
	
	Globals();

	SpriteFactory& spriteFactory();
};

#endif