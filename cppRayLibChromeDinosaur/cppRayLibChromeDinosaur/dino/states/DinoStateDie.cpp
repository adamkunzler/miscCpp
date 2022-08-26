#include "DinoStateDie.h"

DinoStateDie::DinoStateDie() {
	std::cout << "DinoStateDie ctor" << std::endl;
	_sprite = Globals::instance().spriteFactory().getSprite(SpriteType::DINO_DEAD);
}

void DinoStateDie::enter(Dino& dino)
{
	std::cout << "enter DinoStateDie..." << std::endl;
	_sprite.setCurrentFrame(0);
}

void DinoStateDie::exit(Dino& dino)
{
	std::cout << "exit DinoStateDie..." << std::endl;
}

void DinoStateDie::handleInput(Dino& dino)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		std::cout << "DinoStateDie" << std::endl;
	}
}

void DinoStateDie::update(Dino& dino)
{
	_sprite.update();

	if (_sprite.getFrameCount() - 1 == _sprite.getCurrentFrame())
		dino.setState(&DinoStateDead::getInstance());
}

void DinoStateDie::render(Dino& dino)
{
	_sprite.render(dino.position, dino.size);
}

// construct on first use idiom (https://isocpp.org/wiki/faq/ctors#static-init-order)
DinoState& DinoStateDie::getInstance()
{
	static std::shared_ptr<DinoState> idle = std::make_shared<DinoStateDie>();
	return *idle;
}