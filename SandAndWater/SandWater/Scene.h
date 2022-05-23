#pragma once
#include <algorithm>

typedef unsigned short int ushort;

class BaseScene;

enum class Element
{
	OUTOFBOUNDS = -1,
	EMPTY = 0,
	SAND = 1,
	STONE = 2,
	WATER = 3
};

enum class Tool
{
	ERASE,
	SAND,
	STONE,
	WATER
};

class Scene : public BaseScene
{
private:
	Element* _world{ nullptr };
	int _cellDim = 6;
	int _waterDispersalRate = 2;
	int _worldWidth;
	int _worldHeight;

	sf::Color _sandColors[6]
	{
		sf::Color{171, 117, 69}, sf::Color{179, 125, 77 }, sf::Color{185, 131, 83},
		sf::Color{194, 140, 92}, sf::Color{204, 150, 102}, sf::Color{212, 158, 110}
	};

	sf::Color _stoneColors[3]
	{
		sf::Color{152,152,156}, sf::Color{168,169,173}, sf::Color{99,102,106}
	};

	sf::Color _waterColors[3]
	{
		sf::Color{0,128,255}, sf::Color{0,102,204}, sf::Color{51,153,255}
	};

	sf::RenderTexture _texture;
	sf::Sprite _sprite;
	sf::VertexArray _gfxPixels;

	float _timeSinceLastUpdate{ 0.0f };
	float _updateSpeed{ 0.0025f };

	Tool _tool;
	RandomGenerator _rand;

public:
	Scene(int width, int height, std::string title)
		:BaseScene{ width, height, title }
	{
		_worldWidth = width / _cellDim;
		_worldHeight = height / _cellDim;
		_world = new Element[_worldWidth * _worldHeight];
		clearWorld();

		_tool = Tool::SAND;

		std::cout << "World Size: " << _worldWidth << " x " << _worldHeight << std::endl;
		std::cout << "Pixel Size: " << _cellDim << std::endl;
		std::cout << "Num. Pixels: " << _worldWidth * _worldHeight << std::endl << std::endl;

		if (!_texture.create(width, height))
			std::cout << "Failed to create _texture!" << std::endl;

		_gfxPixels.setPrimitiveType(sf::Quads);
	}

	~Scene()
	{
		delete[] _world;
	}

	void updateScene(float elapsed) override
	{
		//_timeSinceLastUpdate += elapsed;
		//if (_timeSinceLastUpdate < _updateSpeed) return;

		updateWorld(elapsed);

		//_timeSinceLastUpdate = 0.0f;
	}

	void drawScene() override
	{
		_texture.clear();

		drawWorld();

		_texture.draw(_gfxPixels);

		_texture.display();

		_sprite.setTexture(_texture.getTexture());
		window.draw(_sprite);
	}

	void spawnElement(const float& screenX, const float& screenY)
	{
		size_t x = screenX / _cellDim;
		size_t y = screenY / _cellDim;


		if (_tool == Tool::ERASE)
		{
			for (size_t i = 0; i < 3; i++)
			{
				for (size_t j = 0; j < 3; j++)
				{
					size_t sx = x + j;
					size_t sy = y + i;

					setElementAt(sx, sy, Element::EMPTY);
				}
			}
		}
		else if (_tool == Tool::SAND)
		{
			for (size_t j = 0; j < 7; j++)
			{
				size_t sx = x + (size_t)(_rand.getUniform() * 10) - 5;
				size_t sy = y + (size_t)(_rand.getUniform() * 10) - 5;

				if (getElementAt(sx, sy) != Element::EMPTY) continue;

				setElementAt(sx, sy, Element::SAND);
			}
		}
		else if (_tool == Tool::STONE)
		{
			for (size_t i = 0; i < 3; i++)
			{
				for (size_t j = 0; j < 3; j++)
				{
					size_t sx = x + j;
					size_t sy = y + i;

					if (getElementAt(sx, sy) != Element::EMPTY) continue;

					setElementAt(sx, sy, Element::STONE);
				}
			}
		}
		else if (_tool == Tool::WATER)
		{
			for (size_t j = 0; j < 7; j++)
			{
				size_t sx = x + (size_t)(_rand.getUniform() * 10) - 5;
				size_t sy = y + (size_t)(_rand.getUniform() * 10) - 5;

				if (getElementAt(sx, sy) != Element::EMPTY) continue;

				setElementAt(sx, sy, Element::WATER);
			}
		}
	}

	void updateWorld(float elapsedTime)
	{
		// process bottom to top to avoid artifacts
		for (int y = _worldHeight - 1; y >= 0; y--)
		{
			if (y % 2 == 0)
			{
				for (int x = 0; x < _worldWidth; x++)
				{
					updateElements(x, y);
				}
			}
			else
			{
				for (int x = _worldWidth; x >= 0; x--)
				{
					updateElements(x, y);
				}
			}
		}
	}

	void updateElements(const size_t& x, const size_t& y)
	{
		Element e = getElementAt(x, y);
		switch (e)
		{
		case Element::STONE:
			// do nothing
			break;

		case Element::SAND:
		{
			Element eUnder = getElementAt(x, y + 1);

			// move down
			if (eUnder == Element::EMPTY || eUnder == Element::WATER)
			{
				setElementAt(x, y, eUnder);
				setElementAt(x, y + 1, Element::SAND);
			}
			// else diag left/diag right
			else
			{
				int dir = std::rand() % 2 == 0;
				Element eDownLeft = getElementAt(x - 1, y + 1);
				Element eDownRight = getElementAt(x + 1, y + 1);

				// move to the left
				if (eDownLeft == Element::EMPTY || eDownLeft == Element::WATER)
				{
					setElementAt(x, y, eDownLeft);
					setElementAt(x - 1, y + 1, Element::SAND);
				}
				// move to the right
				else if (eDownRight == Element::EMPTY || eDownRight == Element::WATER)
				{
					setElementAt(x, y, eDownRight);
					setElementAt(x + 1, y + 1, Element::SAND);
				}
			}
		}
		break;
		case Element::WATER:
		{
			int dir = _rand.getUniform() > 0.5 ? 1 : 0;
			Element eDownLeft = getElementAt(x - 1, y + 1);
			Element eDownRight = getElementAt(x + 1, y + 1);
			Element eLeft = getElementAt(x - 1, y);
			Element eRight = getElementAt(x + 1, y);

			// move down
			if (getElementAt(x, y + 1) == Element::EMPTY)
			{
				setElementAt(x, y, Element::EMPTY);
				setElementAt(x, y + 1, Element::WATER);
			}
			// move down left
			else if (eDownLeft == Element::EMPTY)
			{
				setElementAt(x, y, Element::EMPTY);
				setElementAt(x - 1, y + 1, Element::WATER);
			}
			// move down right
			else if (eDownRight == Element::EMPTY)
			{
				setElementAt(x, y, Element::EMPTY);
				setElementAt(x + 1, y + 1, Element::WATER);
			}
			// move left
			else if(eLeft == Element::EMPTY)
			{
				setElementAt(x, y, Element::EMPTY);
				setElementAt(x - 1, y, Element::WATER);
			}
			// move right
			else if (eRight == Element::EMPTY)
			{
				setElementAt(x, y, Element::EMPTY);
				setElementAt(x + 1, y, Element::WATER);
			}

		}
		break;
		default:
		{

		}
		break;
		}
	}

	void setElementAt(const sf::Vector2<size_t>& pos, const Element element)
	{
		setElementAt(pos.x, pos.y, element);
	}

	void setElementAt(const size_t& x, const size_t& y, const Element element)
	{
		if (!isInWorldBounds(x, y)) return;

		_world[x + _worldWidth * y] = element;
	}

	Element getElementAt(const sf::Vector2<size_t>& pos) const
	{
		return getElementAt(pos.x, pos.y);
	}

	Element getElementAt(const size_t& x, const size_t& y) const
	{
		if (!isInWorldBounds(x, y))
			return Element::OUTOFBOUNDS;

		return _world[x + _worldWidth * y];
	}

	bool isInWorldBounds(const sf::Vector2<size_t>& pos) const
	{
		return isInWorldBounds(pos.x, pos.y);
	}

	bool isInWorldBounds(const size_t& x, const size_t& y) const
	{
		return !(x < 0 || x > _worldWidth - 1 || y < 0 || y > _worldHeight - 1);
	}

	void drawWorld()
	{
		_gfxPixels.clear();

		for (size_t y = 0; y < _worldHeight; y++)
		{
			for (size_t x = 0; x < _worldWidth; x++)
			{
				size_t index = x + _worldWidth * y;
				Element e = _world[index];

				float xl = x * _cellDim;
				float xr = (x + 1) * _cellDim;
				float yt = y * _cellDim;
				float yb = (y + 1) * _cellDim;

				sf::Vertex v1{ sf::Vector2f{xl, yt} };
				sf::Vertex v2{ sf::Vector2f{xr, yt} };
				sf::Vertex v3{ sf::Vector2f{xr, yb} };
				sf::Vertex v4{ sf::Vector2f{xl, yb} };

				switch (e)
				{
				case Element::SAND:
				{
					size_t colorIndex = (size_t)(_rand.getUniform(0, 5));
					v1.color = v2.color = v3.color = v4.color = _sandColors[colorIndex];
				}
				break;

				case Element::STONE:
				{
					size_t colorIndex = (size_t)(_rand.getUniform(0, 3));
					v1.color = v2.color = v3.color = v4.color = _stoneColors[colorIndex];
				}
				break;

				case Element::WATER:
				{
					size_t colorIndex = (size_t)(_rand.getUniform(0, 3));
					v1.color = v2.color = v3.color = v4.color = _waterColors[colorIndex];
				}
				break;

				case Element::EMPTY:
					v1.color = v2.color = v3.color = v4.color = sf::Color{ 53, 54, 58 };
					break;
				}

				_gfxPixels.append(v1); _gfxPixels.append(v2); _gfxPixels.append(v3); _gfxPixels.append(v4);
			}
		}
	}

	void clearWorld()
	{
		for (size_t i = 0; i < _worldWidth * _worldHeight; i++)
		{
			_world[i] = Element::EMPTY;
		}
	}

	void cycleTools()
	{
		if (_tool == Tool::ERASE) setToolSand();
		else if (_tool == Tool::SAND) setToolStone();
		else if (_tool == Tool::STONE) setToolWater();
		else if (_tool == Tool::WATER) setToolErase();
	}

	void setToolErase()
	{
		_tool = Tool::ERASE;
		std::cout << "Tool \"ERASER\" selected" << std::endl;
	}

	void setToolSand()
	{
		_tool = Tool::SAND;
		std::cout << "Tool \"SAND\" selected" << std::endl;
	}

	void setToolStone()
	{
		_tool = Tool::STONE;
		std::cout << "Tool \"STONE\" selected" << std::endl;
	}

	void setToolWater()
	{
		_tool = Tool::WATER;
		std::cout << "Tool \"WATER\" selected" << std::endl;
	}
};