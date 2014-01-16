#pragma once

#include "SFML\Graphics\Transformable.hpp"
#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\Color.hpp"

struct Particle : public sf::Transformable {
	sf::Vector2f velocity;
	float lifetime;
	sf::Color color;
	bool dead;
};