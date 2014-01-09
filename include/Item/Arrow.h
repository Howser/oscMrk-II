#pragma once
#include "Map\Tile.h"

#include <vector>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

#define ARROW_SPEED 10

namespace projectile{
	class Arrow : public sf::Transformable, public sf::Drawable{
	public:
		Arrow(std::vector<std::vector<gen::Tile>>* ptr_tiles, float angle, sf::Sprite & p_sprite, int p_damage);
		~Arrow();
		void update();
		void draw(sf::RenderTarget & target, sf::RenderStates states)const;
		bool dead;
		int m_damage;
	private:
		std::vector<std::vector<gen::Tile>>* p_tiles;
		sf::Sprite m_sprite;
		sf::Vector2<float> m_velocity;
		
	};
};