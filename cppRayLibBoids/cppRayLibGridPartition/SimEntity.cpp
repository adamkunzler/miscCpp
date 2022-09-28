#include "SimEntity.h"

SimEntity::SimEntity()
{
	_id = getNextId();	
}

uint32_t SimEntity::_nextId{ 1 };

uint32_t SimEntity::getNextId()
{
	return _nextId++;
}

uint32_t SimEntity::id()
{
	return _id;
}