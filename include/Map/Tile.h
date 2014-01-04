#pragma once

namespace gen
{
	struct Tile
	{
		Tile(void);
		Tile(short x, short y);
		~Tile(void);
		short		x;
		short		y;
		short		type;
		short		ID;
	};
}