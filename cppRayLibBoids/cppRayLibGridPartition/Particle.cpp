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
}

void Particle::render(Texture& texture)
{
	// HEADING
	/*auto temp = (heading * radius);
	DrawLine(
		(int)position.x, (int)position.y,
		(int)(position.x + temp.x), (int)(position.y + temp.y),
		BLUE);*/

	DrawTexture(texture, (int)(position.x - radius), (int)(position.y - radius), WHITE);

	// ID
	//auto text = std::to_string(_id);
	//DrawText(text.c_str(), position.x, position.y - radius - radius, 20, WHITE);
}

