#include "DinoStateRun.h"

DinoStateRun::DinoStateRun() {	
	_sprite = Globals::instance().spriteFactory().getSprite(SpriteType::DINO_RUN);
}

void DinoStateRun::enter(Dino& dino)
{	
}

void DinoStateRun::exit(Dino& dino)
{	
}

void DinoStateRun::handleInput(Dino& dino)
{	
	if (IsKeyPressed(KEY_SPACE))  dino.setState(&DinoStateJump::getInstance());
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