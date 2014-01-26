#pragma once
#include "Inventory\Inventory.h"
#include "Inventory\GearSlot.h"

class GearSlots : public Inventory{
public:
	GearSlots::GearSlots(TextureHolder const& textures, int invWidth, int x_offset, int y_offset);
	std::vector<GearSlot> slots;
	void GearSlots::Command(GearSlot* slot, Inventory* inventory);
	void draw(sf::RenderTarget & target, sf::RenderStates states)const;
};