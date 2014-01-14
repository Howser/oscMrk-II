#pragma once
#include "Map\Tile.h"
#include "Item\Projectile.h"
#include "ItemManager.h"

#include <vector>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

#define SPEED 10

namespace projectile{
	class Spell : public Projectile{
	public:
		Spell(std::vector<std::vector<gen::Tile>>* ptr_tiles, float angle, sf::Sprite & p_sprite, int p_damage, const Items & p_item);
		~Spell();
		void update();
		void draw(sf::RenderTarget & target, sf::RenderStates states)const;
		void kill();
	};
};