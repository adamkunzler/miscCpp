#pragma once

#include <SFML/Graphics.hpp>

class EllipseShape : public sf::Shape
{
public:

    explicit EllipseShape(const sf::Vector2f& radius = sf::Vector2f(0.f, 0.f), const size_t numPoints = 30) :
        _radius(radius), _numPoints{ numPoints }
    {
        update();
    }

    void setRadius(const sf::Vector2f& radius)
    {
        _radius = radius;
        update();
    }
    
    const sf::Vector2f& getRadius() const
    {
        return _radius;
    }

    virtual std::size_t getPointCount() const
    {
        return _numPoints; // fixed, but could be an attribute of the class if needed
    }

    virtual sf::Vector2f getPoint(std::size_t index) const
    {
        static const float pi = 3.141592654f;

        float angle = index * 2 * pi / getPointCount() - pi / 2;
        float x = std::cos(angle) * _radius.x;
        float y = std::sin(angle) * _radius.y;

        return sf::Vector2f(_radius.x + x, _radius.y + y);
    }

private:

    sf::Vector2f _radius;
    std::size_t _numPoints;
};