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
	
	if (IsKeyPressed(KEY_ONE))   setState(&DinoStateIdle::getInstance());
	if (IsKeyPressed(KEY_TWO))   setState(&DinoStateWalk::getInstance());
	if (IsKeyPressed(KEY_THREE)) setState(&DinoStateRun::getInstance());	
	if (IsKeyPressed(KEY_FIVE))  setState(&DinoStateDie::getInstance());

}

void Dino::update()
{	
	_state->update(*this);	

	if (position.y >= startY) position.y = startY;		

	if (Globals::instance().worldSpeed > 0 && Globals::instance().worldSpeed <= 5 && !isJumping()) 
		setState(&DinoStateWalk::getInstance());

	if (Globals::instance().worldSpeed > 5 && !isJumping()) 
		setState(&DinoStateRun::getInstance());
}

void Dino::render()
{	
	_state->render(*this);	
}

bool Dino::isJumping()
{
	return dynamic_cast<DinoStateJump*>(_state) != nullptr;
}
