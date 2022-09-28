#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <thread>
#include <future>

#include "ISpatialPartition.h"
#include "utils.h"

template<typename T>
class Quadtree : public ISpatialPartition<T>
{
public:
	Quadtree(SpatialRect<T> boundary);
	
	void render();

	void clear();

	bool insert(const SpatialPoint<T>& point);	

	void query(const SpatialRect<T>& rect, std::vector<T>& result);

private:
	void subdivide();	

private:
	const size_t CAPACITY{ 10 };
	SpatialRect<T> _boundary{ 0.f, 0.f, 0.f, 0.f };
	std::vector<SpatialPoint<T>> _points;
	
	bool _divided{ false };
	std::unique_ptr<Quadtree<T>> _topRight{ nullptr };
	std::unique_ptr<Quadtree<T>> _bottomRight{ nullptr };
	std::unique_ptr<Quadtree<T>> _bottomLeft{ nullptr };
	std::unique_ptr<Quadtree<T>> _topLeft{ nullptr };

	std::thread _tr;
	std::thread _tl;
	std::thread _br;
	std::thread _bl;
};


// --------------------------------------------------------------------

template<typename T>
Quadtree<T>::Quadtree(SpatialRect<T> boundary)
	: _boundary{ boundary }
{
	_points.reserve(CAPACITY);
}

template<typename T>
bool Quadtree<T>::insert(const SpatialPoint<T>& point)
{
	if (!isRectContainsPoint(
		_boundary.x, _boundary.y, _boundary.w, _boundary.h, 
		point.x, point.y)) return false;
	
	if (_points.size() < CAPACITY)
	{
		_points.emplace_back(point);
		return true;
	}
	else
	{
		if (!_divided)
		{
			subdivide();			
		}

		if (_topRight->insert(point)) return true;
		if(_bottomRight->insert(point)) return true;
		if(_bottomLeft->insert(point)) return true;
		if(_topLeft->insert(point)) return true;		
	}

	return false;
}

template<typename T>
void Quadtree<T>::subdivide()
{	
	_divided = true;

	auto halfW = _boundary.w / 2.f;
	auto halfH = _boundary.h / 2.f;
	
	auto left = _boundary.x;
	auto right = _boundary.x + halfW;
	auto top = _boundary.y;
	auto bottom = _boundary.y + halfH;

	_topRight = std::make_unique<Quadtree<T>>(
		SpatialRect<T>{ 
			right, top, halfW, halfH });
	
	_bottomRight = std::make_unique<Quadtree<T>>(
		SpatialRect<T>{
			right, bottom, halfW, halfH });
	
	_bottomLeft = std::make_unique<Quadtree<T>>(
		SpatialRect<T>{
			left, bottom, halfW, halfH });
	
	_topLeft = std::make_unique<Quadtree<T>>(
		SpatialRect<T>{
			left, top, halfW, halfH });
}

template<typename T>
void Quadtree<T>::clear()
{		
	_divided = false;
	_points.clear();

	if (_divided)
	{
		_topRight->clear();
		_bottomRight->clear();
		_bottomLeft->clear();
		_topLeft->clear();
	}
}

template<typename T>
void Quadtree<T>::render()
{
	DrawRectangleLines(
		_boundary.x,
		_boundary.y,
		_boundary.w, _boundary.h, //LIGHTGRAY);
		Fade(LIGHTGRAY, 0.3f));	

	if (_divided)
	{
		_topRight->render();
		_bottomRight->render();
		_bottomLeft->render();
		_topLeft->render();
	}
}

template<typename T>
void Quadtree<T>::query(const SpatialRect<T>& rect, std::vector<T>& result)
{		
	if (!isIntersects(
		_boundary.x, _boundary.y, _boundary.w, _boundary.h,
		rect.x, rect.y, rect.w, rect.h
	)) return;

	for (const auto& p : _points)
	{
		if (isRectContainsPoint(rect.x, rect.y, rect.w, rect.h, p.x, p.y))
			result.emplace_back(p.data);
	}

	if (_divided)
	{
		_topRight->query(rect, result);
		_bottomRight->query(rect, result);
		_bottomLeft->query(rect, result);
		_topLeft->query(rect, result);
	}
}

#endif