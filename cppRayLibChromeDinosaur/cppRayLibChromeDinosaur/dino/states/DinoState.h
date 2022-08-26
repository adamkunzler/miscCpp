#ifndef DINOSTATE_H
#define DINOSTATE_H

// Thoughts on improvements
/*

	- non-static instances - need to manage memory to avoid fragmentation (Game Programming Patterns)
	- StateManager - Game AI by Example
	- Generic State  - Game AI by Example

*/


#include <iostream>

class Dino;

class DinoState
{
public:	
	DinoState()
	{		
	}

	virtual ~DinoState() {}

	virtual void enter(Dino& dino) = 0;
	virtual void exit(Dino& dino) = 0;

	virtual void handleInput(Dino& dino) = 0;
	virtual void update(Dino& dino) = 0;
	virtual void render(Dino& dino) = 0;
};

#endif