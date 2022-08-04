#ifndef Tetrimino_H
#define Tetrimino_H

enum class TetriminoType
{	
	I = 1,
	O = 2,
	T = 3,
	S = 4,
	Z = 5,
	J = 6,
	L = 7
};

class Tetrimino
{
public:
	Tetrimino()
		: _type{ TetriminoType::I }
	{		
		setData();
	}
	

	std::vector<std::vector<short>> getData()
	{
		return _data;
	}

#pragma region Render


	/// <summary>
	/// Generate sf::VertexArray for rendering tetramino
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="padding"></param>
	/// <param name="scale"></param>	
	//sf::Sprite& render(const float& x, const float& y, const float& padding)
	sf::VertexArray render(const float& x, const float& y, const float& padding)
	{
		sf::VertexArray verts;

		auto dim = getDimension();
		auto color = getColor();
		
		for (std::size_t row = 0; row < dim; ++row)
		{
			for (std::size_t col = 0; col < dim; ++col)
			{
				auto p = _data[row][col];
				if (p == 0) continue;
				
				float sx = x + (float)col * WORLD_SCALE;
				float sy = y + (float)row * WORLD_SCALE;

				// top left | top right | bottom right | bottom left
				sf::Vertex v1{ sf::Vector2f{ sx + padding, sy + padding }, color };
				sf::Vertex v2{ sf::Vector2f{ sx + WORLD_SCALE - padding, sy + padding }, color };
				sf::Vertex v3{ sf::Vector2f{ sx + WORLD_SCALE - padding, sy + WORLD_SCALE - padding }, color };
				sf::Vertex v4{ sf::Vector2f{ sx + padding, sy + WORLD_SCALE - padding}, color };

				verts.append(v1); verts.append(v2); verts.append(v3); verts.append(v4);
			}
		}

		return verts;
	}
	
#pragma endregion Render

#pragma region Dimensions

	/// <summary>
	/// Get the dimension of the tetramino (either 3x3 or 4x4)
	/// </summary>
	/// <returns></returns>
	std::size_t getDimension() const
	{
		return getDimension(_type);
	}

	static std::size_t getDimension(const TetriminoType& type) 
	{
		switch (type)
		{
		case TetriminoType::I:
		case TetriminoType::O:
			return 4;
		default: return 3;
		}
	}

#pragma endregion Dimensions

#pragma region Color

	/// <summary>
	/// Get sf::Color for the current Tetrimino
	/// </summary>
	/// <returns></returns>
	sf::Color getColor() const
	{
		return getColor(_type);
	}

	/// <summary>
	/// Get sf::Color for a specified TetriminoType
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	static sf::Color getColor(const TetriminoType& type)
	{
		switch (type)
		{
		case TetriminoType::I:  return sf::Color{ 101, 201, 211 }; // cyan
		case TetriminoType::O:  return sf::Color{ 225, 221, 65 }; // yellow;
		case TetriminoType::T:  return sf::Color{ 135, 88, 159 }; //purple;
		case TetriminoType::S:	return sf::Color{ 91, 181, 79 }; //green;
		case TetriminoType::Z:  return sf::Color{ 227, 61, 54 }; //red;
		case TetriminoType::J:  return sf::Color{ 66, 86, 159 }; //blue;
		case TetriminoType::L:  return sf::Color{ 226, 166, 54 }; //orange;		
		default: return sf::Color{ 31,31,31 }; // dark gray
		}
	}
	
	/// <summary>
	/// Get sf::Color for a specified TetriminoType (as an int)
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	static sf::Color getColor(const ushort& type)
	{		
		return getColor(static_cast<TetriminoType>(type));
	}

#pragma endregion Color

#pragma region TetraminoType Utils

	TetriminoType getType() const
	{
		return _type;
	}

	void setType(const TetriminoType& type)
	{
		_type = type;
		setData();
	}

	/// <summary>
	/// Gets the int value of the TetraminoType enum for specified value
	/// </summary>
	/// <returns></returns>
	int getTypeAsInt() const
	{
		return getTypeAsInt(_type);
	}

	/// <summary>
	/// Gets the int value of the TetraminoType enum for specified value
	/// </summary>
	/// <returns></returns>
	static int getTypeAsInt(const TetriminoType& type)
	{
		return static_cast<int>(type);
	}
		
#pragma endregion TetraminoType Utils

#pragma region Rotation

	/// <summary>
	/// Rotate the Tetrimino clockwise
	/// </summary>
	void rotateCW()
	{		
		auto dim = getDimension();
		std::vector<std::vector<short>> temp{ {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0} };
		
		for (std::size_t row = 0; row < dim; ++row)
			for (std::size_t col = 0; col < dim; ++col)
				temp[row][col] = _data[dim - col - 1][row];

		_data = temp;
	}

	/// <summary>
	/// Rotate the Tetrimino counter-clockwise
	/// </summary>
	void rotateCCW()
	{
		auto dim = getDimension();
		std::vector<std::vector<short>> temp{ {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0} };

		for (std::size_t row = 0; row < dim; ++row)
			for (std::size_t col = 0; col < dim; ++col)
				temp[row][col] = _data[col][dim - row - 1];

		_data = temp;
	}

#pragma endregion Rotation

#pragma region Debug Util Methods
	
	/// <summary>
	/// print current tetramino data to console
	/// </summary>
	void print() const
	{
		auto dim = getDimension();

		std::cout << std::endl;

		for (std::size_t row = 0; row < dim; ++row)
		{
			std::cout << std::endl;
			for (std::size_t col = 0; col < dim; ++col)
			{
				std::cout << _data[row][col] << " ";
			}
		}
	}

	/// <summary>
	/// Make Tetrimino a random type
	/// </summary>
	void random()
	{
		std::uniform_int_distribution<size_t> dis(0, ALL_TYPES.size() - 1);
		_type = ALL_TYPES[dis(gen)];
		setData();
	}
		
#pragma endregion Debug Util Methods
	
private:
	TetriminoType _type{ TetriminoType::I };
	std::vector<std::vector<short>> _data;
	
#pragma region Tetramino Shape Data

	/// <summary>
	/// Set the Tetrimino data based on current type
	/// </summary>	
	void setData()
	{
		_data.clear();

		switch (_type)
		{
		case TetriminoType::I:
			_data = std::vector<std::vector<short>>
			{
				{ 0, 1, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 0, 1, 0, 0 }
			};
			break;
		case TetriminoType::O:
			_data = std::vector<std::vector<short>>
			{
				{ 0, 0, 0, 0 },
				{ 0, 2, 2, 0 },
				{ 0, 2, 2, 0 },
				{ 0, 0, 0, 0 }
			};
			break;
		case TetriminoType::T:
			_data = std::vector<std::vector<short>>
			{
				{ 0, 3, 0, 0 },
				{ 3, 3, 3, 0 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 }
			};
			break;
		case TetriminoType::S:
			_data = std::vector<std::vector<short>>
			{
				{ 0, 4, 4, 0 },
				{ 4, 4, 0, 0 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
			};
			break;
		case TetriminoType::Z:
			_data = std::vector<std::vector<short>>
			{
				{ 5, 5, 0, 0 },
				{ 0, 5, 5, 0 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
			};
			break;
		case TetriminoType::J:
			_data = std::vector<std::vector<short>>
			{
				{ 6, 0, 0, 0 },
				{ 6, 6, 6, 0 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
			};
			break;
		case TetriminoType::L:
			_data = std::vector<std::vector<short>>
			{
				{ 0, 0, 7, 0 },
				{ 7, 7, 7, 0 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
			};
			break;
		}
	}

#pragma endregion Tetramino Shape Data

	static std::vector<TetriminoType> ALL_TYPES;
};

std::vector<TetriminoType> Tetrimino::ALL_TYPES{ TetriminoType::I, TetriminoType::O , TetriminoType::T , TetriminoType::S , TetriminoType::Z , TetriminoType::J , TetriminoType::L };

#endif