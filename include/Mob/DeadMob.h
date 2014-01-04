#pragma once
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include "Mob\MobTypeManager.h"

struct DeadMob : public sf::Transformable, public sf::Drawable{
	DeadMob(sf::Sprite* sprite, sf::Vector2<float> const& position, TYPE* type);
	void draw(sf::RenderTarget & target, sf::RenderStates states)const;
	sf::Sprite* sprite;
	TYPE* type;
};