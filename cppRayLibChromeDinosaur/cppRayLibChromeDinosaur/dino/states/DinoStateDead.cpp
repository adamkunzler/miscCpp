#include "DinoStateDead.h"

DinoStateDead::DinoStateDead() {
	std::cout << "DinoStateDead ctor" << std::endl;
	_sprite = Globals::instance().spriteFactory().getSprite(SpriteType::DINO_DEAD);	

	_sprite.setCurrentFrame(_sprite.getFrameCount() - 1);
	_sprite.isAnimated = false;
}

void DinoStateDead::enter(Dino& dino)
{
	std::cout << "enter DinoStateDead..." << std::endl;
}

void DinoStateDead::exit(Dino& dino)
{
	std::cout << "exit DinoStateDead..." << std::endl;
}

void DinoStateDead::handleInput(Dino& dino)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		std::cout << "DinoStateDead" << std::endl;
	}
}

void DinoStateDead::update(Dino& dino)
{
	_sprite.update();
}

void DinoStateDead::render(Dino& dino)
{
	_sprite.render(dino.position, dino.size);
}

// construct on first use idiom (https://isocpp.org/wiki/faq/ctors#static-init-order)
DinoState& DinoStateDead::getInstance()
{
	static std::shared_ptr<DinoState> idle = std::make_shared<DinoStateDead>();
	return *idle;
}