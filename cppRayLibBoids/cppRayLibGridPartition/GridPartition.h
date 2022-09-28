#ifndef GRIDPARTITION_H
#define GRIDPARTITION_H

#include <vector>

#include <glm/glm.hpp>

#include "SimEntity.h"
#include "QueryResult.h"

struct Cell
{
	size_t x{ 0 };
	size_t y{ 0 };
	Particle* particle{ nullptr };
};

class GridPartition
{
public:
	GridPartition()		
	{
	}

	void init(float cellSize)
	{
		_cellSize = cellSize;
		_width = (size_t)(WORLD_WIDTH / _cellSize);
		_height = (size_t)(WORLD_HEIGHT / _cellSize);

		// create the cells
		_cells.reserve(_width * _height);
		for (size_t y = 0; y < _height; ++y)
		{
			for (size_t x = 0; x < _width; ++x)
			{
				Cell cell{ x, y };
				_cells.emplace_back(cell);
			}
		}		
	}
	
	void add(Particle* p)
	{
		auto index = getCellIndex(p->position.x, p->position.y);	
		auto temp = _cells.at(index).particle;
		if (p == temp) 
			return;
				
		// add particle to the head of the cell
		p->prev = nullptr;				// null prev because new head
		p->next = temp;					// previous head now next
		_cells.at(index).particle = p;	// cell points to head

		// link existing nodes
		if (p->next != nullptr) p->next->prev = p; // next was previous head, point it back to new head
	}

	void move(Particle* p)
	{		
		if (p == nullptr) return;

		auto oldCell = getCell(p->oldPosition.x, p->oldPosition.y);
		auto newCell = getCell(p->position.x, p->position.y);
		
		if (oldCell == newCell) return;
				
		// unlink it from the list
		if (p->prev != nullptr)
			p->prev->next = p->next;			
		

		if (p->next != nullptr)
			p->next->prev = p->prev;					

		// if it's the head of the old cell...remove it
		auto oldCellIndex = getCellIndex(p->oldPosition.x, p->oldPosition.y);
		if (_cells.at(oldCellIndex).particle == p)
			_cells.at(oldCellIndex).particle = p->next;					

		add(p);
	}

	void query(glm::vec2& position, std::vector<Particle*>& results)
	{
		auto cell = getCell(position.x, position.y);
		
		//
		// results will hold the heads of the linked lists
		//
		results.emplace_back(queryParticleAt(cell.x, cell.y)); // center;
		results.emplace_back(queryParticleAt(cell.x, cell.y - 1)); // top middle
		results.emplace_back(queryParticleAt(cell.x - 1, cell.y - 1)); // top left;
		results.emplace_back(queryParticleAt(cell.x - 1, cell.y)); // middle left;
		results.emplace_back(queryParticleAt(cell.x - 1, cell.y + 1)); // bottom left;

		//
		// partition handles following linked list to build results
		//
		//aggregateQueryResults(queryParticleAt(cell.x,     cell.y), results); // center

		//aggregateQueryResults(queryParticleAt(cell.x, cell.y - 1), results); // top middle

		//aggregateQueryResults(queryParticleAt(cell.x - 1, cell.y - 1), results); // top left
		//aggregateQueryResults(queryParticleAt(cell.x - 1, cell.y), results); // center left
		//aggregateQueryResults(queryParticleAt(cell.x - 1, cell.y + 1), results); // bottom left

		//
		// for collisions...above is enough and will prevent duplicate collision detections
		// for boids where each boid needs to know all it neighbors...uncomment the below
		// 
		//aggregateQueryResults(queryParticleAt(cell.x + 1, cell.y), results); // center right					
		//aggregateQueryResults(queryParticleAt(cell.x + 1, cell.y - 1), results); // top right		
		//aggregateQueryResults(queryParticleAt(cell.x, cell.y + 1), results); // bottom middle
		//aggregateQueryResults(queryParticleAt(cell.x + 1, cell.y + 1), results); // bottom right								
	}
	
	void render()
	{
		int iCellSize = (int)_cellSize;
		for (size_t y = 0; y < _height; ++y)
		{
			for (size_t x = 0; x < _width; ++x)
			{				
				auto index = x + y * _width;
				int sx = (int)(x * iCellSize);
				int sy = (int)(y * iCellSize);
				DrawRectangleLines(sx, sy, iCellSize, iCellSize, Fade(RAYWHITE, 0.3f));				
			}
		}
	}

	inline size_t getCellIndex(float x, float y)
	{
		auto cell = getCell(x, y);
		return cell.x + cell.y * _width;
	}

	inline glm::ivec2 getCell(float x, float y)
	{
		int cx = (int)(x / _cellSize);
		int cy = (int)(y / _cellSize);

		// index bounds checking
		if (cx < 0) cx = 0;
		if (cy < 0) cy = 0;
		if (cx >= (int)_width) cx = (int)_width - 1;
		if (cy >= (int)_height) cy = (int)_height - 1;

		return glm::ivec2{ cx, cy };
	}

private:
	void aggregateQueryResults(Particle* p, std::vector<Particle*>& results)
	{
		Particle* temp = p;
		while (temp != nullptr)
		{
			results.emplace_back(temp);
			temp = temp->next;
		}
	}

	Particle* queryParticleAt(int cellX, int cellY)
	{		
		if (cellX < 0) cellX = (int)_width - 1;
		if (cellY < 0) cellY = (int)_height - 1;
		if (cellX >= (int)_width) cellX = 0;
		if (cellY >= (int)_height) cellY = 0;

		auto index = cellX + cellY * _width;
		return _cells.at(index).particle;
	}

private:
	float _cellSize{ 0.f };
	size_t _width{ 0 }; 
	size_t _height{ 0 };
	std::vector<Cell> _cells;
};

#endif