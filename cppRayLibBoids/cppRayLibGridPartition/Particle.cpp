#include "Particle.h"

uint32_t Particle::_nextId{ 1 };

uint32_t Particle::getNextId()
{
	return _nextId++;
}

Particle::Particle()
{
	_id = getNextId();
	//std::cout << "Particle ctor => " << _id << std::endl;

	neighbors.reserve(MAX_PARTICLES);

	next = nullptr;
	prev = nullptr;
}

uint32_t Particle::id()
{
	return _id;
}

void Particle::update()
{
	neighbors.clear();
	
	oldPosition = position;
	position += velocity;
	heading = glm::normalize(velocity);
	angle = glm::atan(heading.y, heading.x);
}

void Particle::checkColllision(Particle& p)
{		
	if (this == &p) return;

	glm::vec2 distVec = p.position - position;
	glm::vec2 distDir = glm::normalize(distVec);
	float dist = glm::length(distVec);
	float totalRadius = radius + p.radius;
	
	float collisionDepth = totalRadius - dist;
	if (collisionDepth <= 0.f) return;

	// push away the less massive one
	if (mass < p.mass) 	
		position -= distDir * collisionDepth;	
	else	
		p.position += distDir * collisionDepth;

	float aci = glm::dot(velocity, distDir);
	float bci = glm::dot(p.velocity, distDir);

	float acf = (aci * (mass - p.mass) + 2.f * p.mass * bci) / (mass + p.mass);
	float bcf = (bci * (p.mass - mass) + 2.f * mass * aci) / (mass + p.mass);

	velocity += (acf - aci) * distDir;
	p.velocity += (bcf - bci) * distDir;	
}

void Particle::render(Texture& texture)
{		
	DrawTexture(texture, (int)(position.x - radius), (int)(position.y - radius), WHITE);
	
	// ID
	//auto text = std::to_string(_id);
	//DrawText(text.c_str(), position.x, position.y - radius - radius, 20, WHITE);
}

