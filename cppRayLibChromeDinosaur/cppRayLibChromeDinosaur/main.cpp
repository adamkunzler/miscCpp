#include <iostream>

#include "raylib.h"

#include "DinoGame.h"

//DinoGame::instance().

int main(void)
{	
	//
	// Initialization	
	//
	const int screenWidth{ 1750 };
	const int screenHeight{ 793 };

	InitWindow(screenWidth, screenHeight, ".: Dinosaur Game :.");

	SetTargetFPS(60);

	DinoGame::instance().init();

	//
	// MAIN RENDER LOOP
	//
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		//
		// Process Inputs
		//		
		DinoGame::instance().handleInput();

		//
		// Update
		//
		DinoGame::instance().update();

		//
		// Draw
		//
		BeginDrawing();
		ClearBackground(BLACK);

		DinoGame::instance().render();

		EndDrawing();
	}

	//
	// De-Initialization
	//

	CloseWindow();        // Close window and OpenGL context	

	return 0;
}
