#ifndef DINOSTATEJUMP_H
#define DINOSTATEJUMP_H

#include <iostream>

#include "../../sprites/Sprite.h"
#include "../Dino.h"
#include "DinoState.h"

class DinoStateJump : public DinoState
{
private:
	Sprite _sprite;
	bool _isJumpFinished{ false };

	Vector2 _velocity;
	
public:
	DinoStateJump();

	void enter(Dino& dino);
	void exit(Dino& dino);
	void handleInput(Dino& dino);
	void update(Dino& dino);
	void render(Dino& dino);

	static DinoState& getInstance();
};

#endif