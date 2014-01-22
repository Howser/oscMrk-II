#pragma once

#include "GeneralMath.h"

template <typename T>
float math::distance(sf::Vector2<T> const& lhs, sf::Vector2<T> const& rhs){
	return (float)std::sqrt(std::pow(std::abs(rhs.x - lhs.x), 2) + std::pow(std::abs(lhs.y - rhs.y), 2));
}

template <typename T>
sf::Rect<T> math::intersection(const sf::Rect<T> & p_a, const sf::Rect<T> p_b){
	sf::Rect<T> rect;
	rect.left = (p_a.left > p_b.left) ? p_a.left + p_a.left - p_b.left:p_b.left + p_b.left - p_a.left;
	rect.top = (p_a.top > p_b.top) ? p_a.top + p_a.top - p_b.top:p_b.top + p_b.top - p_a.top;
	rect.width = std::abs(std::abs((p_a.left + p_a.width) - p_b.left));
	rect.height = std::abs(std::abs((p_a.top + p_a.height) - p_b.top));
	return rect;
}