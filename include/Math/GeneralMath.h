#pragma once

#include <SFML\System\Vector2.hpp>
#include <math.h>
#include <random>

namespace math{
	int random(int low, int high);

	///<summary>Get a random number between low, and high, but not exclude</summary>
	int random(int low, int high, int exclude);

	template <typename T>
	float distance(sf::Vector2<T> const& lhs, sf::Vector2<T> const& rhs);

	///<summary> Converts an angle in radians to degrees </summary>
	float toDegrees(float x);
}

#include "GeneralMath.inl"