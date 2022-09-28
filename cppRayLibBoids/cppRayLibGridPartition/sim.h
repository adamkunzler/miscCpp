#ifndef SIM_H
#define SIM_H

#include <vector>
#include <string>
#include <span>

#include <glm/glm.hpp>
#include "raylib.h"

#include "Globals.h"
#include "Particle.h"
#include "GridPartition.h"
#include "QueryResult.h"
#include "ThreadPool.h"

template <typename Span>
void updateParticleBatch(Span span, int screenWidth, int screenHeight, GridPartition& partition)
{		
	for (auto& p : span)
	{		
		p.update();
		
		partition.query(p.position, p.neighbors); // ??? seems like a bad idea to do this here

		// bounce off walls
		if (p.position.x < p.radius) p.velocity.x = -p.velocity.x;
		if (p.position.y < p.radius) p.velocity.y = -p.velocity.y;
		if (p.position.x > screenWidth - p.radius) p.velocity.x = -p.velocity.x;
		if (p.position.y > screenHeight - p.radius) p.velocity.y = -p.velocity.y;
	}	
}

class Sim
{
public:
	Sim(int screenWidth, int screenHeight, size_t numParticles)
		: _screenWidth{screenWidth}, _screenHeight{ screenHeight }
	{
		SetRandomSeed(1711);
		
		initParticles(numParticles);
		
		// create and initialize the partition
		_partition.init(screenWidth, screenHeight, CELL_SIZE); // should be half of flocking radius | should be evenly divisble by screen dims
		for (auto& p : _particles) _partition.add(&p);		

		initParticleUpdateChunks();

		// create a texture to be shared by particles
		auto temp = LoadImage("moon.png");
		ImageResize(&temp, (int)(DEFAULT_RADIUS * 2.f), (int)(DEFAULT_RADIUS * 2.f));
		_particleTexture = LoadTextureFromImage(temp);
		UnloadImage(temp);
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

		if(_isRenderPartition)
			_partition.render();

		for (auto& p : _particles)
			p.render(_particleTexture);			

		// text displays
		auto strMouseScreen = "Mouse: " + std::to_string(GetMouseX()) + ", " + std::to_string(GetMouseY());
		DrawText(strMouseScreen.c_str(), 10, 10, 16, WHITE);

		auto mouseCell = _partition.getCell((float)GetMouseX(), (float)GetMouseY());
		auto strMouseCell = "Cell: " + std::to_string(mouseCell.x) + ", " + std::to_string(mouseCell.y);
		DrawText(strMouseCell.c_str(), 10, 30, 16, WHITE);

		DrawFPS(10, _screenHeight - 20);

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

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			/*std::vector<Particle*> results;
			results.reserve(_particles.size());*/

			auto mousePosition = glm::vec2{ GetMouseX(), GetMouseY() };
			//_partition.query(mousePosition, results);		
						
			/*for (auto& r : results) {				
				std::cout << r->id() << std::endl;
			}
			std::cout << std::endl;*/
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
				(float)GetRandomValue((int)DEFAULT_RADIUS, (int)(_screenWidth - DEFAULT_RADIUS)),
				(float)GetRandomValue((int)DEFAULT_RADIUS, (int)(_screenHeight - DEFAULT_RADIUS))};
			p.oldPosition = p.position;

			p.velocity = glm::vec2{
				(float)GetRandomValue(-5, 5),
				(float)GetRandomValue(-5, 5) };

			p.velocity = glm::normalize(p.velocity) * MAX_VELOCITY;

			p.radius = DEFAULT_RADIUS;

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

	void updateSingleThread()
	{		
		for (auto& p : _particles)
		{
			p.update();

			// fix bounds
			if (p.position.x < -p.radius) p.position.x = _screenWidth + p.radius;
			if (p.position.y < -p.radius) p.position.y = _screenHeight + p.radius;
			if (p.position.x > _screenWidth + p.radius) p.position.x = -p.radius;
			if (p.position.y > _screenHeight + p.radius) p.position.y = -p.radius;
		}	
	}

	void updateMultiThread()
	{
		for (auto& range : _ranges)
			_threadPool.enqueue(std::bind(updateParticleBatch<std::span<Particle>>, range, _screenWidth, _screenHeight, _partition));

		_threadPool.waitFinished();
	}

private:	
	int _screenWidth{ 0 };
	int _screenHeight{ 0 };

	bool _isStarted{ false };
	bool _isRenderPartition{ false };

	Texture _particleTexture;

	ThreadPool _threadPool{ MAX_THREADS };
	std::vector<std::span<Particle>> _ranges;
	
	GridPartition _partition;
	std::vector<Particle> _particles;	
};

#endif