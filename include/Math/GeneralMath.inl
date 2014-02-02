#pragma once

#include "GeneralMath.h"

template <typename T>
float math::distance(sf::Vector2<T> const& a, sf::Vector2<T> const& b){
	sf::Vector2<T> C = sf::Vector2<T>(b.x, a.y);
	return (float)std::sqrt((((C.x - a.x) * (C.x - a.x)) + ((C.y - b.y) * (C.y - b.y))));
}