#pragma once

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Rect.hpp>

#include <math.h>
#include <random>

namespace math{
	int random(int low, int high);

	///<summary>Get a random number between low, and high, but not exclude</summary>
	int random(int low, int high, std::vector<int> exclude);

	template <typename T>
	float distance(sf::Vector2<T> const& a, sf::Vector2<T> const& b);

	///<summary> Converts an angle in radians to degrees </summary>
	float toDegrees(float x);

	template <typename T>
	bool LineIntersectsLine(const sf::Vector2<T> & l1p1, const sf::Vector2<T> & l1p2, const sf::Vector2<T> & l2p1, const sf::Vector2<T> & l2p2);

	template <typename T>
	bool LineIntersectsRect(const sf::Vector2<T> &  p1, const sf::Vector2<T> &  p2, const sf::Rect<T> & r);
}

#include "GeneralMath.inl"