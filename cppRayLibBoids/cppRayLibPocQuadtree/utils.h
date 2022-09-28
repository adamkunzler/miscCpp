#ifndef UTILS_H
#define UTILS_H

bool isIntersects(
	float x, float y, float w, float h,
	float xx, float yy, float ww, float hh);

bool isRectContainsPoint(float x, float y, float w, float h, float px, float py);

#endif