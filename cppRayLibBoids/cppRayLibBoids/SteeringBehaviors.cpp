#include "SteeringBehaviors.h"

#include "Boid.h"

// -----------------------------------------------------------------------------------------------------------

float SteeringBehaviors::separationWeight{ 15.f };
float SteeringBehaviors::cohesionWeight{ 1.f };
float SteeringBehaviors::alignmentWeight{ 1.f };

// -----------------------------------------------------------------------------------------------------------

SteeringBehaviors::SteeringBehaviors()
{
}

SteeringBehaviors::~SteeringBehaviors()
{
}

// -----------------------------------------------------------------------------------------------------------

glm::vec2 SteeringBehaviors::calculateSteeringForce(const Boid& boid, const std::vector<Boid>& neighbors)
{	
	glm::vec2 steeringForce{ 0.f };

	if (neighbors.size() == 0) return steeringForce;	

	if (isSeparation)
	{
		auto force1 = separation(boid, neighbors) * separationWeight;
		//auto m1 = glm::length(force1);
		//steeringForce += force1;		
		if (!accumulateForce(steeringForce, force1, boid.maxForce)) return steeringForce;
	}

	if (isCohesion)
	{
		auto force2 = cohesion(boid, neighbors) * cohesionWeight;
		//auto m2 = glm::length(force2);
		//steeringForce += force2;		
		if (!accumulateForce(steeringForce, force2, boid.maxForce)) return steeringForce;
	}

	if (isAlignment)
	{		
		auto force3 = alignment(boid, neighbors) * alignmentWeight;
		//auto m3 = glm::length(force3);
		//steeringForce += force3;		
		if (!accumulateForce(steeringForce, force3, boid.maxForce)) return steeringForce;
	}

	return steeringForce;
}

// -----------------------------------------------------------------------------------------------------------

// doesn't work yet
void SteeringBehaviors::enforceNonPenetrationConstraint(Boid& boid, const std::vector<Boid>& neighbors)
{
	if (neighbors.size() == 0) return;

	for (const auto& n : neighbors)
	{
		glm::vec2 toNeighbor = n.position - boid.position;
		auto dist = glm::length(toNeighbor);
		auto amountOfOverlap = n.radius + boid.radius - dist;
		if (amountOfOverlap >= 0)
		{			
			boid.position += ((toNeighbor / dist) * amountOfOverlap);
		}
	}
}

// -----------------------------------------------------------------------------------------------------------

bool SteeringBehaviors::accumulateForce(glm::vec2& runningTotal, const glm::vec2 forceToAdd, const float maxForce)
{
	auto magnitudeSoFar = glm::length(runningTotal);
	auto magnitudeRemaining = maxForce - magnitudeSoFar;

	// no more force left to use
	if (magnitudeRemaining <= 0.f) return false;

	// add the whole force if room, or fill up remaining space
	auto magnitudeToAdd = glm::length(forceToAdd);
	if (magnitudeToAdd < magnitudeRemaining)
	{
		runningTotal += forceToAdd;
	}
	else
	{
		runningTotal += (glm::normalize(forceToAdd) * magnitudeRemaining);
	}

	return true;
}

// -----------------------------------------------------------------------------------------------------------

glm::vec2 SteeringBehaviors::seek(const Boid& boid, const glm::vec2& target)
{
	auto force = glm::normalize(target - boid.position) * boid.maxSpeed;
	return force - boid.velocity;
}

// -----------------------------------------------------------------------------------------------------------

glm::vec2 SteeringBehaviors::cohesion(const Boid& boid, const std::vector<Boid>& neighbors)
{
	glm::vec2 centerOfMass{ 0.0f };
	if (neighbors.size() == 0) return centerOfMass;

	for (const auto& n : neighbors)
	{
		centerOfMass += n.position;
	}

	centerOfMass /= (float)neighbors.size();
	auto steeringForce = seek(boid, centerOfMass);
	return glm::normalize(steeringForce);
	//return steeringForce;
}

// -----------------------------------------------------------------------------------------------------------

glm::vec2 SteeringBehaviors::separation(const Boid& boid, const std::vector<Boid>& neighbors)
{
	glm::vec2 steeringForce{ 0.0f };
	if (neighbors.size() == 0) return steeringForce;

	for (const auto& n : neighbors)
	{
		auto direction = n.position - boid.position;
		auto distance = glm::length(direction);						
		auto strength = glm::min(-boid.radius / (distance * distance), boid.maxForce);
		steeringForce += glm::normalize(direction) * strength;		
	}

	return steeringForce;
}

// -----------------------------------------------------------------------------------------------------------

glm::vec2 SteeringBehaviors::alignment(const Boid& boid, const std::vector<Boid>& neighbors)
{
	glm::vec2 steeringForce{ 0.0f };
	if (neighbors.size() == 0) return steeringForce;

	for (const auto& n : neighbors)
	{
		steeringForce += n.heading;
	}

	steeringForce /= (float)neighbors.size(); // average heading
	steeringForce -= boid.heading; // actual steering force
	return steeringForce;
}

// -----------------------------------------------------------------------------------------------------------
