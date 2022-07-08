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

};