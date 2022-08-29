#ifndef DINOSTATEDEAD_H
#define DINOSTATEDEAD_H

#include <iostream>

#include "../../sprites/Sprite.h"
#include "../Dino.h"
#include "DinoState.h"

class DinoStateDead : public DinoState
{
private:
	Sprite _sprite;

public:
	DinoStateDead();

	void enter(Dino& dino);
	void exit(Dino& dino);
	void handleInput(Dino& dino);
	void update(Dino& dino);
	void render(Dino& dino);

	static DinoState& getInstance();
};

#endif;