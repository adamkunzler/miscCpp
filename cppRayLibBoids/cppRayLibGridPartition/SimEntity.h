#ifndef SIMENTITY_H
#define SIMENTITY_H

#include <cstdint>

class SimEntity
{
public:
	SimEntity();

	uint32_t id();
	
private:
	uint32_t _id;

	static uint32_t getNextId();
	static uint32_t _nextId;
};


#endif