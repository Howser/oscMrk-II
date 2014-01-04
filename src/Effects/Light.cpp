#include "Effects\Light.h"
#include <iostream>

Light::Light(sf::Color color, sf::Vector3f position, sf::Vector3f falloff)
	:
	color(color),
	position(position),
	falloff(falloff)
{
}

void passLightsToShader(sf::Shader* shader, std::vector<Light> const& lights)
{
	for (size_t i = 0; i < lights.size(); ++i)
	{
		std::string index = "[" + std::to_string(i) + "]";
		shader->setParameter("LightPos" + index, lights[i].position);
		shader->setParameter("LightColor" + index, lights[i].color);
		shader->setParameter("Falloff" + index, lights[i].falloff);
	}
	shader->setParameter("LightCount", lights.size());
}