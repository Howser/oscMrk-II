#pragma once
#include <SFML\Graphics\Color.hpp>
#include <SFML\System\Vector3.hpp>
#include <SFML\Graphics\Shader.hpp>
#include <SFML\Graphics\View.hpp>
#include <SFML\System\Time.hpp>
#include <vector>
#include "Math\GeneralMath.h"

struct Light
{
	///<summary>if(screen){Don't update, m_draw = true, position = screen coords}
	Light(sf::Color color, sf::Vector3f position, sf::Vector3f falloff, bool p_screen);

	sf::Color color;
	sf::Vector3f position;
	sf::Vector3f falloff;
	sf::Vector2f* ptr_follow;

	bool m_draw, m_screen;
	bool erase; //remove it from m_lights in the manager
	
	void update(sf::Time & p_dt);

private:
	float m_alpha;
	bool m_add, m_elapsed;
	void flicker(sf::Time & p_dt);
};

void passLightsToShader(sf::Shader* shader, std::vector<Light>* ptr_lights, sf::View* ptr_view);