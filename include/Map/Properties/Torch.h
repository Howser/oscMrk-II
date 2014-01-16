#pragma once
#include <SFML\Graphics\Color.hpp>
#include <SFML\System\Vector3.hpp>
#include <SFML\Graphics\View.hpp>
#include "Effects\Lights\Light.h"

struct Torch{
	Torch(sf::Vector3f & p_position, sf::Color & p_color, float p_radius);
	~Torch();
	void update(sf::View* ptr_view);
	void draw();

	Light m_light;
	sf::Vector3<float> m_position;
	float m_radius;
	bool m_draw;
};