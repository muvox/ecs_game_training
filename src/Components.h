#pragma once

#include "Common.h"

class CTransform
{
public:
	sf::Vector2f pos		= { 0.0, 0.0 };
	sf::Vector2f velocity	= { 0.0, 0.0 };
	float angle 	= 0;

	CTransform(const sf::Vector2f & p, const sf::Vector2f & v, float a)
		:pos(p), velocity(v), angle(a) {}
};

class CShape
{
public:
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color & fill, const sf::Color & outline, float thickness)
		: circle(radius, points)
		{
			circle.setFillColor(fill);
			circle.setOutlineColor(outline);
			circle.setOutlineThickness(thickness);
			sf::Vector2f originVector = { radius, radius };
			circle.setOrigin(originVector);
		}
};

class CCollision
{
public:
	float radius = 0;
	CCollision(float r)
		: radius(r) {}
};

class CScore
{
public:
	int	score = 0;
	CScore(int s)
		: score(s) {}
};

// of frames remaining
class CLifespan
{
public:
	int remaining 		= 0; //
	int total			= 0; // initial lifespan
	CLifespan(int total)
		: remaining(total), total(total) {}
};

class CInput
{
public:
	bool up 	= false;
	bool left 	= false;
	bool right 	= false;
	bool down 	= false;
	bool shoot 	= false;

	CInput() {}
};