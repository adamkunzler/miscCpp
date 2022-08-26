#ifndef DINO_H
#define DINO_H

#pragma message("Loading Dino.h...")

#include "raylib.h"

#include "states/DinoStateIdle.h"
#include "states/DinoStateWalk.h"
#include "states/DinoStateRun.h"
#include "states/DinoStateJump.h"
#include "states/DinoStateDie.h"
#include "states/DinoStateDead.h"

class DinoState;

class Dino
{
private:		
	DinoState* _state{ 0 };
	DinoState* _previousState{ 0 };
			
public:
	Vector2 position{ 0, 0 };
	float size{ 256.f };
	
public:	
	Dino();
		
	void handleInput();
	void update();
	void render();

	void setState(DinoState* state);
};

#endif