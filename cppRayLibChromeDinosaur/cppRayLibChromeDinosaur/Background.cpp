#include "Background.h"

BackgroundLayer::BackgroundLayer(Texture2D texture, int depth)
	: _texture{texture}, _depth{ depth }
{

}

void BackgroundLayer::update()
{	
	_backgroundOffset += DinoGame::instance().worldSpeed;
	auto layerOffset = (int)(((float)_depth / 11.f) * _backgroundOffset);
	if (layerOffset > getWidth())
		_backgroundOffset = layerOffset - getWidth();
}

void BackgroundLayer::render() const
{
	auto layerOffset = (int)(((float)_depth / 11.f) * _backgroundOffset);
	for (int x = -layerOffset; x < GetScreenWidth(); x += getWidth())	
		DrawTexture(_texture, x, 0, WHITE);	
}

int BackgroundLayer::getWidth() const { return (int)_texture.width; }

// --------------------------------------------------------------------------------------------


Background::Background() {}

void Background::init()
{
	_backgroundLayers.push_back(BackgroundLayer{ LoadTexture("resources/Background layers/Layer_0011_0.png"),       1 });
	_backgroundLayers.push_back(BackgroundLayer{ LoadTexture("resources/Background layers/Layer_0009_2.png"),       2 });
	_backgroundLayers.push_back(BackgroundLayer{ LoadTexture("resources/Background layers/Layer_0008_3.png"),       3 });
	_backgroundLayers.push_back(BackgroundLayer{ LoadTexture("resources/Background layers/Layer_0007_Lights.png"),  4 });
	_backgroundLayers.push_back(BackgroundLayer{ LoadTexture("resources/Background layers/Layer_0006_4.png"),       5 });
	_backgroundLayers.push_back(BackgroundLayer{ LoadTexture("resources/Background layers/Layer_0005_5.png"),       6 });
	_backgroundLayers.push_back(BackgroundLayer{ LoadTexture("resources/Background layers/Layer_0004_Lights.png"),  7 });
	_backgroundLayers.push_back(BackgroundLayer{ LoadTexture("resources/Background layers/Layer_0003_6.png"),       8 });
	_backgroundLayers.push_back(BackgroundLayer{ LoadTexture("resources/Background layers/Layer_0002_7.png"),       9 });

	_foregroundLayers.push_back(BackgroundLayer{ LoadTexture("resources/Background layers/Layer_0001_8.png"),      10 });
	_foregroundLayers.push_back(BackgroundLayer{ LoadTexture("resources/Background layers/Layer_0000_9.png"),      11 });
}

void Background::renderBackground() const
{			
	render(_backgroundLayers);
}

void Background::renderForeground() const
{
	render(_foregroundLayers);	
}

void Background::update()
{
	for (auto& layer : _backgroundLayers)
		layer.update();

	for (auto& layer : _foregroundLayers)
		layer.update();	
}

void Background::render(std::vector<BackgroundLayer> layers) const
{		
	for (const auto& layer : layers)
	{
		layer.render();		
	}
}