#include <iostream>

#include "raylib.h"
#include "raymath.h"


struct Dino
{
	Texture2D texture;
	Vector2 position;	
	Rectangle src;
			
	Rectangle boundingBox;

	Dino(const char* filename)
	{
		auto image = LoadImage(filename);
		ImageResize(&image, image.width / 2.f, image.height / 2.f);
		boundingBox = GetImageAlphaBorder(image, 0.f);

		texture = LoadTextureFromImage(image);
		
		src = Rectangle{ 0.f, 0.f, (float)texture.width, (float)texture.height };
		
		position.x = GetScreenWidth() / 2.f;// GetRandomValue(texture.width, GetScreenWidth() - texture.width);
		position.y = GetScreenHeight() / 2.f;// GetRandomValue(texture.height, GetScreenHeight() - texture.height);		
	}

	void update(const Dino& other)
	{						
		// collision with other objects

		Rectangle a{
			boundingBox.x + position.x,
			boundingBox.y + position.y,
			boundingBox.width,
			boundingBox.height
		};

		Rectangle b{
			other.boundingBox.x + other.position.x,
			other.boundingBox.y + other.position.y,
			other.boundingBox.width,
			other.boundingBox.height
		};

		//if (CheckCollisionRecs(a, b))
		if (CheckCollision(a, b))
		{
			std::cout << "*";
		}
	}

	bool CheckCollision(const Rectangle& a, const Rectangle& b) const
	{
		if(
			a.x + a.width > b.x
			&& a.x < b.x + b.width
			&& a.y + a.height > b.y
			&& a.y < b.y + b.height)
		{
			return true;
		}

		return false;
	}

	void render()
	{				
		auto halfImageWidth = texture.width / 2.f;
		auto halfImageHeight= texture.height / 2.f;

		auto dest = Rectangle{ 
			position.x, 
			position.y, 
			(float)texture.width, 
			(float)texture.height };
					
		DrawTexture(texture, position.x, position.y, WHITE);
				
		// sprite bounds
		DrawRectangleLines(position.x, position.y, texture.width, texture.height, SKYBLUE);
		
		// sprite position (top left)
		DrawCircle(position.x, position.y, 5.f, PURPLE);		

		// sprite bounding box
		DrawRectangleLines(
			boundingBox.x + position.x,
			boundingBox.y + position.y,
			boundingBox.width,
			boundingBox.height,
			GREEN);

	}
};


int main(void)
{
	//
	// Initialization	
	//
	const int screenWidth{ 1024 };
	const int screenHeight{ 1024 };

	InitWindow(screenWidth, screenHeight, ".: Image Mask Collision :.");
	HideCursor();

	SetTargetFPS(60);

	Dino d1{ "dead.png" };
	Dino d2{ "idle.png" };	


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


		//
		// Update
		//				
		d2.position = GetMousePosition();
		d2.update(d1);



		//
		// Draw
		//
		BeginDrawing();
		ClearBackground(BLACK);

		d1.render();
		d2.render();

		EndDrawing();
	}

	//
	// De-Initialization
	//

	CloseWindow();        // Close window and OpenGL context	

	return 0;
}
