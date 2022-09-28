#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include <vector>
#include <span>

#include "Globals.h"
#include "Particle.h"
#include "GridPartition.h"
#include "QueryResult.h"
#include "ThreadPool.h"

template<typename Span>
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

class ParticleManager
{
public:

	ParticleManager();
	
	void update();

	void render();


private:
	void initParticles(size_t numParticles);

	void initParticleUpdateChunks();

	void updateMultiThread();
	
public:
	bool isRenderPartition{ false };

private:
	Texture _particleTexture;

	ThreadPool _threadPool{ MAX_THREADS };
	
	std::vector<std::span<Particle>> _ranges;
	
	GridPartition _partition;

	std::vector<Particle> _particles;
};

#endif

