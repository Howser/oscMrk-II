#pragma once
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include "ResourceHolders\TextureHolder.h"
#include "Item\ItemManager.h"
#include "Slot.h"
#include <string>

class Tooltip : public sf::Drawable, public sf::Transformable{
public:
	Tooltip();
	Tooltip(TextureHolder const& textureHolder);
	~Tooltip();
	Slot* slot;
	std::string stats;
	sf::Vector2i size;
	sf::Sprite sprite, m_borders[4];
	sf::Font font;
	bool show;
	void SetStats(Slot* slot);
	void Hide();
	void draw(sf::RenderTarget & target, sf::RenderStates states)const;
};