#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Hexagon
{
public:
	Hexagon()
		: _center{ 100.f, 100.f }, _sideLength{ 10.0 }, _coord{ 0, 0 }
	{
	}

	Hexagon(glm::vec2 center, double sideLength, glm::vec2 coord)
		: _center{ center }, _sideLength{ sideLength }, _coord{ coord }
	{
	}

	glm::vec2 getCenter() const
	{
		return _center;
	}

	double getWidth() const
	{
		return 2.0 * _sideLength;
	}

	double getHeight() const
	{
		return _sideLength * glm::sqrt(3.0);
	}

	void render()
	{
		auto rotation = 0.f; // point side up
		//auto rotation = 90.f; // flat side up
		for (auto i = 0; i < 6; ++i)
		{
			//DrawPoly(Vector2{ _center.x, _center.y }, 6, _sideLength, rotation, GREEN);
			DrawPolyLines(Vector2{ _center.x, _center.y }, 6, _sideLength, rotation, GREEN);
		}
	}

private:


private:
	glm::vec2 _center{ 0, 0 };
	double _sideLength{ 1.f };
	glm::vec2 _coord{ 0, 0 };
};
