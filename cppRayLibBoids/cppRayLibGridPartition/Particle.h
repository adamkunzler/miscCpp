#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "raylib.h"

#include "Globals.h"
#include "SimEntity.h"

class Particle : public SimEntity
{
public:
	glm::vec2 position{ 0 };
	glm::vec2 oldPosition{ 0 };
	glm::vec2 velocity{ 0 };
	glm::vec2 heading{ 0 };
	float radius{ 0.f };	
	float mass{ 1.f };
	
	std::vector<Particle*> neighbors;

	Particle* next;
	Particle* prev;

	Particle();

	void update();

	void checkColllision(Particle& p);

	void render(Texture& texture);
	
};

#endif