#include <iostream>
#include <string>

#include "raylib.h"
#include "raymath.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "HexagonMap.h"

int main(void)
{
	//
	// Initialization	
	//
	const int screenWidth{ 1024 };
	const int screenHeight{ 1024 };

	InitWindow(screenWidth, screenHeight, ".: Hexagons :.");

	SetTargetFPS(60);

	HexagonMapConfig mapConfig;
	mapConfig.screenCenter.x = screenWidth / 2.f;
	mapConfig.screenCenter.y = screenHeight / 2.f;
	mapConfig.mapDim.x = 10;
	mapConfig.mapDim.y = 7;
	mapConfig.tileSideLength = 45.f;
	mapConfig.tileDim.x = mapConfig.tileSideLength * 2.f;
	mapConfig.tileDim.y = mapConfig.tileSideLength * glm::sqrt(3.0);

	// centers tilemap in window
	// center screen     width/height of tile       num tiles     halved
	mapConfig.startPos.x = ((mapConfig.screenCenter.x - (mapConfig.tileDim.x * mapConfig.mapDim.x) / 2.f)) + mapConfig.tileDim.x;
	mapConfig.startPos.y = ((mapConfig.screenCenter.y - (mapConfig.tileDim.y * mapConfig.mapDim.y) / 2.f)) + mapConfig.tileDim.y;

	HexagonMap map{ mapConfig };
	map.initPointEndUp();
	//map.initFlatEndUp();

	Vector2 mousePos{ 0, 0 };

	//
	// MAIN RENDER LOOP
	//
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		//
		// Process Inputs
		//

		if (IsKeyPressed(KEY_SPACE))
		{
			// do something...
		}

		//if (IsMouseBUtt(MOUSE_BUTTON_LEFT))
		//{
		mousePos = GetMousePosition();
		//}

		//
		// Update
		//

		// TODO

		//
		// Draw
		//
		BeginDrawing();
		ClearBackground(BLACK);

		map.render();



		glm::vec2 mapPos{ 0, 0 };

		// offset of start position
		mapPos.x = mousePos.x - mapConfig.startPos.x;
		mapPos.y = mousePos.y - mapConfig.startPos.y;
		
		// to square dim
		mapPos.x /= mapConfig.tileDim.x;
		mapPos.y /= mapConfig.tileDim.y;

		//auto hexCenterX = _config.tileSideLength * 1.5f * y;
		//auto hexCenterY = _config.tileDim.y * x + (y % 2 == 0 ? _config.tileDim.y / 2 : 0);

		std::string screenCoord = "Screen Coord: " + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y);		
		std::string mapCoord = "Map Coord: " + std::to_string(mapPos.x) + ", " + std::to_string(mapPos.y);

		DrawText(screenCoord.c_str(), 25, 25, 24, RAYWHITE);
		DrawText(mapCoord.c_str(), 25, 55, 24, RAYWHITE);

		EndDrawing();
	}

	//
	// De-Initialization
	//

	CloseWindow();        // Close window and OpenGL context	

	return 0;
}
