#include "Mob\MinorMob.h"
MinorMob::MinorMob(TYPE type, TextureHolder* textureHolder, std::vector<DeadMob>* p_deadMobs){
	startPos = (sf::Vector2<int>)getPosition();
	/*speed = GetSpeed(type);
	attackSpeed = GetAttackSpeed(type);
	damage = GetDamage(type);
	health = GetHp(type);
	aggroDist = GetAggroDist(type);*/
	width = GetWidth(type);
	height = GetHeight(type);
	setOrigin(GetWidth(type)/2, GetHeight(type)/2);
	inventory = Inventory(10, 10, 10, *textureHolder);
	this->p_deadMobs = p_deadMobs;
	this->type = type;
	this->textureHolder = textureHolder;
	switch (type)
	{
	case TYPE::test:
		sprite.setTexture(*textureHolder->getTexture(Textures::TestMob));
		break;
	case TYPE::special:
		sprite.setTexture(*textureHolder->getTexture(Textures::TestMob));
		break;
	}
	timeSincePath = 0.f;
	worldCollision = false;
	dead = false;
	aggro = false;
}

MinorMob::MinorMob(Mob const& mob, std::vector<DeadMob>* p_deadMobs){
	type = mob.type;
	ID = mob.ID;
	setPosition(mob.getPosition());
	startPos = (sf::Vector2<int>)getPosition();
	/*speed = GetSpeed(type);
	attackSpeed = GetAttackSpeed(type);
	damage = GetDamage(type);
	aggroDist = GetAggroDist(type);*/
	health = GetHp(type);
	width = GetWidth(type);
	height = GetHeight(type);
	setOrigin(width/2, height/2);
	inventory = Inventory(10, 10, 10, *mob.textureHolder);
	textureHolder = mob.textureHolder;
	this->p_deadMobs = p_deadMobs;
	switch (type)
	{
	case TYPE::test:
		sprite.setTexture(*textureHolder->getTexture(Textures::TestMob));
		break;
	case TYPE::special:
		sprite.setTexture(*textureHolder->getTexture(Textures::TestMob));
		break;
	}
	timeSincePath = 0.f;
	worldCollision = false;
	dead = false;
	aggro = false;
}

void MinorMob::update(std::vector<std::vector<gen::Tile>>* map, sf::Time& deltaTime, sf::Vector2f & playerPosition){
	if (health <= 0)
	{
		die();
	}else
	{
		if (aggro)
		{
			if (updatePath > 0)
			{
				updatePath -= deltaTime.asSeconds();
			}
		}
		if (!aggro && path.empty())
		{
			timeSincePath += deltaTime.asSeconds();
		}
		checkCollision(map, playerPosition);
		if (!playerCollision)
		{
			followPath(deltaTime);
			if (!IntersectsWall(sf::Rect<int>(getPosition().x - width/2 + velocity.x, getPosition().y - height/2 + velocity.y, width, height), *map))
			{
				move(velocity);
			}
			for (int i = 0; i < m_arrows.size(); i++)
			{
				m_arrows[i].move(velocity);
			}
		}
	}
}

void MinorMob::draw(sf::RenderTarget & target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(sprite, states);
	for (int i = 0; i < m_arrows.size(); i++)
	{
		states.transform = m_arrows[i].getTransform();
		target.draw(m_arrows[i], states);
	}
}

void MinorMob::takeDamage(int damage){
	health -= damage;
	if (health <= 0)
	{
		die();
	}else
	{
		aggro = true;
	}
}

void MinorMob::dealDamage(int* health){
	*health = GetDamage(type) - (std::abs(GetDamage(type) - *health) > 0 ? 0:std::abs(GetDamage(type) - *health));
}

void MinorMob::die(){
	dead = true;
	sprite.setTexture(*textureHolder->getTexture(Textures::TestMobDead));
	for (int i = 0; i < lootTable.lootItems.size(); i++)        
	{
		switch (lootTable.lootItems[i].item)
		{
		default:
			if (lootTable.lootItems[i].c >= math::random(1, 100))
			{
				int q = math::random(lootTable.lootItems[i].minQ, lootTable.lootItems[i].maxQ);
				for (int Q = 0; Q < q; Q++)
				{
					inventory.slots[inventory.GetFirstAvailableSlot(lootTable.lootItems[i].item).x][inventory.GetFirstAvailableSlot(lootTable.lootItems[i].item).y].Items.push_back(GearItem(lootTable.lootItems[i].item, *textureHolder, ID));
				}
			}
			break;
		}
	}
	p_deadMobs->push_back(DeadMob(&sprite, sf::Vector2<float>(getPosition().x - width/2, getPosition().y - height/2), &type));
}

bool MinorMob::operator ==(MinorMob const& rhs){
	return (ID == rhs.ID);
}

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
						}
						if (sf::IntRect(getPosition().x - width/2 + 2, getPosition().y - height/2 + 2, width - 4, height - 4).intersects(sf::IntRect(x*32, y*32, 32, 32)))
						{
							//int d_up, d_down, d_left, d_right;
							//d_up = (getPosition().y - height/2) - (y*32);
							//d_down = (y*32 + 32) - (getPosition().y + height/2);
							//d_left = (getPosition().x - width/2) - (x*32);
							//d_right = (x*32 + 32) - (getPosition().x + width/2);
							//if (d_up > 0)
							//{
							//	if (d_up > d_left && d_up > d_right)
							//	{
							//		//move down
							//		setPosition(getPosition().x, getPosition().y + std::abs(d_down));
							//		return;
							//	}
							//}
							//if (d_down > 0)
							//{
							//	if (d_down > d_left && d_down > d_right)
							//	{
							//		//move up
							//		setPosition(getPosition().x, getPosition().y - std::abs(d_up));
							//		return;
							//	}
							//}
							//if (d_left > 0)
							//{
							//	if (d_left > d_up && d_left > d_down)
							//	{
							//		//move right
							//		setPosition(getPosition().x + std::abs(d_right), getPosition().y);
							//		return;
							//	}
							//}
							//if (d_right > 0)
							//{
							//	if (d_right > d_up && d_right > d_down)
							//	{
							//		//move left
							//		setPosition(getPosition().x - std::abs(d_left), getPosition().y);
							//		return;
							//	}
							//}
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
			velocity.x = target.x * GetSpeed(type) * dt.asSeconds();
			velocity.y = target.y * GetSpeed(type) * dt.asSeconds();
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