#include "Map\Tile.h"
#include <iostream>
gen::Tile::Tile(short x, short y) 
	: x(x), y(y), type(0)
{
	setPosition(x*32, y*32);
}

gen::Tile::Tile(){
}

gen::Tile::~Tile(void)
{
}