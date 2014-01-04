#pragma once
#include "ItemManager.h"
#include "ResourceHolders\TextureHolder.h"

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Sprite.hpp>

class BaseItem : public sf::Transformable, public sf::Drawable{
public:
	Items item;
	int ID;
	sf::Sprite sprite;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};