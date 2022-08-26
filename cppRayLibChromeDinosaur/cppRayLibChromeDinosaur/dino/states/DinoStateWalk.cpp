#include "DinoStateWalk.h"

DinoStateWalk::DinoStateWalk() {
	std::cout << "DinoStateWalk ctor" << std::endl;
	_sprite = Globals::instance().spriteFactory().getSprite(SpriteType::DINO_WALK);
}

void DinoStateWalk::enter(Dino& dino)
{
	std::cout << "enter DinoStateWalk..." << std::endl;
}

void DinoStateWalk::exit(Dino& dino)
{
	std::cout << "exit DinoStateWalk..." << std::endl;
}

void DinoStateWalk::handleInput(Dino& dino)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		std::cout << "DinoStateWalk" << std::endl;
	}
}

void DinoStateWalk::update(Dino& dino)
{
	_sprite.update();
}

void DinoStateWalk::render(Dino& dino)
{
	_sprite.render(dino.position, dino.size);
}

// construct on first use idiom (https://isocpp.org/wiki/faq/ctors#static-init-order)
DinoState& DinoStateWalk::getInstance()
{
	static std::shared_ptr<DinoState> idle = std::make_shared<DinoStateWalk>();
	return *idle;
}