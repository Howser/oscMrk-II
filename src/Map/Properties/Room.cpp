#include "Map\Properties\Room.h"
#include "Map\Properties\MapManager.h"
#include "Mob\MobTypeManager.h"
#include <iostream>

using namespace gen;

Room::Room(void)
{
}

Room::Room(short x, short y, short width, short height, bool cave) 
	: x(x), y(y), width(width), height(height), cave(cave), mobs(0), radius(width*height){
	
}

sf::Vector2f Room::getCenter()
{
	return sf::Vector2f((x + width/2)*WIDTH,(y + height/2)*WIDTH);
}

Room::~Room(void)
{
}