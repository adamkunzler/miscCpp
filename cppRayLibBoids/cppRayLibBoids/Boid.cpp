#include "Boid.h"

#include "SteeringBehaviors.h"

// -----------------------------------------------------------------------------------------------------------

uint32_t Boid::_nextId{ 1 };

uint32_t Boid::getId()
{
	return _nextId++;
}

// -----------------------------------------------------------------------------------------------------------

Boid::Boid(SteeringBehaviors* steeringBehaviors)
	: _steeringBehaviors{ steeringBehaviors }
{
	id = Boid::getId();		
}

Boid::~Boid()
{	
}

// -----------------------------------------------------------------------------------------------------------

void Boid::update(const std::vector<Boid>& boids)
{
	auto neighbors = getNeighbors(boids);
	auto steeringForce = _steeringBehaviors->calculateSteeringForce(*this, neighbors);
	auto acceleration = steeringForce / mass;
	
	velocity += acceleration;
	truncate(velocity, maxSpeed);
	//velocity = glm::normalize(velocity) * maxSpeed; // force continuous movement...ugh?

	auto vl = glm::length(velocity);	
	if ((vl * vl) > 0.00000001f)
	{
		heading = glm::normalize(velocity);
		right = glm::vec2{ -heading.y, heading.x };
	}
	else
	{
		auto t = 5;
	}

	position += velocity;

	//_steeringBehaviors->enforceNonPenetrationConstraint(*this, neighbors);

	// check bounds
	if (position.x - radius > GetScreenWidth()) position.x = -radius;
	if (position.y - radius > GetScreenHeight()) position.y = -radius;
	if (position.x + radius < 0) position.x = GetScreenWidth() + radius;
	if (position.y + radius < 0) position.y = GetScreenHeight() + radius;
}

// -----------------------------------------------------------------------------------------------------------

std::vector<Boid> Boid::getNeighbors(const std::vector<Boid>& boids)
{
	std::vector<Boid> neighbors;

	for (const auto& boid : boids)
	{
		if (boid.id == id) continue;

		if (glm::distance(position, boid.position) <= fovDistance)
			neighbors.emplace_back(boid);
	}

	return neighbors;
}

// -----------------------------------------------------------------------------------------------------------

void Boid::truncate(glm::vec2& vec, float maxLength)
{
	if (glm::length(vec) <= maxLength) return;

	vec = glm::normalize(vec);
	vec *= maxLength;
}

// -----------------------------------------------------------------------------------------------------------

void Boid::render()
{
	auto temp = position - (heading * radius * 0.25f);
	auto v1 = position + (heading * radius);
	auto v2 = temp + (right * radius * 0.5f);
	auto v3 = temp + (-right * radius * 0.5f);

	DrawTriangleLines(Vector2{ v1.x, v1.y }, Vector2{ v2.x, v2.y }, Vector2{ v3.x, v3.y }, RED);

	if (id == 1)
	{
		// heading
		auto h = heading * (radius * 1.5f);
		h += position;
		DrawLine(position.x, position.y, h.x, h.y, BLUE);

		// side
		auto s = right * radius;
		s += position;
		DrawLine(position.x, position.y, s.x, s.y, GREEN);

		// fov
		DrawCircleLines(position.x, position.y, fovDistance, LIGHTGRAY);
	}

	//DrawText(std::to_string(id).c_str(), position.x, position.y - 35, 20, GRAY);
}

// -----------------------------------------------------------------------------------------------------------
