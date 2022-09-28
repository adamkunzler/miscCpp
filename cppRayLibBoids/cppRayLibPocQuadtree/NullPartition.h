#ifndef NULLPARTITION_H
#define NULLPARTITION_H

#include <vector>

#include "ISpatialPartition.h"

template<typename T>
class NullPartition : public ISpatialPartition<T>
{
public:
	NullPartition();

	void render();

	void clear();

	bool insert(const SpatialPoint<T>& point);
	void update(std::vector<T>& points);

	void query(const SpatialRect<T>& rect, std::vector<T>& result);

private:
	std::vector<T> _points;
};

#endif

template<typename T>
NullPartition<T>::NullPartition()	
{	
}

template<typename T>
bool NullPartition<T>::insert(const SpatialPoint<T>& point)
{
	_points.emplace_back(point.data);
	return true;
}

template<typename T>
void NullPartition<T>::clear()
{	
	_points.clear();
}

template<typename T>
void NullPartition<T>::render()
{}

template<typename T>
void NullPartition<T>::query(const SpatialRect<T>& rect, std::vector<T>& result)
{	
	result = _points;
}