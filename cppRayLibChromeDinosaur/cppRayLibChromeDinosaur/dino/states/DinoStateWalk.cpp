#include "DinoStateWalk.h"

DinoStateWalk::DinoStateWalk() {	
	_sprite = Globals::instance().spriteFactory().getSprite(SpriteType::DINO_WALK);
}

void DinoStateWalk::enter(Dino& dino)
{	
}

void DinoStateWalk::exit(Dino& dino)
{	
}

void DinoStateWalk::handleInput(Dino& dino)
{	
	if (IsKeyPressed(KEY_KP_ADD))
		Globals::instance().worldSpeed++;

	if (IsKeyPressed(KEY_SPACE))  
		dino.setState(&DinoStateJump::getInstance());

	if (IsKeyPressed(KEY_ENTER))
		dino.setState(&DinoStateDie::getInstance());
}

void DinoStateWalk::update(Dino& dino)
{
	_sprite.update();

	if (Globals::instance().worldSpeed > 5 && !dino.isJumping() && !dino.isDyingOrDead())
		dino.setState(&DinoStateRun::getInstance());
}

void DinoStateWalk::render(Dino& dino)
{
	_sprite.render(dino.position, dino.size);

	DrawText("Walking...", 10, 10, 50, RAYWHITE);
}

// construct on first use idiom (https://isocpp.org/wiki/faq/ctors#static-init-order)
DinoState& DinoStateWalk::getInstance()
{
	static std::shared_ptr<DinoState> idle = std::make_shared<DinoStateWalk>();
	return *idle;
}