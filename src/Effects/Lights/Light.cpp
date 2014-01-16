#include "Effects\Lights\Light.h"

Light::Light(sf::Color color, sf::Vector3f position, sf::Vector3f falloff, bool p_screen)
	:
	color(color),
	position(position),
	falloff(falloff),
	m_draw(true),
	m_screen(p_screen)
{
}

void passLightsToShader(sf::Shader* shader, std::vector<Light> const& lights, sf::View* ptr_view)
{
	for (int i = 0; i < lights.size(); i++)
	{
		std::string index = "[" + std::to_string(i) + "]";
		if (lights[i].m_draw)
		{
			if (!lights[i].m_screen)
			{
				shader->setParameter("LightPos" + index, sf::Vector3f((lights[i].position.x - (ptr_view->getCenter().x - 640))/1280, 1 - (lights[i].position.y - (ptr_view->getCenter().y - 360))/720, lights[i].position.z));
			}else
			{
				shader->setParameter("LightPos" + index, sf::Vector3f(lights[i].position.x, lights[i].position.y, lights[i].position.z));
			}
			shader->setParameter("LightColor" + index, lights[i].color);
			shader->setParameter("Falloff" + index, lights[i].falloff);
		}else
		{
			shader->setParameter("LightColor" + index, sf::Color::Transparent);
		}
	}
	shader->setParameter("LightCount", lights.size());
}