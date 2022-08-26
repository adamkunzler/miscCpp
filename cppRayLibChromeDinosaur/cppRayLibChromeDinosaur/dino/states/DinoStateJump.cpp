#include "DinoStateJump.h"

DinoStateJump::DinoStateJump() {	
	_sprite = Globals::instance().spriteFactory().getSprite(SpriteType::DINO_JUMP);
}

void DinoStateJump::enter(Dino& dino)
{	
	_isJumpFinished = false;
	_sprite.setCurrentFrame(2);
	_velocity.y = 25;
}

void DinoStateJump::exit(Dino& dino)
{	
	_isJumpFinished = true;
	_sprite.setCurrentFrame(2);
}

void DinoStateJump::handleInput(Dino& dino)
{	
}

void DinoStateJump::update(Dino& dino)
{	
	_sprite.update();
	
	dino.position.y -= _velocity.y;
	_velocity.y -= 0.75f;

	if (dino.position.y >= dino.startY) 
	{
		dino.restorePreviousState();		
		dino.position.y = dino.startY;
	}
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