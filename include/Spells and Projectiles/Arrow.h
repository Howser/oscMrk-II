#pragma once
#include "Map\Tile.h"
#include "Spells and Projectiles\Projectile.h"
#include "Effects\Particles\ParticleSystem.h"

#include <vector>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

namespace projectile{
	class Arrow : public Projectile{
	public:
		Arrow(std::vector<std::vector<gen::Tile>>* ptr_tiles, float angle, sf::Sprite & p_sprite, int p_damage, const Items & p_item, ParticleSystem* p_ps);
		~Arrow();
		void update(sf::Time & p_dt);
		void draw(sf::RenderTarget & target, sf::RenderStates states)const;
		void kill();

		///<summary>Delete the projectile from the world when this timer == 0.</summary>
		float timer;

	private:
		ParticleSystem* ps;
	};
};