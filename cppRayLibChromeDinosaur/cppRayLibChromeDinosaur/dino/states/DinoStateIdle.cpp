#include "DinoStateIdle.h"

DinoStateIdle::DinoStateIdle() {	
	_sprite = Globals::instance().spriteFactory().getSprite(SpriteType::DINO_IDLE);
}

void DinoStateIdle::enter(Dino& dino)
{	
}

void DinoStateIdle::exit(Dino& dino)
{	
}

void DinoStateIdle::handleInput(Dino& dino)
{	
	if (IsKeyPressed(KEY_SPACE))
	{
		dino.setState(&DinoStateWalk::getInstance());
		Globals::instance().worldSpeed = 1;
	}
}

void DinoStateIdle::update(Dino& dino)
{
	_sprite.update();
}

void DinoStateIdle::render(Dino& dino)
{
	_sprite.render(dino.position);

	DrawText("Press 'spacebar' to start.", 10, 10, 50, RAYWHITE);
}

// construct on first use idiom (https://isocpp.org/wiki/faq/ctors#static-init-order)
DinoState& DinoStateIdle::getInstance()
{
	static std::shared_ptr<DinoState> idle = std::make_shared<DinoStateIdle>();
	return *idle;
}