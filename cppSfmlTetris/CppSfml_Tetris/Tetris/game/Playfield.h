#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <chrono>
#include <thread>

#include "Tetrimino.h"

enum class Bounds
{
	InBounds = 1,
	OutOfBounds = 2,
	AtBottom = 3,
	BottomCollision = 4,
	SideCollision = 5
};

enum class RotateDir
{
	Clockwise = 1,
	CounterClockwise = 2
};

enum class State
{
	Playing = 1,
	GameOver = 2,
	ClearingLines = 3
};

class Playfield
{
public:
#pragma region ctor

	Playfield()
	{	
		if (!_texture.create(COLUMNS * WORLD_SCALE, (ROWS - NUM_HIDDEN_ROWS) * WORLD_SCALE))
			std::cout << "Failed to create Playfield texture!" << std::endl;

		_gfxPixels.setPrimitiveType(sf::Quads);

		initializePlayfield();
	}

	~Playfield()
	{		
	}

	void initializePlayfield()
	{
		std::cout << "Initializing Playfield..." << std::endl;

		_next.random();
		spawnTetramino();

		_numLinesCleared = 0;
		_score = 0;

		// init _grid to 0
		for (std::size_t row = 20; row < ROWS; ++row)
			for (std::size_t col = 0; col < COLUMNS; ++col)
				_grid[row][col] = 0;		

		_state = State::Playing;
	}

	void gameOver()
	{
		_state = State::GameOver;
		std::cout << "Game Over" << std::endl;	
		std::cout << "\nLevel: " << getLevel() << std::endl;
		std::cout << "Score: " << _score << std::endl;
		std::cout << "Lines: " << _numLinesCleared << std::endl;
		std::cout << "\nPress 'Spacebar' to play again..." << std::endl;
	}
#pragma endregion ctor

#pragma region Update

	void update(float elapsed)
	{				
		// PLAYING Timer
		_updateTimer += elapsed;
		if (_updateTimer < _speed && _state == State::Playing) return;
		_updateTimer = 0.0f;

		// CLEARLINES Timer
		_clearLinesTimer += elapsed;
		if (_clearLinesTimer < _clearLinesSpeed && _state == State::ClearingLines) return;
		_clearLinesTimer = 0.0f;
		
		// state specific update logic
		if (_state == State::ClearingLines)
		{			
			_clearLinesSize--;			
			if (_clearLinesSize == -1)
			{												
				for (const auto rowToClear : _rowsToClear)
				{
					for (auto row = rowToClear; row > 0; --row)
					{
						for (auto col = 0; col < COLUMNS; ++col)
						{
							_grid[row][col] = _grid[row - 1][col];
						}
					}
				}

				_rowsToClear.clear();
				_clearLinesSize = CLEAR_LINE_SIZE_START;				

				_state = State::Playing;
			}

		}
		else if (_state == State::GameOver)
		{

		}
		else if (_state == State::Playing)
		{						
			if (_canSpawn) spawnTetramino();

			checkForLinesToClear();

			if (!_holding)
				moveTetramino(DOWN);			
		}
	}

	/// <summary>
	/// Scan playfields top to bottom looking for filled rows
	/// </summary>
	void checkForLinesToClear()
	{		
		for (std::size_t row = 0; row < ROWS; ++row)
		{
			int setCellCount{ 0 };
			for (std::size_t col = 0; col < COLUMNS; ++col)
			{
				auto cellValue = _grid[row][col];
				if (!cellValue) continue;

				setCellCount++;
			}

			if (setCellCount == COLUMNS)			
				_rowsToClear.push_back(row);			
		}

		if (_rowsToClear.size() > 0)
		{
			_state = State::ClearingLines;
			_numLinesCleared += _rowsToClear.size();
			updateScore(_rowsToClear.size());
		}
	}

	/// <summary>
	/// Move the tetramino in the specified direction. Checks bounds.
	/// </summary>
	/// <param name="direction"></param>
	void moveTetramino(const sf::Vector2i& direction)
	{
		_currentPosition += direction;

		auto bounds = checkTetraminoBounds(direction);
		if (bounds == Bounds::AtBottom || bounds == Bounds::BottomCollision)
		{			
			_currentPosition -= direction;
			copyTetraminoToGrid();
			_canSpawn = true;
		}
		else if (bounds == Bounds::OutOfBounds || bounds == Bounds::SideCollision)
			_currentPosition -= direction;		
	}

	/// <summary>
	/// Rotate the tetramino the specified direction. Checks bounds.
	/// </summary>
	/// <param name="direction"></param>
	void rotateTetramino(const RotateDir& direction)
	{
		if (direction == RotateDir::Clockwise)		
			_current.rotateCW();		
		else if (direction == RotateDir::CounterClockwise)		
			_current.rotateCCW();

		auto bounds = checkTetraminoBounds();
		if (bounds == Bounds::AtBottom || bounds == Bounds::SideCollision || bounds == Bounds::OutOfBounds)
		{
			if (direction == RotateDir::Clockwise)
				_current.rotateCCW();
			else if (direction == RotateDir::CounterClockwise)
				_current.rotateCW();			
		}				
	}

	/// <summary>
	/// Determine if the tetramino is out of bounds, at the bottom of the playfield, or collided with another tetramino
	/// </summary>
	/// <returns></returns>
	Bounds checkTetraminoBounds(const sf::Vector2i& direction = sf::Vector2i{ 0,0 })
	{
		const auto data = _current.getData();
		auto dim = _current.getDimension();

		for (std::size_t row = 0; row < dim; ++row)
		{
			for (std::size_t col = 0; col < dim; ++col)
			{
				auto cellValue = data[row][col];
				if (!cellValue) continue;

				auto x = _currentPosition.x + col;
				auto y = _currentPosition.y + row;

				if (x < 0 || x >= COLUMNS) return Bounds::OutOfBounds;
				if (y == ROWS) return Bounds::AtBottom;

				auto gridValue = _grid[y][x];				
				if (gridValue) {					
					if (direction == DOWN)
						return Bounds::BottomCollision;
					
					return Bounds::SideCollision;
				}
			}
		}

		return Bounds::InBounds;
	}

	/// <summary>
	/// Copy the current tetramino to the grid
	/// </summary>
	void copyTetraminoToGrid()
	{
		const auto data = _current.getData();
		auto dim = _current.getDimension();

		for (std::size_t row = 0; row < dim; ++row)
		{
			for (std::size_t col = 0; col < dim; ++col)
			{
				auto cellValue = data[row][col];
				if (!cellValue) continue;


				auto tx = _currentPosition.x + col;
				auto ty = _currentPosition.y + row;								

				if (tx < 0 || ty < 0) continue;				
				if(ty < ROWS && tx < COLUMNS )
					_grid[ty][tx] = cellValue;
			}
		}
	}
	
#pragma endregion Update

#pragma region Render

	sf::Sprite& render(const float& x, const float& y)
	{		
		_texture.clear();
		_gfxPixels.clear();

		// draw playing field as cells
		for (std::size_t row = 20; row < ROWS; ++row)
		{
			for (std::size_t col = 0; col < COLUMNS; ++col)
			{
				if (_state == State::ClearingLines)
				{
					if (std::find(_rowsToClear.begin(), _rowsToClear.end(), row) != _rowsToClear.end())
						renderClearLineCell(row, col);
					else											
						renderCell(row, col);
				}
				else				
					renderCell(row, col);				
			}
		}

		// draw _current tetramino
		auto tx = _currentPosition.x * WORLD_SCALE;
		auto ty = (_currentPosition.y - NUM_HIDDEN_ROWS) * WORLD_SCALE;
		auto currentVerts = _current.render(tx, ty, WORLD_PADDING);
		for (std::size_t i = 0; i < currentVerts.getVertexCount(); ++i)
			_gfxPixels.append(currentVerts[i]);

		// render the everything to a sprite
		_texture.draw(_gfxPixels);
		_texture.display();
		_sprite.setTexture(_texture.getTexture());

		_sprite.setPosition(x, y);

		return _sprite;
	}

	void renderClearLineCell(std::size_t row, std::size_t col)
	{
		// draw regular cell
		sf::Color color = Tetrimino::getColor(_grid[row][col]);

		float sx = (float)col * WORLD_SCALE;
		float sy = (float)(row - NUM_HIDDEN_ROWS) * WORLD_SCALE;

		float percent = ((float)CLEAR_LINE_SIZE_START - (float)_clearLinesSize) / (float)CLEAR_LINE_SIZE_START;
		float offset{(percent * (float)WORLD_SCALE) / 2.0f};

		// top left | top right | bottom right | bottom left
		sf::Vertex v1{ sf::Vector2f{ sx + offset, sy + offset }, color };
		sf::Vertex v2{ sf::Vector2f{ sx + WORLD_SCALE - offset, sy + offset }, color };
		sf::Vertex v3{ sf::Vector2f{ sx + WORLD_SCALE - offset, sy + WORLD_SCALE - offset }, color };
		sf::Vertex v4{ sf::Vector2f{ sx + offset, sy + WORLD_SCALE - offset}, color };

		_gfxPixels.append(v1); _gfxPixels.append(v2); _gfxPixels.append(v3); _gfxPixels.append(v4);
	}

	void renderCell(std::size_t row, std::size_t col)
	{
		// draw regular cell
		sf::Color color = Tetrimino::getColor(_grid[row][col]);

		float sx = (float)col * WORLD_SCALE;
		float sy = (float)(row - NUM_HIDDEN_ROWS) * WORLD_SCALE;

		// top left | top right | bottom right | bottom left
		sf::Vertex v1{ sf::Vector2f{ sx + WORLD_PADDING, sy + WORLD_PADDING }, color };
		sf::Vertex v2{ sf::Vector2f{ sx + WORLD_SCALE - WORLD_PADDING, sy + WORLD_PADDING }, color };
		sf::Vertex v3{ sf::Vector2f{ sx + WORLD_SCALE - WORLD_PADDING, sy + WORLD_SCALE - WORLD_PADDING }, color };
		sf::Vertex v4{ sf::Vector2f{ sx + WORLD_PADDING, sy + WORLD_SCALE - WORLD_PADDING}, color };

		_gfxPixels.append(v1); _gfxPixels.append(v2); _gfxPixels.append(v3); _gfxPixels.append(v4);
	}

#pragma endregion Render

#pragma region Input

	void handleInput(const sf::Event& event)
	{
		if (_state == State::ClearingLines)
		{

		}
		else if (_state == State::GameOver)
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					initializePlayfield();
				}
			}
		}
		else if (_state == State::Playing)
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Right)
				{
					moveTetramino(RIGHT);
				}

				if (event.key.code == sf::Keyboard::Left)
				{
					moveTetramino(LEFT);
				}

				if (event.key.code == sf::Keyboard::Up)
				{
					rotateTetramino(RotateDir::Clockwise);
				}

				if (event.key.code == sf::Keyboard::LControl)
				{
					rotateTetramino(RotateDir::CounterClockwise);
				}

				if (event.key.code == sf::Keyboard::Down)
				{
					moveTetramino(DOWN);
				}

				if (event.key.code == sf::Keyboard::LShift)
				{
					_holding = true;
				}

				if (event.key.code == sf::Keyboard::Space)
				{
					std::cout << "Hard Drop" << std::endl;
				}
			}

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::LShift)
				{
					_holding = false;
				}
			}
		}
	}

#pragma endregion Input

	void spawnTetramino()
	{		
		_canSpawn = false;

		_current.setType(_next.getType());		
		_next.random();
		
		_currentPosition = sf::Vector2i{ 3, NUM_HIDDEN_ROWS - 1 };

		auto bounds = checkTetraminoBounds();
		if (bounds != Bounds::InBounds)
		{
			gameOver();
		}
	}

	Tetrimino& getNextTetrimino()
	{
		return _next;
	}

	void updateScore(std::size_t numLinesCleared)
	{
		switch (numLinesCleared)
		{
		case 1: _score += 1; break;
		case 2: _score += 3; break;
		case 3: _score += 5; break;
		case 4: _score += 8; break;
		}
	}

	short getLevel()
	{
		return (_numLinesCleared / 10) + 1;
	}

	unsigned int getScore()
	{
		return _score;
	}

	short getLines()
	{
		return _numLinesCleared;
	}

private:
	sf::RenderTexture _texture;
	sf::Sprite _sprite;
	sf::VertexArray _gfxPixels;

	State _state{ State::Playing };

	float _updateTimer{ 0.0f };
	float _speed{ 0.55f };

	std::vector<ushort> _rowsToClear;
	float _clearLinesTimer{ 0.0f };
	float _clearLinesSpeed{ 0.025f };
	static const short CLEAR_LINE_SIZE_START{ 10 };
	short _clearLinesSize{ CLEAR_LINE_SIZE_START };

	static const ushort COLUMNS{ 10 };
	static const ushort ROWS{ 40 };
	static const int NUM_HIDDEN_ROWS{ 20 };

	sf::Vector2i LEFT{ -1, 0 };
	sf::Vector2i RIGHT{ 1, 0 };
	sf::Vector2i DOWN{ 0, 1 };

	ushort _grid[ROWS][COLUMNS];

	Tetrimino _current;
	Tetrimino _next;
	bool _canSpawn{ false };
	short _numLinesCleared{ 0 };
	unsigned int _score{ 0 };

	sf::Vector2i _currentPosition;
	bool _holding{ false };	
};


#endif