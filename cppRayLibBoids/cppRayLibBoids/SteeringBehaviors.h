#ifndef STEERINGBEHAVIORS_H
#define STEERINGBEHAVIORS_H

#include <vector>
#include <glm/glm.hpp>

class Boid;

class SteeringBehaviors
{
public:
	SteeringBehaviors();
	~SteeringBehaviors();
	SteeringBehaviors(const SteeringBehaviors&) = default;
	SteeringBehaviors& operator=(const SteeringBehaviors&) = default;
	SteeringBehaviors(SteeringBehaviors&&) = default;
	SteeringBehaviors& operator=(SteeringBehaviors&&) = default;

	glm::vec2 calculateSteeringForce(const Boid& boid, const std::vector<Boid>& neighbors);

	void enforceNonPenetrationConstraint(Boid& boid, const std::vector<Boid>& neighbors);

	glm::vec2 seek(const Boid& boid, const glm::vec2& target);

	glm::vec2 cohesion(const Boid& boid, const std::vector<Boid>& neighbors);

	glm::vec2 separation(const Boid& boid, const std::vector<Boid>& neighbors);

	glm::vec2 alignment(const Boid& boid, const std::vector<Boid>& neighbors);

private:
	bool accumulateForce(glm::vec2& runningTotal, const glm::vec2 forceToAdd, const float maxForce);

public:
	static float separationWeight;
	static float cohesionWeight;
	static float alignmentWeight;

	bool isSeek{ false };
	bool isCohesion{ false };
	bool isSeparation{ false };
	bool isAlignment{ false };
};



#endif