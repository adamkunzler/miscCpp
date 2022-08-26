#include "DinoStateRun.h"

DinoStateRun::DinoStateRun() {
	std::cout << "DinoStateRun ctor" << std::endl;
	_sprite = Globals::instance().spriteFactory().getSprite(SpriteType::DINO_RUN);
}

void DinoStateRun::enter(Dino& dino)
{
	std::cout << "enter DinoStateRun..." << std::endl;
}

void DinoStateRun::exit(Dino& dino)
{
	std::cout << "exit DinoStateRun..." << std::endl;
}

void DinoStateRun::handleInput(Dino& dino)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		std::cout << "DinoStateRun" << std::endl;
	}
}

void DinoStateRun::update(Dino& dino)
{
	_sprite.update();
}

void DinoStateRun::render(Dino& dino)
{
	_sprite.render(dino.position, dino.size);
}

// construct on first use idiom (https://isocpp.org/wiki/faq/ctors#static-init-order)
DinoState& DinoStateRun::getInstance()
{
	static std::shared_ptr<DinoState> idle = std::make_shared<DinoStateRun>();
	return *idle;
}