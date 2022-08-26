#include "DinoGame.h"

DinoGame::DinoGame()
{
	std::cout << "\nDinoGame ctor\n" << std::endl;

	Globals::instance().spriteFactory().registerSprite(SpriteType::DINO_IDLE, "resources/dino/dino-idle.png", Vector2{ 680, 472 });
	Globals::instance().spriteFactory().registerSprite(SpriteType::DINO_JUMP, "resources/dino/dino-jump.png", Vector2{ 680, 472 });
	Globals::instance().spriteFactory().registerSprite(SpriteType::DINO_WALK, "resources/dino/dino-walk.png", Vector2{ 680, 472 });
	Globals::instance().spriteFactory().registerSprite(SpriteType::DINO_RUN, "resources/dino/dino-run.png", Vector2{ 680, 472 });
	Globals::instance().spriteFactory().registerSprite(SpriteType::DINO_DEAD, "resources/dino/dino-dead.png", Vector2{ 680, 472 });

	_dino = std::make_unique<Dino>();
	_dino->position = Vector2{ 200.f, 660.f };
	_dino->startY = 660.f;
}

void DinoGame::handleInput()
{	
	_dino->handleInput();

	if (IsKeyPressed(KEY_KP_ADD)) Globals::instance().worldSpeed += 1.f;	
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