#include "DinoStateDie.h"

DinoStateDie::DinoStateDie() {	
	_sprite = Globals::instance().spriteFactory().getSprite(SpriteType::DINO_DEAD);
	_sprite.frameSpeed = 7;
}

void DinoStateDie::enter(Dino& dino)
{	
	_sprite.setCurrentFrame(0);	
}

void DinoStateDie::exit(Dino& dino)
{	
	Globals::instance().worldSpeed = 0;
}

void DinoStateDie::handleInput(Dino& dino)
{	
}

void DinoStateDie::update(Dino& dino)
{
	_sprite.update();

	Globals::instance().worldSpeed -= (Globals::instance().worldSpeed / _sprite.getFrameCount() / 2.f);

	if (_sprite.getFrameCount() - 1 == _sprite.getCurrentFrame())
		dino.setState(&DinoStateDead::getInstance());
}

void DinoStateDie::render(Dino& dino)
{
	_sprite.render(dino.position, dino.size);

	DrawText("Urgh! Argh!", 10, 10, 50, RAYWHITE);
}

// construct on first use idiom (https://isocpp.org/wiki/faq/ctors#static-init-order)
DinoState& DinoStateDie::getInstance()
{
	static std::shared_ptr<DinoState> idle = std::make_shared<DinoStateDie>();
	return *idle;
}