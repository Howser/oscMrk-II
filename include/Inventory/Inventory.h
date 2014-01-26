#pragma once
#include "Slot.h"
#include "StackManager.h"
#include "InventoryManager.h"
#include "Item\ItemManager.h"
#include "ResourceHolders\TextureHolder.h"
#include <vector>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics.hpp>

class Inventory : public sf::Drawable{
public:
	Inventory(int width, int height, int x_offset, int y_offset, TextureHolder const& textures);
	Inventory();
	~Inventory();

	int width, height;

	virtual void Command(Slot * slot, Slot * mouseSlot, inv::Command command, StackManager* stackManagerPtr);
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;

	Slot GetFirstAvailableSlot(Items item);

	sf::Sprite slotSprite;

	std::vector<std::vector<Slot>> slots;

	void RemoveItem(Items item, int Quantity);

	void clear(bool removePlayerItems);

	bool empty();
	bool full();
	bool contains(const Items & item);

	int amount(const Items & p_item);

	sf::Font font;

	sf::Vector2f GetPosition(Slot const& slot);
};