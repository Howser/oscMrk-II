#pragma once
#include "Mob.h"

class MinorMob : public Mob{
public:
	MinorMob(TYPE type, TextureHolder* textureHolder, std::vector<DeadMob>* p_deadMobs);
	MinorMob(Mob const& mob, std::vector<DeadMob>* p_deadMobs);
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const;
	void update(std::vector<std::vector<gen::Tile>>* map, sf::Time& deltaTime, sf::Vector2f & playerPosition);
	void takeDamage(int damage);
	void dealDamage(int* health);
	void die();
	bool operator=(MinorMob const& rhs);
};