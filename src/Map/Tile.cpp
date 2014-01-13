#include "Map\Tile.h"
#include <iostream>
gen::Tile::Tile(short x, short y) 
	: x(x), y(y), type(0), m_explored(false)
{
	setPosition(x*32, y*32);
}

gen::Tile::Tile() : type(0), m_explored(false){
}

gen::Tile::~Tile(void)
{
}