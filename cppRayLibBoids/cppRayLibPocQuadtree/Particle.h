#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

#include <glm/glm.hpp>
#include "raylib.h"
#include "rlgl.h"

#include "utils.h"

struct Particle
{	
	Texture texture;
	Color color{ LIGHTGRAY };
	float radius{ 0.f };
	glm::vec2 position{ 0.f };
	glm::vec2 velocity{ 0.f };
	uint32_t id;

	Particle();

	void update(const std::vector<Particle>& particles);
	void update();
	void render();		
	
	static uint32_t getId();

private:
	static uint32_t _nextId;
};

#endif