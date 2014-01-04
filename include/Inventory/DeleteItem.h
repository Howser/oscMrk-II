#pragma once
#include "Inventory\Slot.h"

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Font.hpp>
#include <SFML\Graphics\Sprite.hpp>

class DeleteItem : public sf::Transformable, public sf::Drawable{
public:
	DeleteItem(TextureHolder & textureHolder);
	~DeleteItem();

	bool show;
	sf::Rect<int> deleteButton, cancelButton;
	Slot* p_slot;

	void Toggle(bool toggle, Slot* p_slot);
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	
private:
	sf::Font font;
	sf::Sprite sprite;
};