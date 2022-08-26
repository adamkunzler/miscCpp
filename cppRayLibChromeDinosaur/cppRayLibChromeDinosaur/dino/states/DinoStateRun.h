#ifndef DINOSTATERUN_H
#define DINOSTATERUN_H

#include <iostream>

#include "../../Globals.h"
#include "../../sprites/Sprite.h"
#include "../Dino.h"
#include "DinoState.h"

class DinoStateRun : public DinoState
{
private:
	Sprite _sprite;

public:
	DinoStateRun();

	void enter(Dino& dino);
	void exit(Dino& dino);
	void handleInput(Dino& dino);
	void update(Dino& dino);
	void render(Dino& dino);

	static DinoState& getInstance();
};

#endif