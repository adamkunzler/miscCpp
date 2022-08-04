#pragma once

class BaseScene
{
protected:
	int width{0};
	int height{0};	
	float halfWidth{ 0.0f };
	float halfHeight{ 0.0f };
	
	sf::RenderWindow window;

	sf::Text txtFps;
	sf::Font font;
	bool isFPS{ false };
	
	virtual void drawScene(float elapsed) = 0;
	virtual void updateScene(float elapsed) = 0;
	virtual void processSceneEvents(const sf::Event& event) = 0;	

public:
	virtual std::vector<std::string> getOverlayMessages() = 0;
	virtual void customImGui() {};

	BaseScene(int width, int height, std::string title)
		: width{ width }, height{ height }, halfWidth{ (float)width / 2.0f }, halfHeight{ (float)height / 2.0f }
	{
		window.create(sf::VideoMode(width, height), title);	

		if (font.loadFromFile("assets/CourierPrime-Regular.ttf"))
		{
			txtFps.setFont(font);
			txtFps.setCharacterSize(18);
			txtFps.setFillColor(sf::Color::White);
			txtFps.setPosition(10, 10);
		}		
	}

	~BaseScene() {}

	void processEvents(const sf::Event &event)
	{
		processSceneEvents(event);
	}

	void update(float elapsed)
	{		
		updateScene(elapsed);
	}
	
	void draw(float elapsed)
	{		
		drawScene(elapsed);

		if (isFPS)
		{
			float fps = 1.0f / elapsed;			
			txtFps.setString(std::to_string((int)fps));
			window.draw(txtFps);			
		}
	}
	
	sf::RenderWindow& getWindow()
	{
		return window;
	}

	
};