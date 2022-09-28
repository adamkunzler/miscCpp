#include "Particle.h"

uint32_t Particle::_nextId{ 1 };

uint32_t Particle::getId() { return _nextId++; }

Particle::Particle()
{
	id = getId();
}

void Particle::update(const std::vector<Particle>& particles)
{
	//
	// collisions
	//
	color = RAYWHITE;
	for (const auto& p : particles)
	{
		if (p.id == id) continue;

		if (isIntersects(
			position.x, position.y, radius * 2.f, radius * 2.f,
			p.position.x, p.position.y, p.radius * 2.f, p.radius * 2.f))
		{
			color = SKYBLUE;
			break;
		}
	}

	//
	// update position (and wrap around screen)
	//
	position += velocity;

	if (position.x > GetScreenWidth()) position.x = -radius;
	if (position.y > GetScreenHeight()) position.y = -radius;
	if (position.x + radius < 0) position.x = (float)GetScreenWidth();
	if (position.y + radius < 0) position.y = (float)GetScreenHeight();
}

void Particle::update()
{
	//
	// update position (and wrap around screen)
	//
	position += velocity;

	if (position.x > GetScreenWidth()) position.x = -radius;
	if (position.y > GetScreenHeight()) position.y = -radius;
	if (position.x + radius < 0) position.x = (float)GetScreenWidth();
	if (position.y + radius < 0) position.y = (float)GetScreenHeight();
}

void Particle::render()
{
	DrawRectangle(
		(int)(position.x - radius), (int)(position.y - radius),
		(int)(radius * 2.f), (int)(radius * 2.f), color);









		//DrawTexture(texture, (int)(position.x - radius), (int)(position.y - radius), WHITE);
	
	//Color tint = WHITE;

	//if (texture.id <= 0) return;

	//float width = (float)texture.width;
	//float height = (float)texture.height;

	//Rectangle source = { 0.0f, 0.0f, width, height };
	//Rectangle dest = { position.x - radius, position.y - radius, width, height };

	//Vector2 topLeft = { dest.x, dest.y };
	//Vector2 topRight = Vector2{ dest.x + dest.width, dest.y };
	//Vector2 bottomLeft = Vector2{ dest.x, dest.y + dest.height };
	//Vector2 bottomRight = Vector2{ dest.x + dest.width, dest.y + dest.height };


	//rlCheckRenderBatchLimit(4);     // Make sure there is enough free space on the batch buffer

	//rlSetTexture(texture.id);
	//rlBegin(RL_QUADS);

	//rlColor4ub(tint.r, tint.g, tint.b, tint.a);
	//rlNormal3f(0.0f, 0.0f, 1.0f); // Normal vector pointing towards viewer

	//// Top-left corner for texture and quad            
	//rlTexCoord2f(source.x / width, source.y / height);
	//rlVertex2f(topLeft.x, topLeft.y);

	//// Bottom-left corner for texture and quad
	//rlTexCoord2f(source.x / width, (source.y + source.height) / height);
	//rlVertex2f(bottomLeft.x, bottomLeft.y);

	//// Bottom-right corner for texture and quad
	//rlTexCoord2f((source.x + source.width) / width, (source.y + source.height) / height);
	//rlVertex2f(bottomRight.x, bottomRight.y);

	//// Top-right corner for texture and quad
	//rlTexCoord2f((source.x + source.width) / width, source.y / height);
	//rlVertex2f(topRight.x, topRight.y);

	//rlEnd();
	//rlSetTexture(0);

}