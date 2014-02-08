#pragma once
#include "Mob.h"

class MinorMob : public Mob{
public:
	MinorMob(TYPE type, TextureHolder* textureHolder, std::vector<DeadMob>* p_deadMobs);
	MinorMob(Mob const& mob, std::vector<DeadMob>* p_deadMobs);
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const;
	virtual void update(std::vector<std::vector<gen::Tile>>* map, sf::Time& deltaTime, sf::Vector2f & playerPosition, int* p_health, Mob* ptr_mob);
	void takeDamage(int damage);
	void dealDamage(int* health);
	void die();
	bool operator==(MinorMob const& rhs);
};

static struct Skeleton : public MinorMob{
	static void update(std::vector<std::vector<gen::Tile>>* ptr_map, sf::Time& p_dt, sf::Vector2<float>& p_player_position, int* ptr_health, Mob* ptr_mob);
	static void attack(const sf::Vector2<float>& p_player_position);
};

static struct Spider : public MinorMob{
	static void update(std::vector<std::vector<gen::Tile>>* ptr_map, sf::Time& p_dt, sf::Vector2<float>& p_player_position, int* ptr_health, Mob* ptr_mob);
};