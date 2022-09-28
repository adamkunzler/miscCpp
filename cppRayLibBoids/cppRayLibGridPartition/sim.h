#ifndef SIM_H
#define SIM_H


#include <string>


#include <glm/glm.hpp>
#include "raylib.h"
#include "raymath.h"

#include "Globals.h"
#include "ParticleManager.h"

class Sim
{
public:
	Sim()		
	{
		SetRandomSeed(1711);
		
		_particleManager = std::make_unique<ParticleManager>();

		_camera.zoom = 1.f;
	}

	~Sim() { }

	void update()
	{
		if (!_isStarted) return;		

		_particleManager->update();
	}

	void render()
	{
		BeginDrawing();
		ClearBackground(BLACK);
				
		BeginMode2D(_camera);

		_particleManager->render();

		EndMode2D();

		renderTextDisplay();		

		EndDrawing();
	}	

	void handleInput()
	{
		if (IsKeyPressed(KEY_SPACE)) 
		{
			_isStarted = !_isStarted;
		}

		if (IsKeyPressed(KEY_P))
		{
			_particleManager->isRenderPartition = !_particleManager->isRenderPartition;
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			auto delta = GetMouseDelta();
			delta = Vector2Scale(delta, (- 1.0f / _camera.zoom));

			_camera.target = Vector2Add(_camera.target, delta);
		}		

		float wheel = GetMouseWheelMove();
		if (wheel != 0.f)
		{
			auto mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), _camera);
			_camera.offset = GetMousePosition();
			_camera.target = mouseWorldPos;

			const float zoomIncrement{ 0.125f };
			_camera.zoom += (wheel * zoomIncrement);
			if (_camera.zoom < zoomIncrement) _camera.zoom = zoomIncrement;
		}
	}

private:
	void renderTextDisplay()
	{
		if (1)
		{
			auto mouseWorld = GetScreenToWorld2D(GetMousePosition(), _camera);
			auto strMouseScreen = "Mouse: " + std::to_string(mouseWorld.x) + ", " + std::to_string(mouseWorld.y);
			DrawText(strMouseScreen.c_str(), 10, 30, 16, WHITE);

			DrawFPS(10, 10);
		}
	}

private:		
	Camera2D _camera{ 0 };

	bool _isStarted{ false };

	std::unique_ptr<ParticleManager> _particleManager{ nullptr };
	
};

#endif