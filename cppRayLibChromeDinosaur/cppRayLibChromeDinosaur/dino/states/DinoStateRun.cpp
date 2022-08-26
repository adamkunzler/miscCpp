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
	if (IsKeyPressed(KEY_KP_ADD))
		Globals::instance().worldSpeed++;

	if (IsKeyPressed(KEY_SPACE))  
		dino.setState(&DinoStateJump::getInstance());

	if (IsKeyPressed(KEY_ENTER))
		dino.setState(&DinoStateDie::getInstance());
}

void DinoStateRun::update(Dino& dino)
{
	_sprite.update();

	if (Globals::instance().worldSpeed > 15) // 12 is the initial fps (80% of 15 is 12)
		_sprite.frameSpeed = (int)(Globals::instance().worldSpeed * 0.8f);
}

void DinoStateRun::render(Dino& dino)
{
	_sprite.render(dino.position, dino.size);

	DrawText("Running...", 10, 10, 50, RAYWHITE);
}

// construct on first use idiom (https://isocpp.org/wiki/faq/ctors#static-init-order)
DinoState& DinoStateRun::getInstance()
{
	static std::shared_ptr<DinoState> idle = std::make_shared<DinoStateRun>();
	return *idle;
}