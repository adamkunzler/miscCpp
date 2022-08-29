#include "DinoStateDead.h"

DinoStateDead::DinoStateDead() {	
	_sprite = DinoGame::instance().spriteFactory().getSprite(SpriteType::DINO_DEAD);	

	_sprite.setCurrentFrame(_sprite.getFrameCount() - 1);
	_sprite.isAnimated = false;
}

void DinoStateDead::enter(Dino& dino)
{		
}

void DinoStateDead::exit(Dino& dino)
{	
}

void DinoStateDead::handleInput(Dino& dino)
{		
}

void DinoStateDead::update(Dino& dino)
{
	_sprite.update();
}

void DinoStateDead::render(Dino& dino)
{
	_sprite.render(dino.position);

	DrawText("You dead...", 10, 10, 50, RAYWHITE);
}

// construct on first use idiom (https://isocpp.org/wiki/faq/ctors#static-init-order)
DinoState& DinoStateDead::getInstance()
{
	static std::shared_ptr<DinoState> idle = std::make_shared<DinoStateDead>();
	return *idle;
}