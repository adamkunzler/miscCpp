#include "SpriteFactory.h"

void SpriteFactory::registerSprite(SpriteType type, std::string filename, Vector2 dimensions, int numFrames)
{
	_spriteConfigs[type] = SpriteConfig{
		type,
		filename,
		dimensions,
		numFrames
	};
}

Sprite SpriteFactory::getSprite(SpriteType type)
{
	if (_spriteConfigs.count(type))
	{
		Sprite s{ _spriteConfigs[type] };
		return s;
	}
	else
	{
		std::cout << "Sprite for type " << type << " not found!" << std::endl;
		throw std::invalid_argument("SpriteFactory.getSprite() => type not found");
	}
}

std::ostream& operator<<(std::ostream& os, const SpriteType& type)
{
	switch (type)
	{
	case SpriteType::DINO_IDLE: os << "DINO_IDLE"; break;
	case SpriteType::DINO_JUMP: os << "DINO_JUMP"; break;
	case SpriteType::DINO_WALK: os << "DINO_WALK"; break;
	case SpriteType::DINO_RUN:  os << "DINO_RUN";  break;
	case SpriteType::DINO_DEAD: os << "DINO_DEAD"; break;
	case SpriteType::UNKNOWN:   os << "UNKNOWN";   break;
	}
	return os;
}