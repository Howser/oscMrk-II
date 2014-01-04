#pragma once
#include "Item\BaseItem.h"
#include "Inventory\GearSlot.h"

class GearItem : public BaseItem{
public:
	GearItem(Items item, TextureHolder & textureHolder, int ID);
	~GearItem();
	int armor;
	eGearSlot eGearSlot;
	void draw(sf::RenderTarget& target, sf::RenderStates states)const;
	void Equip(GearSlot* gearSlot, Slot* slot, Inventory* inventory);
};