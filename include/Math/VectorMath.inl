#pragma once

#include "VectorMath.h"
#include "GeneralMath.h"

template <typename T>
float vec::length(sf::Vector2<T> const& a)
{
	return sqrtf((a.x * a.x) + (a.y * a.y));
}

template <typename T>
sf::Vector2<T> vec::normalize(sf::Vector2<T> a)
{
	float length = vec::length(a);
	return sf::Vector2<T>(a.x / length, a.y / length);
}

template <typename T>
float vec::angle(sf::Vector2<T> a)
{
	float angle = math::toDegrees(atan2f(a.x, -a.y));
	if (angle < 0 && angle > -180)
	{
		angle += 360;
	}
	return angle;
}