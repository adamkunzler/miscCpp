#ifndef DINOGAME_H
#define DINOGAME_H

#include "Globals.h"
#include "Dino/Dino.h"

class DinoGame
{
private:	
	std::unique_ptr<Dino> _dino = 0;
	// obstacles
	// background
	// ground
	// clouds?

public:
	DinoGame();

	void handleInput();

	void update();

	void render();
};

#endif