#pragma once

#include <SFML\System\Vector2.hpp>
#include <math.h>

namespace vec
{
	///<summary>
	///Returns the length of vector a
	///</summary>
	template <typename T>
	float length(sf::Vector2<T> const& a);

	///<summary>
	///Returns normalized vector of a
	///</summary>
	template <typename T>
	sf::Vector2<T> normalize(sf::Vector2<T> a);

	///<summary>
	///Returns the angle, in degrees, of vector a
	///</summary>
	template <typename T>
	float angle(sf::Vector2<T> a);

}

#include "VectorMath.inl"