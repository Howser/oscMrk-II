#include "Mob\MajorMob.h"

MajorMob::MajorMob(TYPE type, TextureHolder* textureHolder, std::vector<DeadMob>* p_deadMobs){
	startPos = (sf::Vector2<int>)getPosition();
	//speed = GetSpeed(type);
	//attackSpeed = GetAttackSpeed(type);
	//damage = GetDamage(type);
	//aggroDist = GetAggroDist(type);
	health = GetHp(type);
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
		sprite.setTexture(*textureHolder->getTexture(Textures::TestMobDead));
		break;
	}
	sprite.setColor(sf::Color::Magenta);
	specialTimer = 0.f;
	timeSincePath = 0.f;
	worldCollision = false;
	dead = false;
	aggro = false;
}

MajorMob::MajorMob(Mob const& mob, std::vector<DeadMob>* p_deadMobs){
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
		sprite.setTexture(*textureHolder->getTexture(Textures::TestMobDead));
		break;
	}
	specialTimer = 0.f;
	timeSincePath = 0.f;
	worldCollision = false;
	dead = false;
	aggro = false;
}

void MajorMob::update(std::vector<std::vector<gen::Tile>>* map, sf::Time& deltaTime, sf::Vector2f & playerPosition, int* p_health, Mob* ptr_mob, std::vector<projectile::Spell>* ptr_spells, std::vector<projectile::Arrow>* ptr_arrows){
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
			move(velocity);
		}
		specialTimer += deltaTime.asSeconds();
		if (specialTimer >= GetSpecialTimer(type))
		{
			spcl::Special(type, &specialTimer);
		}
	}
}

bool MajorMob::operator=(MajorMob const& rhs){
	return ID == rhs.ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Don't really need to define these functions since we have them in MinorMob. But we might want to make the MajorMobs act a certain way...//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MajorMob::draw(sf::RenderTarget & target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(sprite, states);
}

void MajorMob::takeDamage(int damage){
	health -= damage;
	if (health <= 0)
	{
		die();
	}else
	{
		aggro = true;
	}
}

void MajorMob::dealDamage(int* health){
	*health = GetDamage(type) - (std::abs(GetDamage(type) - *health) > 0 ? 0:std::abs(GetDamage(type) - *health));
}

void MajorMob::die(){
	dead = true;
	sprite.setTexture(*textureHolder->getTexture(Textures::TestMobDead));
	for (int i = 0; i < lootTable.lootItems.size(); i++)        
	{
		switch (lootTable.lootItems[i].item)
		{
		default:
			int q = math::random(lootTable.lootItems[i].minQ, lootTable.lootItems[i].maxQ);
			for (int Q = 0; Q < q; Q++)
			{
				if (lootTable.lootItems[i].c >= math::random(1, 100))
				{
					inventory.slots[inventory.GetFirstAvailableSlot(lootTable.lootItems[i].item).x][inventory.GetFirstAvailableSlot(lootTable.lootItems[i].item).y].Items.push_back(GearItem(lootTable.lootItems[i].item, *textureHolder, ID));
				}
			}
			break;
		}
	}
	p_deadMobs->push_back(DeadMob(&sprite, sf::Vector2<float>(getPosition().x - width/2, getPosition().y - height/2), &type));
}
//
//void Mob::stop(){
//	path.clear();
//	velocity.x = 0;
//	velocity.y = 0;
//}
//
//void Mob::checkCollision(std::vector<std::vector<gen::Tile>>* map){
//	for (unsigned int x = (getPosition().x - width)/32 - 1, y = 0; x < (getPosition().x - width/2)/32 + 1; x++)
//	{
//		if (x > 0 && x < map->size())
//		{
//			for (y = (getPosition().y - height)/32 - 1; y < (getPosition().y -height/2)/32 + 1; y++)
//			{
//				if (y > 0 && y < map[x].size())
//				{
//					if ((*map)[x][y].type != 1)
//					{
//						if (sf::IntRect(getPosition().x - width/2 - 3 + velocity.x, getPosition().y - height/2 - 3 + velocity.y, width + 3, height + 3).intersects(sf::IntRect(x*32, y*32, 32, 32)))
//						{
//							worldCollision = true;
//						}else
//						{
//							worldCollision = false;
//						}
//						if (sf::IntRect(getPosition().x - width/2 + velocity.x, getPosition().y + velocity.y, width, height/2).intersects(sf::IntRect(x*32, y*32, 32, 32)))
//						{
//							if (getPosition().x - width/2 + velocity.x > (x*32 + 16))
//							{
//								setPosition(getPosition().x + 1, getPosition().y);
//							}else if (getPosition().x - width/2 + velocity.x <= x*32 + 16)
//							{
//								setPosition(getPosition().x - 1, getPosition().y);
//							}
//							if (getPosition().y + velocity.y > y*32 + 16)
//							{
//								setPosition(getPosition().x, getPosition().y + 1);
//							}else if(getPosition().y + velocity.y <= y*32 + 16)
//							{
//								setPosition(getPosition().x, getPosition().y - 1);
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
//void Mob::followPath(sf::Time& dt, mobtree::Tree* p_tree){
//	if (!path.empty())
//	{
//		float dist = vec::length(getPosition() - path.back());
//		if (dist < 17.f)
//		{
//			path.pop_back();
//			if (path.empty())
//			{
//				timeSincePath = 0;
//			}
//		}
//
//		if (!path.empty())
//		{
//			sf::Vector2f target = path.back() - getPosition();
//			target = vec::normalize(target);
//			velocity.x = target.x * GetSpeed(type) * dt.asSeconds();
//			velocity.y = target.y * GetSpeed(type) * dt.asSeconds();
//		}
//	}
//	else
//	{
//		stop();
//	}
//}
//
//bool Mob::IntersectsWall(sf::Rect<int> const& position, std::vector<std::vector<gen::Tile>> const& map){
//	for (int x = position.left/32 - 1, y = position.top/32 - 1; x < (position.left + position.width)/32 + 2; x++)
//	{
//		if (x > 0 && x < map.size())
//		{
//			for (y = position.top/32 - 1; y < (position.top + position.height)/32 + 2; y++)
//			{
//				if (y > 0 && y < map[x].size())
//				{
//					if (map[x][y].type == 2)
//					{
//						if (position.intersects(sf::Rect<int>(x*32, y*32, 32, 32)))
//						{
//							return true;
//						}
//					}
//				}
//			}
//		}
//	}
//	return false;
//}