#include "ParticleManager.h"

ParticleManager::ParticleManager()
{
	initParticles(MAX_PARTICLES);

	// create and initialize the partition
	_partition.init(CELL_SIZE); // should be half of flocking radius | should be evenly divisble by screen dims
	for (auto& p : _particles) _partition.add(&p);

	initParticleUpdateChunks();

	// create a texture to be shared by particles
	auto temp = LoadImage("moon.png");
	ImageResize(&temp, (int)(DEFAULT_RADIUS * 2.f), (int)(DEFAULT_RADIUS * 2.f));
	_particleTexture = LoadTextureFromImage(temp);
	UnloadImage(temp);
}


void ParticleManager::update()
{
	updateMultiThread();
	
	// refresh partition
	for (auto& p : _particles)
		_partition.move(&p);	
}

void ParticleManager::render()
{
	

	if (isRenderPartition)
		_partition.render();

	for (auto& p : _particles)
		p.render(_particleTexture);	
}

void ParticleManager::initParticles(size_t numParticles)
{
	_particles.clear();
	_particles.reserve(numParticles);

	for (size_t i = 0; i < numParticles; ++i)
	{
		Particle p;
		p.position = glm::vec2{
			(float)GetRandomValue((int)DEFAULT_RADIUS, (int)(WORLD_WIDTH - DEFAULT_RADIUS)),
			(float)GetRandomValue((int)DEFAULT_RADIUS, (int)(WORLD_HEIGHT - DEFAULT_RADIUS)) };
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

void ParticleManager::initParticleUpdateChunks()
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

void ParticleManager::updateMultiThread()
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
