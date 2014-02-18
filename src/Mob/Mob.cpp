#include "Mob\Mob.h"

void Mob::stop(){
	path.clear();
	velocity.x = 0;
	velocity.y = 0;
}

void Mob::checkCollision(std::vector<std::vector<gen::Tile>>* map, sf::Vector2f & playerPosition){
	if (sf::IntRect(getPosition().x - width/2 - 3 + velocity.x, getPosition().y - height/2 - 3 + velocity.y, width + 3, height + 3).intersects(sf::IntRect(playerPosition.x - 16, playerPosition.y - 16, 32, 32)))
	{
		playerCollision = true;
	}else
	{
		playerCollision = false;
	}
	for (unsigned int x = (getPosition().x - width)/32 - 1, y = 0; x < (getPosition().x - width/2)/32 + 1; x++)
	{
		if (x > 0 && x < (*map).size() - 1)
		{
			bool broken = false;
			for (y = (getPosition().y - height)/32 - 1; y < (getPosition().y -height/2)/32 + 1; y++)
			{
				if (y > 0 && y < (*map)[x].size() - 1)
				{
					if ((*map)[x][y].type != 1)
					{
						if (sf::IntRect(getPosition().x - width/2 - 3 + velocity.x, getPosition().y - height/2 - 3 + velocity.y, width + 3, height + 3).intersects(sf::IntRect(x*32, y*32, 32, 32)))
						{
							worldCollision = true;
						}else
						{
							worldCollision = false;
							broken = true;
							break;
						}
						if (sf::IntRect(getPosition().x - width/2 + velocity.x, getPosition().y + velocity.y, width, height/2).intersects(sf::IntRect(x*32, y*32, 32, 32)))
						{
							if (getPosition().x - width/2 + velocity.x > (x*32 + 16))
							{
								//(32 - std::abs(getPosition().x - width/2 - x*32))
								setPosition(getPosition().x + 1, getPosition().y);
							}else if (getPosition().x - width/2 + velocity.x <= x*32 + 16)
							{
								//(32 - std::abs(getPosition().x - width/2 - x*32))
								setPosition(getPosition().x - 1, getPosition().y);
							}
							if (getPosition().y + velocity.y > y*32 + 16)
							{
								//(32 - std::abs(getPosition().y - height/2 - y*32))
								setPosition(getPosition().x, getPosition().y + 1);
							}else if(getPosition().y + velocity.y <= y*32 + 16)
							{
								//(32 - std::abs(getPosition().y - height/2 - y*32))
								setPosition(getPosition().x, getPosition().y - 1);
							}
						}
					}
					if (broken)
					{
						break;
					}
				}
				if (broken)
				{
					break;
				}
			}
		}
	}
}

void Mob::followPath(sf::Time& dt){
	if (!path.empty())
	{
		float dist = vec::length(getPosition() - (sf::Vector2f)path.back());
		if (dist < 17.f)
		{
			path.pop_back();
			if (path.empty())
			{
				timeSincePath = 0;
			}
		}

		if (!path.empty())
		{
			sf::Vector2f target = (sf::Vector2f)path.back() - getPosition();
			target = vec::normalize(target);
			velocity.x = target.x;
			velocity.y = target.y;
			velocity = vec::normalize(velocity) * GetSpeed(type) * dt.asSeconds();
		}
	}
	else
	{
		stop();
	}
}

bool Mob::IntersectsWall(sf::Rect<int> const& position, std::vector<std::vector<gen::Tile>> const& map){
	for (int x = position.left/32 - 1, y = position.top/32 - 1; x < (position.left + position.width)/32 + 2; x++)
	{
		if (x > 0 && x < map.size())
		{
			for (y = position.top/32 - 1; y < (position.top + position.height)/32 + 2; y++)
			{
				if (y > 0 && y < map[x].size())
				{
					if (map[x][y].type == 2)
					{
						if (position.intersects(sf::Rect<int>(x*32, y*32, 32, 32)))
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void Mob::StickArrow(const projectile::Arrow & arrow){
	sf::Sprite s_arrow;
	s_arrow = arrow.m_sprite;
	s_arrow.setRotation(arrow.m_sprite.getRotation());
	s_arrow.setPosition(sf::Vector2<float>(std::abs(getPosition().x - arrow.getPosition().x), std::abs(getPosition().y - arrow.getPosition().y)));
	m_arrows.push_back(s_arrow);
}

bool Mob::hasBuff(const Items & p_item){
	for (int i = 0; i < m_buffs.size(); i++)
	{
		if (m_buffs[i].m_buff == p_item)
		{
			return true;
		}
	}
	return false;
}

void Mob::UpdateAnimation(const sf::Vector2<float> & playerPosition){
	if (velocity.x < 0)
	{
		m_direction = Direction::Left;
	}else if (velocity.x > 0)
	{
		m_direction = Direction::Right;
	}
	if (velocity.y < 0 && std::abs(velocity.y) > std::abs(velocity.x))
	{
		m_direction = Direction::Up;
	}else if (velocity.y > 0 && std::abs(velocity.y) > std::abs(velocity.x))
	{
		m_direction = Direction::Down;
	}
	if (aggro)
	{
		if (std::abs(velocity.x) + std::abs(velocity.y) < GetSpeed(type))
		{
			float angle = math::toDegrees(std::atan2f(playerPosition.y - getPosition().y, playerPosition.x - getPosition().x));
			if (angle > 215 && angle <= 45)
			{
				m_direction = Direction::Up;
			}else if (angle > 45 && angle <= 135)
			{
				m_direction = Direction::Right;
			}else if (angle > 135 && angle <= 225)
			{
				m_direction = Direction::Down;
			}else if (angle > 225 && angle <= 315)
			{
				m_direction = Direction::Left;
			}
		}
	}
	switch (m_direction)
	{
	case Mob::Up:
		m_animation.loop(0);
		break;
	case Mob::Down:
		m_animation.loop(2);
		break;
	case Mob::Left:
		m_animation.loop(3);
		break;
	case Mob::Right:
		m_animation.loop(1);
		break;
	default:
		break;
	}
	m_animation.update();
	sprite.setTextureRect(m_animation.getFrame());
}