#pragma once
#include "Map\Tile.h"
#include "Item\ItemManager.h"

#include <vector>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\System\Time.hpp>

namespace projectile{
	class Projectile : public sf::Transformable, public sf::Drawable{
	public:
		virtual void update(sf::Time & p_dt) = 0;
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const = 0;
		virtual void kill() = 0;
		bool dead;
		int m_damage;
		sf::Sprite m_sprite;
		Items m_item;
	protected:
		std::vector<std::vector<gen::Tile>>* p_tiles;
		sf::Vector2<float> m_velocity;
	};
};