#pragma once

#include "GeneralMath.h"

template <typename T>
float math::distance(sf::Vector2<T> const& lhs, sf::Vector2<T> const& rhs){
	return (float)std::sqrt(std::pow(std::abs(rhs.x - lhs.x), 2) + std::pow(std::abs(lhs.y - rhs.y), 2));
}