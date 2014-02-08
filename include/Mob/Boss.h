#pragma once

#include <functional>

#include "Mob.h"

class Boss : public Mob {

public:

	Boss();
	Boss(TextureHolder* textureHolder, std::vector<DeadMob>* p_deadMobs, std::function<void(void)> die_func);
	Boss(Mob const& mob, std::vector<DeadMob>* p_deadMobs);
	~Boss();

	void update(std::vector<std::vector<gen::Tile>>* map, sf::Time& deltaTime, sf::Vector2f & playerPosition, int* p_health);
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	void takeDamage(int damage);

	void die();
	std::function<void(void)> die_func;

private:
	enum Phase {
		///<summary>Path to the player and attack him like normal mobs do.</summary>
		Normal,
		///<summary>Boss stands still and fires a laser after the player.</summary>
		Laser,
		///<summary>Stands still and shoots projectiles out in a circle.</summary>
		FireCircle,
		///<summary>Summons smaller mobs that attack the player.</summary>
		Summon
	};
	Phase m_current_phase;

	void dealDamage(int* health);

	
};