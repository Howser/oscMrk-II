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
	inventory = Inventory(10, 10, 0, 0, *textureHolder);
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
	m_attack_timer = GetAttackSpeed(type);
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
	inventory = Inventory(10, 10, 0, 0, *mob.textureHolder);
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
	m_attack_timer = GetAttackSpeed(type);
	worldCollision = false;
	dead = false;
	aggro = false;
}

void MinorMob::update(std::vector<std::vector<gen::Tile>>* map, sf::Time& deltaTime, sf::Vector2f & playerPosition, int* p_health, Mob* ptr_mob, std::vector<projectile::Spell>* ptr_spells, std::vector<projectile::Arrow>* ptr_arrows){
	switch (type)
	{
	case test:
		Skeleton::update(map, deltaTime, playerPosition, p_health, ptr_mob, ptr_spells, ptr_arrows);
		break;
	case special:
		break;
	default:
		std::cout << "\033[0;31m" << "!!!ERROR!!!: TYPE NOT SUPPORTED\n";
		break;
	}

	for (int i = 0; i < m_buffs.size(); i++)
	{
		if (m_buffs[i].m_duration > 0)
		{
			m_buffs[i].update(deltaTime);
		}else
		{
			m_buffs.erase(m_buffs.begin() + i);
		}
	}
}

void MinorMob::draw(sf::RenderTarget & target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(sprite, states);
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
	*health -= GetDamage(type);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Mob type specific functions////Mob type specific functions////Mob type specific functions////Mob type specific functions////Mob type specific functions////Mob type specific functions////Mob type specific functions////Mob type specific functions//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Skeleton::attack(const sf::Vector2<float>& p_player_position, std::vector<projectile::Arrow>* ptr_arrows, Mob* ptr_mob, std::vector<std::vector<gen::Tile>>* ptr_tiles){
	float angle = std::atan2f(p_player_position.y - ptr_mob->getPosition().y, p_player_position.x - ptr_mob->getPosition().x);
	sf::Sprite arrow_sprite;
	arrow_sprite.setTexture(*ptr_mob->textureHolder->getTexture(Textures::d_Arrow));
	projectile::Arrow arrow = projectile::Arrow(ptr_tiles, angle, arrow_sprite, GetDamage(Items::Bow), Items::Arrow, NULL);
	arrow.setPosition(ptr_mob->getPosition());
	arrow.m_damage_player = true;
	ptr_arrows->push_back(arrow);
}

void Skeleton::update(std::vector<std::vector<gen::Tile>>* ptr_map, sf::Time& p_dt, sf::Vector2<float>& p_player_position, int* ptr_health, Mob* ptr_mob, std::vector<projectile::Spell>* ptr_spells, std::vector<projectile::Arrow>* ptr_arrows){
	if (ptr_mob->health <= 0)
	{
		ptr_mob->die();
	}else
	{
		if (ptr_mob->aggro)
		{
			if (ptr_mob->updatePath > 0)
			{
				ptr_mob->updatePath -= p_dt.asSeconds();
			}
			if (math::distance(ptr_mob->getPosition(), p_player_position) <= GetAtackDistance(ptr_mob->type))
			{
				if (ptr_mob->m_attack_timer > 0)
				{
					ptr_mob->m_attack_timer -= p_dt.asSeconds();
				}else
				{
					ptr_mob->m_attack_timer = GetAttackSpeed(ptr_mob->type);
				}
			}
		}
		if (!ptr_mob->aggro && ptr_mob->path.empty())
		{
			ptr_mob->timeSincePath += p_dt.asSeconds();
		}
		ptr_mob->checkCollision(ptr_map, p_player_position);
		//if (!playerCollision)
		{
			if (!ptr_mob->aggro)
			{
				ptr_mob->followPath(p_dt);
			}else
			{
				if (math::distance(p_player_position, ptr_mob->getPosition()) > GetAtackDistance(ptr_mob->type) || !LineOfSight(p_player_position, ptr_mob->getPosition(), ptr_map))
				{
					ptr_mob->followPath(p_dt);
				}else
				{
					if (ptr_mob->m_attack_timer <= 0)
					{
						attack(p_player_position, ptr_arrows, ptr_mob, ptr_map);
					}
				}
			}
			if (!ptr_mob->IntersectsWall(sf::Rect<int>(ptr_mob->getPosition().x - ptr_mob->width/2 + ptr_mob->velocity.x + 3, ptr_mob->getPosition().y + ptr_mob->velocity.y, ptr_mob->width - 6, ptr_mob->height/2 - 3), *ptr_map))
			{
				ptr_mob->move(ptr_mob->velocity);
			}
		}
	}

}