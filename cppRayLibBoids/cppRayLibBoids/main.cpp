#include <iostream>
#include <vector>

#define RAYGUI_IMPLEMENTATION

#include "raylib.h"

#include "Boid.h"
#include "SteeringBehaviors.h"

int main(void)
{
	//
	// Initialization	
	//
	const int screenWidth{ 3800 };
	const int screenHeight{ 1900 };

	InitWindow(screenWidth, screenHeight, ".: Boids :.");

	SetTargetFPS(60);

	SteeringBehaviors* steeringBehaviors = new SteeringBehaviors();
	steeringBehaviors->isAlignment = true;
	steeringBehaviors->isSeparation = true;
	steeringBehaviors->isCohesion = true;

	std::vector<Boid> boids;
	for (int i = 0; i < 5000; ++i)
	{
		Boid b{ steeringBehaviors };
		b.position.x = (float)GetRandomValue(0, screenWidth);
		b.position.y = (float)GetRandomValue(0, screenHeight);
		b.velocity.x = (float)GetRandomValue(-5, 5);
		b.velocity.y = (float)GetRandomValue(-5, 5);
		b.heading = glm::normalize(b.velocity);
		b.right = glm::vec2{ -b.heading.y, b.heading.x };
		b.maxSpeed = 5.f;				
		b.radius = 10.f;
		b.fovDistance = 50.f;
		b.maxForce = 2.0f;

		boids.emplace_back(b);
	}

	bool isStarted{ false };

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
			isStarted = !isStarted;
		}


		//
		// Update
		//

		if (isStarted)
		{
			for (auto& b : boids)
				b.update(boids);
		}

		//
		// Draw
		//
		BeginDrawing();
		ClearBackground(BLACK);

		for (auto& b : boids)
			b.render();

		DrawFPS(10, 10);

		// Draw GUI controls
		//------------------------------------------------------------------------------
		//Boid::separationWeight = GuiSliderBar(Rectangle{ 100, 40, 120, 35 }, "Separation", NULL, Boid::separationWeight, 0.f, 10.f);
		//Boid::cohesionWeight = GuiSliderBar(Rectangle{ 100, 80, 120, 35 }, "Cohesion", NULL, Boid::cohesionWeight, 0.f, 10.f);
		//Boid::alignmentWeight = GuiSliderBar(Rectangle{ 100, 120, 120, 35 }, "Alignment", NULL, Boid::alignmentWeight, 0.f, 10.f);
		//------------------------------------------------------------------------------


		EndDrawing();
	}

	//
	// De-Initialization
	//

	CloseWindow();        // Close window and OpenGL context	

	delete steeringBehaviors;

	return 0;
}
