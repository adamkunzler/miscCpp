#include <iostream>

#include "raylib.h"

#include "DinoGame.h"

int main(void)
{	
	//
	// Initialization	
	//
	const int screenWidth{ 2048 };
	const int screenHeight{ 1024 };

	InitWindow(screenWidth, screenHeight, ".: Dinosaur Game :.");

	SetTargetFPS(60);
	
	DinoGame game;

	//
	// MAIN RENDER LOOP
	//
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		//
		// Process Inputs
		//		
		game.handleInput();

		//
		// Update
		//
		game.update();

		//
		// Draw
		//
		BeginDrawing();
		ClearBackground(BLACK);

		game.render();

		EndDrawing();
	}

	//
	// De-Initialization
	//

	CloseWindow();        // Close window and OpenGL context	

	return 0;
}
