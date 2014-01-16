#pragma once
#include <SFML\Graphics\Color.hpp>
#include <SFML\System\Vector3.hpp>
#include <SFML\Graphics\View.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include "ResourceHolders\TextureHolder.h"
#include "Effects\Lights\Light.h"
#include "Effects\Lights\LightManager.h"

struct Torch : public sf::Transformable, public sf::Drawable{
	Torch(Light & p_light, LightManager* ptr_light_manager, TextureHolder* ptr_texture_holder);
	Torch();
	~Torch();

	sf::Sprite m_sprite;
	Light* ptr_light;

	void update();
	void draw(sf::RenderTarget & target, sf::RenderStates states)const;
};