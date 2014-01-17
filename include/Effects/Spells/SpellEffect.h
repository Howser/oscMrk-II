#pragma once
#include "Effects\Lights\Light.h"
#include "Effects\Particles\Emitter.h"
#include <SFML\System\Vector2.hpp>
struct SpellEffect{
	SpellEffect(Light* ptr_light, Emitter & p_emitter, sf::Vector2<float>* ptr_position);
	SpellEffect();
	~SpellEffect();
	sf::Vector2<float>* ptr_position;
	Light* ptr_light;
	Emitter m_emitter;

	void update(sf::Time & p_dt);
};