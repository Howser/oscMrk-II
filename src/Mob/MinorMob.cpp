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
	case TYPE::skeleton:
		sprite.setTexture(*textureHolder->getTexture(Textures::Skeleton));
		m_animation = Animation(sf::Vector2<int>(24, 22), 3, 0.2f);
		break;
	case TYPE::spider:
		sprite.setTexture(*textureHolder->getTexture(Textures::Spider));
		m_animation = Animation(sf::Vector2<int>(35, 30), 4, 0.2f);
		break;
	default:
		std::cout << "!!!ERROR!!!: mob type " << type << " not supported. Check the mob constructors.\n";
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
	case TYPE::skeleton:
		sprite.setTexture(*textureHolder->getTexture(Textures::Skeleton));
		m_animation = Animation(sf::Vector2<int>(24, 22), 3, 0.2f);
		break;
	case TYPE::spider:
		sprite.setTexture(*textureHolder->getTexture(Textures::Spider));
		m_animation = Animation(sf::Vector2<int>(35, 30), 4, 0.2f);
		break;
	default:
		std::cout << "!!!ERROR!!!: mob type " << type << " not supported. Check the mob constructors.\n";
		break;
	}
	timeSincePath = 0.f;
	m_attack_timer = GetAttackSpeed(type);
	worldCollision = false;
	dead = false;
	aggro = false;
}

void MinorMob::update(std::vector<std::vector<gen::Tile>>* map, sf::Time& deltaTime, sf::Vector2f & playerPosition, int* p_health, Mob* ptr_mob, std::vector<projectile::Spell>* ptr_spells, std::vector<projectile::Arrow>* ptr_arrows){
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
	UpdateAnimation(playerPosition);

	switch (type)
	{
	case skeleton:
		Skeleton::update(map, deltaTime, playerPosition, p_health, ptr_mob, ptr_spells, ptr_arrows);
		break;
	case spider:
		Spider::update(map, deltaTime, playerPosition, p_health, ptr_mob);
		break;
	default:
		std::cout << "!!!ERROR!!!: TYPE " << ptr_mob->type << " NOT SUPPORTED\n";
		break;
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Mob type specific functions////Mob type specific functions////Mob type specific functions////Mob type specific functions////Mob type specific functions////Mob type specific functions////Mob type specific functions////Mob type specific functions//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region Skeleton
void Skeleton::attack(const sf::Vector2<float>& p_player_position, std::vector<projectile::Arrow>* ptr_arrows, Mob* ptr_mob, std::vector<std::vector<gen::Tile>>* ptr_tiles){
	float angle = std::atan2f(p_player_position.y - ptr_mob->getPosition().y, p_player_position.x - ptr_mob->getPosition().x);
	sf::Sprite arrow_sprite;
	arrow_sprite.setTexture(*ptr_mob->textureHolder->getTexture(Textures::d_Arrow));
	projectile::Arrow arrow = projectile::Arrow(ptr_tiles, angle + math::toRadians(math::random(-5, 5)), arrow_sprite, GetDamage(Items::Bow), Items::Arrow, NULL);
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
			if (math::distance(ptr_mob->getPosition(), p_player_position) <= GetAttackDistance(ptr_mob->type))
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
				if (math::distance(p_player_position, ptr_mob->getPosition()) > GetAttackDistance(ptr_mob->type) || !LineOfSight(p_player_position, ptr_mob->getPosition(), ptr_map))
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
#pragma endregion

#pragma region Spider
void Spider::attack(int* ptr_health, Mob* ptr_mob){
	ptr_mob->dealDamage(ptr_health);
	ptr_mob->m_attack_timer = GetAttackSpeed(ptr_mob->type);
}

void Spider::update(std::vector<std::vector<gen::Tile>>* ptr_map, sf::Time& p_dt, sf::Vector2<float>& p_player_position, int* ptr_health, Mob* ptr_mob){
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
			if (math::distance(ptr_mob->getPosition(), p_player_position) <= GetAttackDistance(ptr_mob->type))
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
				if (math::distance(p_player_position, ptr_mob->getPosition()) > GetAttackDistance(ptr_mob->type))
				{
					ptr_mob->followPath(p_dt);
				}else
				{
					ptr_mob->stop();
					if (ptr_mob->m_attack_timer <= 0)
					{
						attack(ptr_health, ptr_mob);
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
#pragma endregion