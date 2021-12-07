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
    //Scene scene{ 1024, 1024, "Ellipses" };
    Scene scene{ 2048, 2048, "Ellipses" };

    sf::Clock clock;
    bool isStarted = false;
    while (scene.getWindow().isOpen())
    {
        sf::Event event;
        while (scene.getWindow().pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:            
                scene.getWindow().close();
                break;
            case sf::Event::KeyReleased:
                switch(event.key.code)
                {
                case sf::Keyboard::Space:
                    scene.restart();
                    break;
                case sf::Keyboard::E:
                    scene.toggleEllipse();
                    break;
                }
                break;
            }            
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {

            sf::Vector2i mousePosition = sf::Mouse::getPosition(scene.getWindow());
            sf::Vector2u winPosition = scene.getWindow().getSize();
            scene.addParticle1(mousePosition.x - winPosition.x / 2.0f, mousePosition.y - winPosition.y / 2.0f);
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {

            sf::Vector2i mousePosition = sf::Mouse::getPosition(scene.getWindow());
            sf::Vector2u winPosition = scene.getWindow().getSize();
            scene.addParticle2(mousePosition.x - winPosition.x / 2.0f, mousePosition.y - winPosition.y / 2.0f);
        }
        
        sf::Time elapsed = clock.restart();
        
        scene.getWindow().clear();        
        float fElapsed = (float)elapsed.asMicroseconds() / 10000.0f;
        scene.update(fElapsed);
        scene.draw(fElapsed);
        scene.getWindow().display();
    }

	return EXIT_SUCCESS;
}