#ifndef PARTICLE_H
#define PARTICLE_H

#include "raylib.h"
#include "RayLibMathHelper.h"
#include "GlobalSettings.h"

class Particle
{
private:
	GlobalSettings _settings;

public:
	Vector2 position;
	Vector2 oldPosition;
	Vector2 velocity;
	Vector2 acceleration;
	Color color;

	Particle(
		const GlobalSettings& settings, 
		Vector2 position, Vector2 oldPosition, 
		Vector2 velocity, Vector2 acceleration, 
		Color color) 
		: _settings{ settings }, 
		position{ position }, oldPosition{ oldPosition }, 
		velocity{ velocity }, acceleration{ acceleration }, 
		color{ color }
	{};

	void render(bool asCircle = false) const
	{
		if(asCircle)
			DrawCircle((int)position.x, (int)position.y, 5.f, color);
		else		
			DrawLine((int)position.x, (int)position.y, (int)oldPosition.x, (int)oldPosition.y, color);
	}

	void update(bool wrapBounds = true)
	{
		oldPosition = position;
		position += velocity;
		velocity += acceleration;
		
		acceleration.x = 0.f;
		acceleration.y = 0.f;

		velocity = Vector2Normalize(velocity);
		velocity = Vector2Scale(velocity, _settings.maxVelocity);

		// window bounds checking/wrapping
		if (!wrapBounds) return;

		if (position.x > _settings.screenWidth) {
			position.x = 0.f;
			oldPosition = position;
		}
		
		if (position.x < 0) {
			position.x = (float)_settings.screenWidth;
			oldPosition = position;
		}
		
		if (position.y > _settings.screenHeight) {
			position.y = 0.f;
			oldPosition = position;
		}
		
		if (position.y < 0) {
			position.y = (float)_settings.screenHeight;
			oldPosition = position;
		}
	}
};

#endif