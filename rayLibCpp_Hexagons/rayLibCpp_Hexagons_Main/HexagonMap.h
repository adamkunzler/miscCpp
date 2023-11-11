#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "hexagon.h"

struct HexagonMapConfig
{
	glm::vec2 screenCenter{ 0.f, 0.f }; // center of screen
	glm::vec2 mapDim{ 0.f, 0.f }; // width height of map (in tiles)
	glm::vec2 tileDim{ 0.f, 0.f }; // width wight of tile (in pixels)
	float tileSideLength{ 0.f }; // side length of tile
	glm::vec2 startPos{ 0.f, 0.f }; // where to draw the first tile (top left)
};

class HexagonMap
{
public:

	HexagonMap(HexagonMapConfig config)
		: _config{ config }
	{

	}
	
	void initPointEndUp()
	{
		for (auto y = 0; y < _config.mapDim.y; ++y)
		{
			for (auto x = 0; x < _config.mapDim.x; ++x)
			{
				auto hexCenterX = _config.tileDim.y * x + (y % 2 == 0 ? _config.tileDim.y / 2 : 0);
				auto hexCenterY = _config.tileSideLength * 1.5f * y;

				Hexagon h{
					glm::vec2{ _config.startPos.x + hexCenterX, _config.startPos.y + hexCenterY },
					_config.tileSideLength,
					glm::vec2{ x, y }
				};

				_tiles.push_back(h);
			}
		}
	}

	void initFlatEndUp()
	{
		for (auto y = 0; y < _config.mapDim.y; ++y)
		{
			for (auto x = 0; x < _config.mapDim.x; ++x)
			{
				auto hexCenterX = _config.tileSideLength * 1.5f * y;
				auto hexCenterY = _config.tileDim.y * x + (y % 2 == 0 ? _config.tileDim.y / 2 : 0);

				Hexagon h{
					glm::vec2{ _config.startPos.x + hexCenterX, _config.startPos.y + hexCenterY },
					_config.tileSideLength,
					glm::vec2{ x, y }
				};

				_tiles.push_back(h);
			}
		}
	}

	void render() 
	{
		for (auto h : _tiles)
		{
			h.render();
		}

		
	}

private:
	std::vector<Hexagon> _tiles;
	HexagonMapConfig _config;

};