#pragma once

typedef unsigned short ushort;

static std::random_device rd;
static std::mt19937 gen(rd());

static float WORLD_SCALE{ 46.0f };
static float WORLD_PADDING{ 1.5f };

#include "game/Tetrimino.h"
#include "game/Playfield.h"

class BaseScene;

class TetrisScene : public BaseScene
{
private:
		
	Playfield _playfield;

	sf::Music _music;

	sf::RenderTexture _texture;
	sf::Sprite _sprite;
	sf::VertexArray _gfxPixels;

	Tetrimino _a;
	Tetrimino _b;

public:
	TetrisScene(int width, int height, std::string title)
		:BaseScene{ width, height, title }
	{
		if (!_texture.create(width, height))
			std::cout << "Failed to create TetrisScene texture!" << std::endl;

		_gfxPixels.setPrimitiveType(sf::Quads);

		_a.random();
		_b.random();
		
		if (!_music.openFromFile("./assets/korobeiniki.ogg"))
			std::cout << "Failed to load music!" << std::endl;
		
		_music.setLoop(true);
		_music.play();
	}

	void processSceneEvents(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				std::cout << "PAUSE GAME" << std::endl;
			}

			if (event.key.code == sf::Keyboard::Space)
			{
				//spawn();
			}
		}

		_playfield.handleInput(event);		
	}

	void spawn() 
	{
		_a.setType(_b.getType());
		_b.random();
	}

	void updateScene(float elapsed) override
	{
		_playfield.update(elapsed);
	}

	void drawScene(float elapsed) override
	{
		_texture.clear();		
		_gfxPixels.clear();

		auto playfieldSprite = _playfield.render(50.0f, 50.0f);

		auto next = _playfield.getNextTetrimino();
		auto av = next.render(600.0f, 300.0f, 2.5f);		
		for (auto i = 0; i < av.getVertexCount(); ++i)
			_gfxPixels.append(av[i]);
		

		_texture.draw(_gfxPixels);
		_texture.display();
		_sprite.setTexture(_texture.getTexture());		
				
		window.draw(_sprite);
		window.draw(playfieldSprite);		
	}
	
	std::vector<std::string> getOverlayMessages()
	{
		std::vector<std::string> msg;

		msg.push_back("Level: " + std::to_string(_playfield.getLevel()));
		msg.push_back("Score: " + std::to_string(_playfield.getScore()));
		msg.push_back("Lines: " + std::to_string(_playfield.getLines()));

		return msg;
	}
};