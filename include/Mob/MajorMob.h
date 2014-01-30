#pragma once
#include "Mob.h"

class MajorMob : public Mob{
public:
	MajorMob(TYPE type, TextureHolder* textureHolder, std::vector<DeadMob>* p_deadMobs);
	MajorMob(Mob const& mob, std::vector<DeadMob>* p_deadMobs);
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const;
	void update(std::vector<std::vector<gen::Tile>>* map, sf::Time& deltaTime, sf::Vector2f & playerPosition, int* p_health);
	void takeDamage(int damage);
	void dealDamage(int* health);
	void die();
	bool operator=(MajorMob const& rhs);

	float specialTimer;
};