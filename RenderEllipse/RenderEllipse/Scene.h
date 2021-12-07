#pragma once

#include "EllipseShape.h"

class BaseScene;

struct Ellipse
{
	float h; // origin.x
	float k; // origin.y
	float a; // major-axis radius
	float b; // minor-axis radius
	float c; // foci.x
	float f1; // focus left
	float f2; // focus right
};

struct Particle
{
	sf::Vector2f position;
	sf::Vector2f oldPosition;
	sf::Vector2f velocity;
	float radius;	
	sf::CircleShape gfx;
};

struct Line
{
	float m;
	float c;
};

//struct BouncingParticle
//{
//	Particle particle;	
//	
//};

class Scene : public BaseScene
{
private:
	//sf::Text txtInfo;
	sf::RenderTexture _texture;
	sf::Sprite _sprite;

	Ellipse _e;
	std::vector<Particle> _particles{ 2 };
		
	sf::VertexArray _ellipse;
	sf::CircleShape _f1;
	sf::CircleShape _f2;
	
	float _speed{ 20.0f };
	float _radius{ 1.0f };	

	RandomGenerator rand;
	
	const float PI{ 3.1415926f };
	const size_t NumEllipsePoints{ 360 };
	const float DistanceThreshold = 100.0f;
		
	bool isShowTails{ true };
	bool isShowEllipse{ false };

public:
	Scene(int width, int height, std::string title)
		:BaseScene{ width, height, title }
	{
		restart();

		addParticle1(-50.0f, -50.0f);
		addParticle2(50.0f, 50.0f);
	}
		
	void restart()
	{
		if (!_texture.create(width, height))
		{
			std::cout << "Failed to create texture!" << std::endl;
			return;
		}				
		_texture.clear();

		// define the Ellipse
		_e.h = 0.0f;
		_e.k = 0.0f;
		//_e.a = 500.0f;
		//_e.b = 300.0f;
		_e.a = 1000.0f;
		_e.b = 700.0f;
		_e.c = std::sqrtf(_e.a * _e.a - _e.b * _e.b);
		_e.f1 = -_e.c;
		_e.f2 = _e.c;
				
		// initialize rendering structures
		_ellipse = sf::VertexArray{ sf::LinesStrip, NumEllipsePoints + 1 };
		drawEllipse();

		_f1 = sf::CircleShape{ 3.0f };
		_f1.setPosition(sf::Vector2f{ -_e.c + halfWidth, halfHeight });
		_f1.setOrigin(1.5f, 1.5f);

		_f2 = sf::CircleShape{ 3.0f };
		_f2.setPosition(sf::Vector2f{ _e.c + halfWidth, halfHeight });
		_f2.setOrigin(1.5f, 1.5f);			
	}

	void toggleEllipse()
	{
		isShowEllipse = !isShowEllipse;
	}

	void addParticle1(const float initX, const float initY)
	{
		_particles[0] = initParticle(initX, initY, 0);
	}

	void addParticle2(const float initX, const float initY)
	{
		_particles[1] = initParticle(initX, initY, 1);
	}

	Particle initParticle(const float initX, const float initY, const size_t index)
	{
		Particle p;

		// define the Particle
		p.position = sf::Vector2f{ initX, initY };
		p.velocity = sf::Vector2f{ (float)rand.getUniform(-1.0f, 1.0f), (float)rand.getUniform(-1.0f, 1.0f) };
		p.velocity = normalize(p.velocity) * _speed;
		p.radius = _radius;
		p.oldPosition = sf::Vector2f{ -1.0f, -1.0f };

		sf::Color c1{ 139,69,19 };
		sf::Color c2{ 100,149,237 };
		p.gfx = sf::CircleShape{ p.radius };
		p.gfx.setOrigin(p.radius / 2.0f, p.radius / 2.0f);
		p.gfx.setFillColor(index == 0 ? c1 : c2);

		return p;
	}
	
	float distance(sf::Vector2f a, sf::Vector2f b)
	{
		if (b.x == -1.0f && b.y == -1.0f) return DistanceThreshold + 1.0f;

		float dx = a.x - b.x;
		float dy = a.y - b.y;
		return std::sqrtf(dx * dx + dy * dy);
	}

	float checkEllipseCollision(const Ellipse& e, const sf::Vector2f& p)
	{
		float result = ((p.x * p.x) / (e.a * e.a)) + ((p.y * p.y) / (e.b * e.b));
		return result;
	}	

	void updateScene(float elapsed) override
	{		
		for(auto& p : _particles)
		{			
			// move the particle
			p.position += p.velocity * elapsed;

			// check if the current position is far enough away from last position
			// before trying to check for bouncing off the ellipse
			float dist = distance(p.position, p.oldPosition);
			if (dist < DistanceThreshold) continue;

			// check if there was a collision with the ellipse
			float result = checkEllipseCollision(_e, p.position);
			if (result < 1.0f) continue; // MIGHT NEED TO CHANGE TO <= 1.0f
						
			// calculate tangent and perpendicular angle to the tangent
			float tangent = -1 * (_e.b * _e.b * p.position.x) / (_e.a * _e.a * p.position.y);						
			float perpAng = std::atanf(-(1.0f / tangent));
						
			// reflect velocity around perpendicular
			sf::Vector2f r = reflect(normalize(p.velocity), normalize(sf::Vector2f{ std::cosf(perpAng), std::sinf(perpAng) }));
			p.velocity = normalize(r) * _speed;
									
			// copy current particle position into old position
			p.oldPosition = p.position;
		}
	}	

	void drawScene() override
	{	
		if(!isShowTails) 
			_texture.clear();
		
		// render ellipse and foci
		if (isShowEllipse)
		{
			_texture.draw(_ellipse);
			_texture.draw(_f1);
			_texture.draw(_f2);
		}
		
		// render particles
		for (auto& p : _particles)
		{
			p.gfx.setPosition(sf::Vector2f{ p.position.x + halfWidth, p.position.y + halfHeight });
			_texture.draw(p.gfx);
		}

		_texture.display();
		
		_sprite.setTexture(_texture.getTexture());		

		window.draw(_sprite);
	}	

	void drawEllipse()
	{
		for (size_t index = 0; index < NumEllipsePoints; index++)
		{
			sf::Vector2f point = calculateEllipsePoint(index);
			_ellipse[index].position = sf::Vector2f(point.x + halfWidth, point.y + halfHeight);
			_ellipse[index].color = sf::Color::White;
		}

		// make the last point same as first (to close the ellipse)
		sf::Vector2f point = calculateEllipsePoint(0);
		_ellipse[NumEllipsePoints].position = sf::Vector2f(point.x + halfWidth, point.y + halfHeight);
		_ellipse[NumEllipsePoints].color = sf::Color::White;
	}

	sf::Vector2f calculateEllipsePoint(size_t index)
	{
		float angle = index * (2.0f * PI) / NumEllipsePoints - (PI / 2.0f);
		float x = std::cos(angle) * _e.a;
		float y = std::sin(angle) * _e.b;

		return sf::Vector2f(x, y);
	}

	sf::Vector2f normalize(const sf::Vector2f& source)
	{
		float length = std::sqrtf((source.x * source.x) + (source.y * source.y));
		if (length != 0)
			return sf::Vector2f(source.x / length, source.y / length);
		else
			return source;
	}

	float dot(const sf::Vector2f& lhs, const sf::Vector2f& rhs) const
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	sf::Vector2f reflect(const sf::Vector2f& lhs, const sf::Vector2f& rhs)
	{
		sf::Vector2 a{ lhs };
		sf::Vector2 b{ rhs };

		return a - b * 2.0f * dot(a, b);
	}
};