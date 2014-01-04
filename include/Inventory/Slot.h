#pragma once
#include "InventoryManager.h"
#include <SFML\Graphics\Font.hpp>
#include <SFML\Graphics\Text.hpp>
#include <vector>
#include "Item\BaseItem.h"

#define SLOTWIDTH 32
#define SLOTHEIGHT 32

class Slot : public sf::Transformable, public sf::Drawable{
public:
	Slot();
	Slot(int x, int y);
	int x, y;
	~Slot();
	virtual void use();
	sf::Font* fontPtr;
	std::vector<BaseItem> Items;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
};