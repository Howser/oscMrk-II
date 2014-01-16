#pragma once
#include "Map\Tile.h"
#include "Item\Projectile.h"
#include "Effects\Particles\ParticleSystem.h"

#include <vector>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

#define SPEED 10

namespace projectile{
	class Arrow : public Projectile{
	public:
		Arrow(std::vector<std::vector<gen::Tile>>* ptr_tiles, float angle, sf::Sprite & p_sprite, int p_damage, const Items & p_item, ParticleSystem* p_ps);
		~Arrow();
		void update();
		void draw(sf::RenderTarget & target, sf::RenderStates states)const;
		void kill();

	private:
		ParticleSystem* ps;
	};
};