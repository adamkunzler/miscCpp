#ifndef DINOSTATEDIE_H
#define DINOSTATEDIE_H

#include <iostream>

#include "../../sprites/Sprite.h"
#include "../Dino.h"
#include "DinoState.h"

class DinoGame;

class DinoStateDie : public DinoState
{
private:
	Sprite _sprite;

public:
	DinoStateDie();

	void enter(Dino& dino);
	void exit(Dino& dino);
	void handleInput(Dino& dino);
	void update(Dino& dino);
	void render(Dino& dino);

	static DinoState& getInstance();
};

#endif