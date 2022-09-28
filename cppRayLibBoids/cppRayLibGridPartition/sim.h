#ifndef SIM_H
#define SIM_H

#include <vector>
#include <string>
#include <span>

#include <glm/glm.hpp>
#include "raylib.h"
#include "raymath.h"

#include "Globals.h"
#include "Particle.h"
#include "GridPartition.h"
#include "QueryResult.h"
#include "ThreadPool.h"

template <typename Span>
void updateParticleBatch(Span span, GridPartition& partition)
{		
	for (auto& p : span)
	{		
		p.update();
		
		partition.query(p.position, p.neighbors); // ??? seems like a bad idea to do this here

		// bounce off walls
		if (p.position.x < p.radius)
		{
			p.position.x = p.radius + 1.f;
			p.velocity.x = -p.velocity.x;
		}
		if (p.position.y < p.radius)
		{
			p.position.y = p.radius + 1.f;
			p.velocity.y = -p.velocity.y;
		}
		if (p.position.x > WORLD_WIDTH - p.radius)
		{
			p.position.x = WORLD_WIDTH - p.radius - 1.f;
			p.velocity.x = -p.velocity.x;
		}
		if (p.position.y > WORLD_HEIGHT - p.radius)
		{
			p.position.y = WORLD_HEIGHT - p.radius - 1.f;
			p.velocity.y = -p.velocity.y;
		}		
	}	
}

class Sim
{
public:
	Sim(size_t numParticles)		
	{
		SetRandomSeed(1711);
		
		initParticles(numParticles);
		
		// create and initialize the partition
		_partition.init(CELL_SIZE); // should be half of flocking radius | should be evenly divisble by screen dims
		for (auto& p : _particles) _partition.add(&p);		

		initParticleUpdateChunks();

		// create a texture to be shared by particles
		auto temp = LoadImage("moon.png");
		ImageResize(&temp, (int)(DEFAULT_RADIUS * 2.f), (int)(DEFAULT_RADIUS * 2.f));
		_particleTexture = LoadTextureFromImage(temp);
		UnloadImage(temp);

		_camera.zoom = 1.f;
	}

	~Sim()
	{

	}

	void update()
	{
		if (!_isStarted) return;		

		updateMultiThread();							

		//_partition.clear();
		for (auto& p : _particles)
			_partition.move(&p);
			//_partition.add(&p);				
	}

	void render()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		BeginMode2D(_camera);

		if(_isRenderPartition)
			_partition.render();

		for (auto& p : _particles)
			p.render(_particleTexture);			

		EndMode2D();

		// text displays
		if (1)
		{
			auto mouseWorld = GetScreenToWorld2D(GetMousePosition(), _camera);
			auto strMouseScreen = "Mouse: " + std::to_string(mouseWorld.x) + ", " + std::to_string(mouseWorld.y);
			DrawText(strMouseScreen.c_str(), 10, 30, 16, WHITE);
		
			DrawFPS(10, 10);
		}		

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
			_isRenderPartition = !_isRenderPartition;
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
	void initParticles(size_t numParticles)
	{
		_particles.clear();
		_particles.reserve(numParticles);
		
		for (size_t i = 0; i < numParticles; ++i)
		{		
			Particle p;
			p.position = glm::vec2{
				(float)GetRandomValue((int)DEFAULT_RADIUS, (int)(WORLD_WIDTH - DEFAULT_RADIUS)),
				(float)GetRandomValue((int)DEFAULT_RADIUS, (int)(WORLD_HEIGHT - DEFAULT_RADIUS))};
			p.oldPosition = p.position;

			p.velocity = glm::vec2{
				(float)GetRandomValue(-5, 5),
				(float)GetRandomValue(-5, 5) };

			// prevent velocity of 0 (glm::normalize will give nan)
			if (p.velocity.x == 0.f) p.velocity.x = 3.5f;
			if (p.velocity.y == 0.f) p.velocity.y = 1.5f;

			p.velocity = glm::normalize(p.velocity) * MAX_VELOCITY;

			p.radius = DEFAULT_RADIUS;
			p.mass = (float)GetRandomValue(10, 100) / 10.f;

			_particles.emplace_back(p);			
		}
	}
	
	void initParticleUpdateChunks()
	{
		int chunkSize = (int)(_particles.size() / MAX_THREADS);
		int startRange = 0;
		int endRange = chunkSize;
		for (int i = 0; i < MAX_THREADS; ++i)
		{
			// create range spans
			std::span<Particle> span(
				_particles.begin() + startRange,
				_particles.begin() + endRange);
			_ranges.emplace_back(span);

			startRange += chunkSize;
			endRange += chunkSize;
		}
	}
	
	void updateMultiThread()
	{
		for (auto& range : _ranges)
			_threadPool.enqueue(std::bind(updateParticleBatch<std::span<Particle>>, range, _partition));

		_threadPool.waitFinished();

		// collision detection
		for (auto& p : _particles)
		{
			for (auto& n : p.neighbors)
			{
				if (n == nullptr) continue;

				Particle* temp = n;
				while (temp != nullptr)
				{					
					p.checkColllision(*temp);
					temp = temp->next;
				}
			}			
		}
	}

private:		
	Camera2D _camera{ 0 };

	bool _isStarted{ false };
	bool _isRenderPartition{ false };

	Texture _particleTexture;

	ThreadPool _threadPool{ MAX_THREADS };
	std::vector<std::span<Particle>> _ranges;
	
	GridPartition _partition;
	std::vector<Particle> _particles;	
};

#endif