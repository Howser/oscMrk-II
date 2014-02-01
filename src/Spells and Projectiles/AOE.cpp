#include "Spells and Projectiles\AOE.h"

AOE::AOE(){
	m_radius = 0.f;
	m_dead = true;
	setPosition(0, 0);
}

AOE::AOE(Items & p_spell_type, float & p_radius, float & p_timer, sf::Vector2<int> & p_position) : m_spell_type(p_spell_type), m_radius(p_radius), m_timer(p_timer), m_dead(false){
	setOrigin(m_radius, m_radius);
	setPosition(p_position.x, p_position.y);
}

AOE::~AOE(){}

void AOE::update(sf::Time & p_dt){
	if (!m_dead)
	{
		m_timer -= p_dt.asSeconds();
		if (m_timer <= 0)
		{
			m_dead = true;
			return;
		}
	}
}

void AOE::draw(sf::RenderTarget & p_target, sf::RenderStates p_states)const{
	p_states.transform *= getTransform();
	p_target.draw(m_sprite, p_states);
}