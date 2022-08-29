#ifndef DINOGAME_H
#define DINOGAME_H

#include "sprites/SpriteFactory.h"
#include "Dino/Dino.h"
#include "Background.h"

class DinoGame
{
private:	
	static DinoGame _instance;

	std::unique_ptr<SpriteFactory> _spriteFactory;

	std::unique_ptr<Dino> _dino = 0;
	Background _background;	
	
	// obstacles	

public:
	float worldSpeed{ 0.f };

public:
	static DinoGame& instance();

	DinoGame();

	void init();

	void handleInput();

	void update();

	void render();

	SpriteFactory& spriteFactory();
};

Vector2& operator+=(Vector2& a, const Vector2& b);

#endif