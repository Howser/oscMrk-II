#include "Effects\Lights\Light.h"

Light::Light(sf::Color color, sf::Vector3f position, sf::Vector3f falloff, bool p_screen)
	:
	color(color),
	position(position),
	falloff(falloff),
	m_draw(true),
	m_add(true),
	m_alpha((color.a > 0) ? color.a:255.f),
	m_elapsed(0.f),
	m_screen(p_screen)
{
}

void Light::update(sf::Time & p_dt){
	flicker(p_dt);
}

void Light::flicker(sf::Time & p_dt){
	m_elapsed += p_dt.asSeconds()/10;
	if (m_elapsed >= math::random(1, 8)/10)
	{
		if (m_add)
		{
			int i = math::random(10, 50);
			if (color.a + i < m_alpha + color.a / 3 && color.a + i <= 255)
			{
				color.a += i;
			}
			m_add = false;
		}else
		{
			int i = math::random(10, 50);
			if (color.a - i > m_alpha - color.a / 3 && color.a - i >= 0)
			{
				color.a -= i;
			}
			m_add = true;
		}
		m_elapsed = 0.f;
	}
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