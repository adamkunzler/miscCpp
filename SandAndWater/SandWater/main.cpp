#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include <FastNoise/FastNoise.h>

#include <SFML/Graphics.hpp>

#include "utils/Math.h"
#include "utils/RandomGenerator.h"
#include "BaseScene.h"
#include "Scene.h"

int main()
{    
    Scene scene{ 3200, 1600, ".: Sand And Water :." };

    sf::Clock clock;
    while (scene.getWindow().isOpen())
    {
        sf::Event event;
        while (scene.getWindow().pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                scene.getWindow().close();
            
            /*if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(scene.getWindow());
                scene.spawnSand(mousePosition.x, mousePosition.y);
            }*/
            
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                scene.cycleTools();
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Key::Space)
                {
                    scene.clearWorld();
                }
                else if (event.key.code == sf::Keyboard::Key::Num0)
                    scene.setToolErase();
                else if (event.key.code == sf::Keyboard::Key::Num1)
                    scene.setToolSand();
                else if (event.key.code == sf::Keyboard::Key::Num2)
                    scene.setToolStone();
                else if (event.key.code == sf::Keyboard::Key::Num3)
                    scene.setToolWater();
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(scene.getWindow());
            scene.spawnElement(mousePosition.x, mousePosition.y);
        }

        sf::Time elapsed = clock.restart();

        scene.getWindow().clear();        
        scene.update(elapsed.asSeconds());
        scene.draw(elapsed.asSeconds());
        scene.getWindow().display();
    }

	return EXIT_SUCCESS;
}