#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "raylib.h"

#include "Globals.h"

class Particle
{
private:
	uint32_t _id;

public:
	glm::vec2 position{ 0 };
	glm::vec2 oldPosition{ 0 };
	glm::vec2 velocity{ 0 };
	glm::vec2 heading{ 0 };
	float radius{ 0.f };	

	std::vector<Particle*> neighbors;

	Particle* next;
	Particle* prev;

	Particle();

	void update();

	void render(Texture& texture);

	uint32_t id();

private:
	static uint32_t getNextId();
	static uint32_t _nextId;	
};

#endif