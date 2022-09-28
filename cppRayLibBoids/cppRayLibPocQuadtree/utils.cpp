#include "utils.h"

bool isIntersects(
	float x, float y, float w, float h,
	float xx, float yy, float ww, float hh)
{
	return (
		// left < right && right > left
		(x < xx + ww) && (x + w > xx)
		// top < bottom && bottom > top
		&& (y < yy + hh) && (y + h > yy)
		);
}

bool isRectContainsPoint(float x, float y, float w, float h, float px, float py)
{
	return (
		px >= x && px <= x + w
		&& py >= y && py <= y + h
		);
}