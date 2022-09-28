#include <iostream>
#include <vector>
#include <memory>
#include <span>

#include "raylib.h"
#include "raymath.h"

#include "ThreadPool.h"
#include "utils.h"
#include "Particle.h"
#include "ISpatialPartition.h"
#include "Quadtree.h"
#include "NullPartition.h"

#define RENDER_QUADTREE
#define RENDER_QUADTREE_MOUSE_REGION

#define MAX_BATCH_ELEMENTS  8192

template <typename Span>
void updateParticleBatch(Span span)
{
	for (auto& p : span)	
		p.update();	
}

int main(void)
{
	//
	// Initialization	
	//
	//const int screenWidth{ 2400 };
	//const int screenHeight{ 1800 };
	const int screenWidth{ 2048 };
	const int screenHeight{ 1024 };

	InitWindow(screenWidth, screenHeight, ".: POC Quadtree :.");

	SetTargetFPS(60);	

	//
	// settings
	//
	const uint32_t numParticles{ 500000 };
	const float radius{ 2.f };
	const float fovRadius{ radius * 2.f };
	
	auto imageParticle = GenImageColor((int)(radius * 2.f), (int)(radius * 2.f), RAYWHITE);
	auto textureParticle = LoadTextureFromImage(imageParticle);

	//
	// init the particles
	//
	std::vector<Particle> particles;
	for (uint32_t i = 0; i < numParticles; ++i)
	{
		Particle p;
		p.color = RAYWHITE;
		p.radius = radius;
		p.position = glm::vec2{ (float)GetRandomValue(0, GetScreenWidth()), (float)GetRandomValue(0, GetScreenHeight()) };
		p.velocity = glm::vec2{ (float)GetRandomValue(-5, 5), (float)GetRandomValue(-5, 5) };
		p.velocity = glm::normalize(p.velocity) * 3.f;
		p.texture = textureParticle;

		particles.emplace_back(p);		
	}		
	
	//
	// setup spatial partioning
	//
	SpatialRect<Particle> boundary{
		0.f, 0.f,
		screenWidth, screenHeight };

	std::unique_ptr<ISpatialPartition<Particle>> quadtree = nullptr;
	//quadtree = std::make_unique<Quadtree<Particle>>(boundary);
	
	// allocate space for quadtree query results
	std::vector<Particle> queryResults;
	queryResults.reserve(100);

	//
	// PRE-GAME INIT
	//

	bool started{ false };

#ifdef RENDER_QUADTREE_MOUSE_REGION
	std::vector<Particle> qtMouseQueryResults;
	float rangeDim{ 100.f };
	SpatialRect<Particle> range{
			GetMousePosition().x, GetMousePosition().y, rangeDim, rangeDim };
	qtMouseQueryResults.reserve(500);	
#endif
	
	//
	// CONCURRENCY SETUP
	//
	const size_t numThreads{ 16 };
	int chunkSize = (int)(particles.size() / numThreads);
	
	ThreadPool threadpool(numThreads);
	
	std::vector<std::span<Particle>> ranges;
		
	int startRange = 0;
	int endRange = chunkSize;
	for (int i = 0; i < numThreads; ++i)
	{		
		// create range spans
		std::span<Particle> span(
			particles.begin() + startRange,
			particles.begin() + endRange);
		ranges.emplace_back(span);

		startRange += chunkSize;
		endRange += chunkSize;	
	}			

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
			started = !started;
		}

		//
		// Update
		//

#ifdef RENDER_QUADTREE_MOUSE_REGION
		if (quadtree)
		{
			auto range = SpatialRect<Particle>{
				GetMousePosition().x - (rangeDim / 2.f), GetMousePosition().y - (rangeDim / 2.f), rangeDim, rangeDim };

			qtMouseQueryResults.clear();
			quadtree->query(range, qtMouseQueryResults);
		}
#endif		
		
		if (started)
		{						
			// reset and build the quadtree			
			if (quadtree)
			{
				quadtree->clear();
				for (auto& p : particles)
					quadtree->insert(SpatialPoint<Particle>{ p.position.x, p.position.y, p });
			}
			
			// update particles
			//for (auto& p : particles)
			//{
			//	// get particles from quadtree	
			//	if (quadtree)
			//	{
			//		queryResults.clear();
			//		quadtree->query({
			//			p.position.x - fovRadius, p.position.y - fovRadius,
			//			p.radius + p.radius + fovRadius, p.radius + p.radius + fovRadius },
			//			queryResults);
			//	}
			//	p.update(queryResults);				
			//}		
			
			
			for(auto& range : ranges)
				threadpool.doJob(std::bind(updateParticleBatch<std::span<Particle>>, range));			
		}
		
		
		//
		// Draw
		//
		BeginDrawing();
		ClearBackground(BLACK);

		
		for (auto& p : particles)
		{
			p.render();		
		}		
		

#ifdef RENDER_QUADTREE
		if (quadtree) quadtree->render();
#endif

#ifdef RENDER_QUADTREE_MOUSE_REGION
		if (quadtree)
		{
			// draw mouse range
			DrawRectangleLines(
				(int)range.x, (int)range.y,
				(int)range.w, (int)range.h, BLUE);

			// draw quadtree results
			if (qtMouseQueryResults.size() > 0)
				for (const auto& p : qtMouseQueryResults)
					DrawRectangle(
						(int)(p.position.x - p.radius), (int)(p.position.y - p.radius),
						(int)(p.radius * 2.f), (int)(p.radius * 2.f), BLUE);
		}
#endif

		DrawFPS(10, 10);

		EndDrawing();
	}

	//
	// De-Initialization
	//	
	CloseWindow();        // Close window and OpenGL context	

	return 0;
}
