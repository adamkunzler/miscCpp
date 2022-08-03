#ifndef RAYLIBMATHHELPER_H
#define RAYLIBMATHHELPER_H

#include "raylib.h"

Vector2& operator+=(Vector2& lhs, const Vector2& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

#endif