#include <iostream>
#include <vector>

#include "raylib.h"

void initCells(std::vector<Vector2>& cells, std::vector<Vector2>& cellVelocities, int n, int cellSize);

int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 2000;
	const int screenHeight = screenWidth;

	InitWindow(screenWidth, screenHeight, ".: RayLib Tesselation :.");

	// 0 0 0 0
	// 0 1 1 0
	// 0 1 1 0
	// 0 0 0 0

	// 0 offscreen
	// 1 screen

	const int cellN{ 48 }; // N x N cells - needs to be even
	const int cellSize{ screenWidth / (cellN - 2) }; // -2 since top/bottom/left/right cells are offscreen
	const int cellOffset{ cellSize };

	std::vector<Vector2> cells;
	std::vector<Vector2> cellVelocities;
	initCells(cells, cellVelocities, cellN, cellSize);

	const Color colors[21 - 3] = {
		//LIGHTGRAY,
		//GRAY,
		//DARKGRAY,
		YELLOW,
		GOLD,
		ORANGE,
		PINK,
		RED,
		MAROON,
		GREEN,
		LIME,
		DARKGREEN,
		SKYBLUE,
		BLUE,
		DARKBLUE,
		PURPLE,
		VIOLET,
		DARKPURPLE,
		BEIGE,
		BROWN,
		DARKBROWN
	};

	std::vector<Color> greens;
	for (int i = 0; i < 1000000; ++i)
	{
		greens.push_back(ColorFromHSV(120.f, (float)GetRandomValue(42, 100) / 100.f, (float)GetRandomValue(37, 100) / 100.f));
	}

	SetTargetFPS(60);

	// MAIN RENDER LOOP
	//--------------------------------------------------------------------------------------
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		if (IsKeyPressed(KEY_SPACE))
		{
			initCells(cells, cellVelocities, cellN, cellSize);
		}

		if (IsKeyPressed(KEY_F)) ToggleFullscreen();

		// Update
		//----------------------------------------------------------------------------------

		// bounce cells around
		for (int y = 0; y < cellN; ++y)
		{
			for (int x = 0; x < cellN; ++x)
			{
				auto index = x + y * cellN;

				auto nx = cells[index].x += cellVelocities[index].x;
				auto ny = cells[index].y += cellVelocities[index].y;

				if (nx < 0 || nx > cellSize) cellVelocities[index].x *= -1;
				if (ny < 0 || ny > cellSize) cellVelocities[index].y *= -1;

				cells[index].x = nx;
				cells[index].y = ny;
			}
		}
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		ClearBackground(BLACK);

		// draw tesselation
		for (int y = 1; y < cellN; ++y)
		{
			for (int x = 0; x < cellN - 1; ++x)
			{
				auto ax = x;
				auto ay = y;
				Vector2 a
				{
					ax * cellSize - cellOffset + cells[ax + ay * cellN].x,
					ay * cellSize - cellOffset + cells[ax + ay * cellN].y,
				};

				auto bx = x + 1;
				auto by = y;
				Vector2 b
				{
					bx * cellSize - cellOffset + cells[bx + by * cellN].x,
					by * cellSize - cellOffset + cells[bx + by * cellN].y,
				};

				auto cx = x;
				auto cy = y - 1;
				Vector2 c
				{
					cx * cellSize - cellOffset + cells[cx + cy * cellN].x,
					cy * cellSize - cellOffset + cells[cx + cy * cellN].y,
				};

				auto dx = x + 1;
				auto dy = y - 1;
				Vector2 d
				{
					dx * cellSize - cellOffset + cells[dx + dy * cellN].x,
					dy * cellSize - cellOffset + cells[dx + dy * cellN].y,
				};
				
				DrawTriangle(a, b, c, greens.at(x + y * cellN));
				DrawTriangle(b, d, c, greens.at(x + y * cellN + 10));
			}
		}

		// draw points in cells
		/*for (int y = 0; y < cellN; ++y)
		{
			for (int x = 0; x < cellN; ++x)
			{
				auto sx = x * cellSize - cellOffset + cells[x + y * cellN].x;
				auto sy = y * cellSize - cellOffset + cells[x + y * cellN].y;
				DrawCircle(sx, sy, 5.f, BLACK);
			}
		}*/

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

void initCells(std::vector<Vector2>& cells, std::vector<Vector2>& cellVelocities, int n, int cellSize)
{
	cells.clear();
	cellVelocities.clear();

	for (int i = 0; i < n * n; ++i)
	{
		auto randX = (float)GetRandomValue(0, cellSize);
		auto randY = (float)GetRandomValue(0, cellSize);

		cells.push_back(Vector2{ randX, randY });

		auto velX = (float)GetRandomValue(-100, 100) / 50.f;
		auto velY = (float)GetRandomValue(-100, 100) / 50.f;

		cellVelocities.push_back(Vector2{ velX, velY });
	}
}