#pragma once
#include "Map\Tile.h"
#include "Item\Projectile.h"
#include "ItemManager.h"
#include "Effects\Lights\LightManager.h"
#include "Effects\Particles\Emitter.h"
#include "Effects\Particles\ParticleSystem.h"

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
		Spell(const sf::Vector2f & p_position, float angle, const Items & p_item, sf::Sprite & p_sprite, int p_damage, std::vector<std::vector<gen::Tile>>* ptr_tiles, Light & p_light);
		~Spell();
		void update(sf::Time & p_dt);
		void draw(sf::RenderTarget & target, sf::RenderStates states)const;
		void kill();
		Light m_light;
		sf::Vector2f m_position;
	};
};