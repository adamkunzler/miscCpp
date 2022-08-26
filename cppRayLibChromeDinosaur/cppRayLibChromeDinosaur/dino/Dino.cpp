#include "Dino.h"


Dino::Dino()	
{
	setState(&DinoStateIdle::getInstance());
}
	
void Dino::setState(DinoState* state)
{
	if(_state) _state->exit(*this);
	_previousState = _state;
	_state = state;
	_state->enter(*this);
}

void Dino::restorePreviousState()
{
	setState(_previousState);
}

void Dino::handleInput()
{
	_state->handleInput(*this);	
}

void Dino::update()
{	
	_state->update(*this);	

	if (position.y >= startY) position.y = startY;		
		
}

void Dino::render()
{	
	_state->render(*this);	
}

bool Dino::isJumping()
{
	return dynamic_cast<DinoStateJump*>(_state) != nullptr;
}

bool Dino::isDyingOrDead()
{
	auto isDying = dynamic_cast<DinoStateDie*>(_state) != nullptr;
	auto isDead = dynamic_cast<DinoStateDead*>(_state) != nullptr;
	return isDying || isDead;
}
