#include "Item\Arrow.h"

projectile::Arrow::Arrow(std::vector<std::vector<gen::Tile>>* ptr_tiles, float angle, sf::Sprite & p_sprite, int p_damage) : p_tiles(ptr_tiles), m_sprite(p_sprite), dead(false), m_damage(p_damage){
	m_velocity = sf::Vector2f(std::cos(angle)*ARROW_SPEED, std::sin(angle)*ARROW_SPEED);
	m_sprite.setOrigin(15, 1);
	m_sprite.setRotation(angle*180/3.14);
}
projectile::Arrow::~Arrow(){}

void projectile::Arrow::update(){
	for (unsigned int x = getPosition().x/32 - 1, y = getPosition().y/32  - 1; x < getPosition().x/32 + 1; x++)
	{
		for (y = getPosition().y/32  - 1; y < getPosition().y/32 + 1; y++)
		{
			if (x >= 0 && x < (*p_tiles).size() && y >= 0 && y < (*p_tiles)[0].size())
			{
				if ((*p_tiles)[x][y].type == 2)
				{
					if (sf::Rect<int>((int)getPosition().x, (int)getPosition().y, 1, 1).intersects(sf::Rect<int>(x*32, y*32, 32, 32)))
					{
						m_velocity = sf::Vector2<float>(0, 0);
					}
				}
			}else
			{
				dead = true;
			}
		}
	}
	move(m_velocity);
}

void projectile::Arrow::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	states.transform = getTransform();
	target.draw(m_sprite, states);
}