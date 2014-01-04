#pragma once

#include "Inventory\Slot.h"
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>

struct StackManager : public sf::Transformable, public sf::Drawable{
	StackManager(TextureHolder & textureHolder);
	int count;
	bool show;
	sf::Font font;
	Slot* slotPtr;

	void Show(Slot* slotPtr);
	void Hide();
	void SetRects();
	void Decrease();
	void Increase();

	sf::Sprite sprite;
	sf::Rect<int> increase, decrease, pickUp;

	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
};