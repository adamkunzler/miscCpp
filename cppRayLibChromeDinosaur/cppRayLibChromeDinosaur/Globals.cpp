#include "Globals.h"

Globals::Globals()
{
	_spriteFactory = std::make_unique<SpriteFactory>();
}

SpriteFactory& Globals::spriteFactory() { return *_spriteFactory; }

Globals& Globals::instance() { return _instance; }

Globals Globals::_instance;

// ------------------------------------------------------------------

Vector2& operator+=(Vector2& a, const Vector2& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}