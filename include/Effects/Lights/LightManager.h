#pragma once
#include "Light.h"
#include <vector>
#include <SFML\Graphics\View.hpp>

class LightManager{
public:
	std::vector<Light> m_lights;
	void update(sf::View* ptr_view);
	Light* add(Light & p_light);
};