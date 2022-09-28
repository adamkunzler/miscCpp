#ifndef BOID_H
#define BOID_H

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>
#include "raylib.h"

class SteeringBehaviors;

class Boid
{
public:	
	Boid(SteeringBehaviors* steeringBehaviors);
	~Boid();
	Boid(const Boid&) = default;
	Boid& operator=(const Boid&) = default;
	Boid(Boid&&) = default;
	Boid& operator=(Boid&&) = default;
	
	static uint32_t getId();

	void update(const std::vector<Boid>& boids);

	void render();

private:	
	std::vector<Boid> getNeighbors(const std::vector<Boid>& boids);

	void truncate(glm::vec2& vec, float maxLength);

public:	
	uint32_t id;
	
	float radius{ 0.f };
	float mass{ 1.f };
	
	glm::vec2 position{ 0.f };
	glm::vec2 velocity{ 0.f };
			
	glm::vec2 heading{ 0.f };
	glm::vec2 right{ 0.f };

	float fovDistance{ 0.f };
	
	float maxForce{ 0.f };
	float maxSpeed{ 0.f };	

private:
	static uint32_t _nextId;

	SteeringBehaviors* _steeringBehaviors;
};




#endif