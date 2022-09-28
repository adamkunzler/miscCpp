#include <iostream>

#include "raylib.h"
#include "raymath.h"

#include "Globals.h"
#include "Sim.h"



int main(void)
{
	//
	// Initialization	
	//
	const int screenWidth{ 2048 };
	const int screenHeight{ 1920 };
		
	InitWindow(screenWidth, screenHeight, ".: Grid Partition :.");

	SetTargetFPS(60);

	Sim sim;

	//
	// MAIN RENDER LOOP
	//
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		//
		// Process Inputs
		//
		sim.handleInput();

		//
		// Update
		//
		sim.update();

		//
		// Draw
		//
		sim.render();
	}

	//
	// De-Initialization
	//
	CloseWindow();        // Close window and OpenGL context	

	return 0;
}
