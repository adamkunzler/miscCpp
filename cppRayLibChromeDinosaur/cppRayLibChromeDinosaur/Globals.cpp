#include "Globals.h"

Globals::Globals()
{
	_spriteFactory = std::make_unique<SpriteFactory>();
}

SpriteFactory& Globals::spriteFactory() { return *_spriteFactory; }

Globals& Globals::instance() { return _instance; }

Globals Globals::_instance;