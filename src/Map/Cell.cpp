#include "Map\Cell.h"

gen::Cell::Cell(int x, int y) 
	: x(x), y(y), active(true){}

sf::Vector2i gen::Cell::digpos(){
	int a = math::random(0, 1);
	int X = x, Y = y;
	if (a == 0)
	{
		X += math::random(-1, 1, 0);
	}else
	{
		Y += math::random(-1, 1, 0);
	}
	return sf::Vector2i(X, Y);
}