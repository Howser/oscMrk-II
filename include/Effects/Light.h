#pragma once

#include <SFML\Graphics\Color.hpp>
#include <SFML\System\Vector3.hpp>
#include <SFML\Graphics\Shader.hpp>

#include <vector>

struct Light
{
					Light(sf::Color color, sf::Vector3f position, sf::Vector3f falloff);

	sf::Color		color;
	sf::Vector3f	position;
	sf::Vector3f	falloff;
};

// A very nonmodular function (might refactor to World class when that get's implemented)
void passLightsToShader(sf::Shader* shader, std::vector<Light> const& lights);