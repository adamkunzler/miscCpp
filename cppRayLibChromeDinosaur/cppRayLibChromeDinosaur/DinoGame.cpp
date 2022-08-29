#include "DinoGame.h"

// private static
DinoGame DinoGame::_instance;

// public static
DinoGame& DinoGame::instance() { return _instance; }


DinoGame::DinoGame()
{
	std::cout << "\nDinoGame ctor\n" << std::endl;	

	_spriteFactory = std::make_unique<SpriteFactory>();
}

void DinoGame::init()
{
	std::cout << "DinoGame::init()" << std::endl;

	_spriteFactory->registerSprite(SpriteType::DINO_IDLE, "resources/dino/dino-idle.png", Vector2{ 256, 178 }, 10);
	_spriteFactory->registerSprite(SpriteType::DINO_JUMP, "resources/dino/dino-jump.png", Vector2{ 256, 178 }, 12);
	_spriteFactory->registerSprite(SpriteType::DINO_WALK, "resources/dino/dino-walk.png", Vector2{ 256, 178 }, 10);
	_spriteFactory->registerSprite(SpriteType::DINO_RUN, "resources/dino/dino-run.png", Vector2{ 256, 178 }, 8);
	_spriteFactory->registerSprite(SpriteType::DINO_DEAD, "resources/dino/dino-dead.png", Vector2{ 256, 178 }, 8);

	_background.init();

	_dino = std::make_unique<Dino>();
	_dino->position = Vector2{ 200.f, 660.f };
	_dino->startY = 660.f;
}

void DinoGame::handleInput()
{	
	_dino->handleInput();

	if (IsKeyPressed(KEY_KP_ADD)) worldSpeed += 1.f;	
}

void DinoGame::update(){			
	_dino->update();
	_background.update();
}

void DinoGame::render()
{
	_background.renderBackground();
	_dino->render();
	_background.renderForeground();	
}

SpriteFactory& DinoGame::spriteFactory() { return *_spriteFactory; }

Vector2& operator+=(Vector2& a, const Vector2& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}