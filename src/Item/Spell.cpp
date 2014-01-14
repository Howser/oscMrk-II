#include "Item\Spell.h"

using namespace projectile;

Spell::Spell(std::vector<std::vector<gen::Tile>>* ptr_tiles, float angle, sf::Sprite & p_sprite, int p_damage, const Items & p_item){
	m_velocity = sf::Vector2f(std::cos(angle)*SPEED, std::sin(angle)*SPEED);
	m_sprite = p_sprite;
	m_sprite.setOrigin(24, 1);
	m_sprite.setRotation(angle*180/3.14);
	p_tiles = ptr_tiles;
	dead = false;
	m_damage = p_damage;
	m_item = p_item;
}
Spell::~Spell(){}

void Spell::update(){
	switch (m_item)
	{
	case TestSpell:
		for (unsigned int x = getPosition().x/32 - 1, y = getPosition().y/32 - 1; x < getPosition().x/32 + 1; x++)
		{
			for (y = getPosition().y/32 - 1; y < getPosition().y/32 + 1; y++)
			{
				if (x >= 0 && x < (*p_tiles).size() && y >= 0 && y < (*p_tiles)[0].size())
				{
					if ((*p_tiles)[x][y].type == 2)
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
		move(m_velocity);
		break;
	default:
		break;
	}
}

void Spell::kill(){
	dead = true;
	m_velocity.x = 0;
	m_velocity.y = 0;
}

void Spell::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	states.transform = getTransform();
	target.draw(m_sprite, states);
}