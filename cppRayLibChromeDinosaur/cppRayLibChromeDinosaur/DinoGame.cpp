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
	_dino->position = Vector2{ 200.f, 700.f };
	_dino->startY = 700.f;
}

void DinoGame::handleInput()
{
	_dino->handleInput();
}

void DinoGame::update()
{
	_dino->update();
}

void DinoGame::render()
{
	_dino->render();

	DrawText("Dinosaurs!", 10, 10, 50, RAYWHITE);
}