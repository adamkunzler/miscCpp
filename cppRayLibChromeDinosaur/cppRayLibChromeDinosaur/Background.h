#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <vector>

#include "Globals.h"

#include "raylib.h"

class BackgroundLayer
{
private:
	Texture2D _texture;	
	float _backgroundOffset{ 0.f };
	int _depth{ 1 };

public:
	BackgroundLayer(Texture2D texture, int depth);
	
	void update();

	void render() const;

	int getWidth() const;
};

// --------------------------------------------------------------------------------------------

class Background
{
private:
	std::vector<BackgroundLayer> _backgroundLayers;
	std::vector<BackgroundLayer> _foregroundLayers;

private:
	void render(std::vector<BackgroundLayer> layers) const;

public:
	Background();
	
	void update();

	void renderBackground() const;
	
	void renderForeground() const;
};

#endif