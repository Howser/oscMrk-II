#pragma once
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
namespace gen
{
	struct Tile : public sf::Transformable
	{
		Tile(void);
		Tile(short x, short y);
		~Tile(void);
		short x;
		short y;
		short type;
		short ID;
		bool m_explored;
	};
}