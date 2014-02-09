#include "Spells and Projectiles\Spell.h"

using namespace projectile;

Spell::Spell(const sf::Vector2f & p_position, float angle, const Items & p_item, sf::Sprite & p_sprite, int p_damage, std::vector<std::vector<gen::Tile>>* ptr_tiles, Light & p_light)
	: m_light(p_light), m_position(p_position){
		m_light.ptr_follow = &m_position;
		setPosition(p_position);
		m_velocity = sf::Vector2f(std::cos(angle)*SPEED, std::sin(angle)*SPEED);
		m_item = p_item;
		m_sprite = p_sprite;
		m_sprite.setOrigin(24, 1);
		m_sprite.setRotation(angle*180/3.14);
		m_damage = p_damage;
		p_tiles = ptr_tiles;
		dead = false;
		m_damage_player = false;
}

Spell::~Spell(){

}

void Spell::update(sf::Time & p_dt){
	switch (m_item)
	{
	case TestSpell:
		for (unsigned int x = getPosition().x/32 - 1, y = getPosition().y/32 - 1; x < getPosition().x/32 + 1; x++)
		{
			for (y = getPosition().y/32 - 1; y < getPosition().y/32 + 1; y++)
			{
				if (x >= 0 && x < (*p_tiles).size() && y >= 0 && y < (*p_tiles)[0].size())
				{
					if ((*p_tiles)[x][y].type != 1)
					{
						if (sf::Rect<int>((int)getPosition().x, (int)getPosition().y, 1, 1).intersects(sf::Rect<int>(x*32, y*32, 32, 32)))
						{
							kill();
						}
					}
				}else
				{
					kill();
				}
			}
		}
		if (!dead)
		{
			move(m_velocity);
			m_position = getPosition();
			m_light.position.x = getPosition().x;
			m_light.position.y = getPosition().y;
		}
		break;
	default:
		break;
	}
}

void Spell::kill(){
	dead = true;
	m_velocity.x = 0;
	m_velocity.y = 0;
	//m_light.erase = true;
}

void Spell::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	states.transform = getTransform();
	target.draw(m_sprite, states);
}