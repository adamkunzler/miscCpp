#ifndef DINOSTATEIDLE_H
#define DINOSTATEIDLE_H

#include <iostream>

#include "../../sprites/Sprite.h"
#include "../Dino.h"
#include "DinoState.h"

class DinoGame;

class DinoStateIdle : public DinoState
{
private:
	Sprite _sprite;

public:
	DinoStateIdle();

	void enter(Dino& dino);
	void exit(Dino& dino);
	void handleInput(Dino& dino);
	void update(Dino& dino);
	void render(Dino& dino);

	static DinoState& getInstance();
};

#endif;