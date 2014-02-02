#include "Spells and Projectiles\AOE.h"

AOE::AOE(){
	m_radius = 0.f;
	m_dead = true;
	setPosition(0, 0);
}

AOE::AOE(Items p_spell_type, float p_radius, float p_timer, const sf::Vector2f & p_position, mobtree::Tree* ptr_tree) : m_spell_type(p_spell_type), m_radius(p_radius), m_timer(p_timer), m_dead(false){
	setOrigin(m_radius, m_radius);
	setPosition(p_position.x, p_position.y);

	///<summary>An array of points to outline the border of the AOE spell in order to get the correct mob tree branches to search.</summary>
	sf::Vector2<int> points[2] = {sf::Vector2<int>(getPosition().x - m_radius, getPosition().y - m_radius), sf::Vector2<int>(getPosition().x + m_radius, getPosition().y + m_radius)};

	for (int x = points[0].x/mobtree::SIZE, y = points[0].y/mobtree::SIZE; x < points[1].x/mobtree::SIZE; x++)
	{
		for (int y = points[0].y/mobtree::SIZE; y < points[1].y/mobtree::SIZE; y++)
		{
			mobtree::Branch* ptr_branch = ptr_tree->search(sf::Vector2f(x*mobtree::SIZE, y*mobtree::SIZE));
			if (ptr_branch != NULL)
			{
				m_branches.push_back(ptr_branch);
			}
		}
	}
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