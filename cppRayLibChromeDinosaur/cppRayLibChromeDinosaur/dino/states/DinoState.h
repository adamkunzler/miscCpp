#ifndef DINOSTATE_H
#define DINOSTATE_H

#pragma message("Loading DinoState.h...")

#include <iostream>

class Dino;

class DinoState
{
public:	
	DinoState()
	{
		std::cout << "DinoState ctor" << std::endl;
	}

	virtual ~DinoState() {}

	virtual void enter(Dino& dino) = 0;
	virtual void exit(Dino& dino) = 0;

	virtual void handleInput(Dino& dino) = 0;
	virtual void update(Dino& dino) = 0;
	virtual void render(Dino& dino) = 0;
};

#endif