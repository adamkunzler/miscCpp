#ifndef SPATIALPARTITION_H
#define SPATIALPARTITION_H

#include <vector>

bool isIntersects(float x, float y, float w, float h, float xx, float yy, float ww, float hh);
bool isRectContainsPoint(float x, float y, float w, float h, float px, float py);

template<typename T>
struct SpatialPoint
{
	float x;
	float y;
	T data;
};

template<typename T>
struct SpatialRect
{
	float x; float y; // center of rect		
	float w; float h; // full width/height of rect	
};

template<typename T>
class ISpatialPartition
{	
public:
	virtual ~ISpatialPartition() {}
	
	virtual void render() {}
	
	virtual void clear() {}
	
	virtual bool insert(const SpatialPoint<T>& point) { return false; }
	
	virtual void query(const SpatialRect<T>& rect, std::vector<T>& result) {}
};

#endif