#include "DinoStateJump.h"

DinoStateJump::DinoStateJump() {
	std::cout << "DinoStateJump ctor" << std::endl;
	_sprite = Globals::instance().spriteFactory().getSprite(SpriteType::DINO_JUMP);
}

void DinoStateJump::enter(Dino& dino)
{
	std::cout << "enter DinoStateJump..." << std::endl;
}

void DinoStateJump::exit(Dino& dino)
{
	std::cout << "exit DinoStateJump..." << std::endl;
}

void DinoStateJump::handleInput(Dino& dino)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		std::cout << "DinoStateJump" << std::endl;
	}
}

void DinoStateJump::update(Dino& dino)
{
	_sprite.update();
}

void DinoStateJump::render(Dino& dino)
{
	_sprite.render(dino.position, dino.size);
}

// construct on first use idiom (https://isocpp.org/wiki/faq/ctors#static-init-order)
DinoState& DinoStateJump::getInstance()
{
	static std::shared_ptr<DinoState> idle = std::make_shared<DinoStateJump>();
	return *idle;
}