#pragma once
#include "MapManager.h"
#include "Mob\MobManager.h"

namespace gen
{
	struct Room
	{
		Room(short x, short y, short width, short height, bool cave);
		Room();

		~Room();

		short x;
		short y;
		short width;
		short height;
		short mobs;
		short radius;
		bool cave;

		sf::Vector2f getCenter();
	};
}