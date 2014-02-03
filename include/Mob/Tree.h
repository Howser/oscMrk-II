#pragma once

#include "Map\Tile.h"
#include "Mob\Mob.h"
#include <vector>

namespace mobtree{
	static const int SIZE = 8;

	struct Branch{
		std::vector<std::vector<gen::Tile*>> tiles;
		std::vector<Mob*> mobs;
		int key;

		void AddMob(Mob* p_mob);
		void RemoveMob(Mob* p_mob);

		Mob* GetMobWithTarget(const sf::Vector2i & position);
		Mob* GetMobWithTarget(const sf::Vector2i & position, const Mob & exclude);
	};

	struct Tree{
		std::vector<Branch> m_branches;

		sf::Vector2<int> m_size;

		void build(std::vector<std::vector<gen::Tile*>> tiles, std::vector<Mob*> mobs);

		Branch* search(Mob const& mob);
		Branch* search(sf::Vector2f const& position);

		void move(Mob* p_mob, Branch* p_from, Branch* p_to);
	};
}