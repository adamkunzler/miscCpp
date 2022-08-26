#include "DinoStateIdle.h"

DinoStateIdle::DinoStateIdle() {
	std::cout << "DinoStateIdle ctor" << std::endl;
	_sprite = Globals::instance().spriteFactory().getSprite(SpriteType::DINO_IDLE);
}

void DinoStateIdle::enter(Dino& dino)
{
	std::cout << "enter DinoStateIdle..." << std::endl;
}

void DinoStateIdle::exit(Dino& dino)
{
	std::cout << "exit DinoStateIdle..." << std::endl;
}

void DinoStateIdle::handleInput(Dino& dino)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		std::cout << "DinoStateIdle" << std::endl;
	}
}

void DinoStateIdle::update(Dino& dino)
{
	_sprite.update();
}

void DinoStateIdle::render(Dino& dino)
{
	_sprite.render(dino.position, dino.size);
}

// construct on first use idiom (https://isocpp.org/wiki/faq/ctors#static-init-order)
DinoState& DinoStateIdle::getInstance()
{
	static std::shared_ptr<DinoState> idle = std::make_shared<DinoStateIdle>();
	return *idle;
}