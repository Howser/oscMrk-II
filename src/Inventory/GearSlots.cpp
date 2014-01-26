#pragma once
#include "Inventory\GearSlots.h"

GearSlots::GearSlots(TextureHolder const& textures, int invWidth, int x_offset, int y_offset){
	int size = 4;
	slots.resize(size);
	for (unsigned int i = 0; i < size; i++)
	{
		slots[i].x = i;
		slots[i].y = 0;
	}
	slots[0].setPosition(slots[0].x * SLOTWIDTH + invWidth + 10 + SLOTWIDTH + x_offset, y_offset);
	slots[1].setPosition(slots[0].x * SLOTWIDTH + invWidth + 10 + SLOTWIDTH + x_offset, slots[1].x * SLOTHEIGHT + y_offset);
	slots[2].setPosition(slots[1].x * SLOTWIDTH + invWidth + 10 + SLOTWIDTH + x_offset, slots[1].x * SLOTHEIGHT - SLOTHEIGHT/2 + y_offset);
	slots[3].setPosition(slots[0].x * SLOTWIDTH + invWidth + 10 + x_offset, slots[1].x * SLOTHEIGHT - SLOTHEIGHT/2 + y_offset);
	slotSprite.setTexture(*textures.getTexture(Textures::InventorySlot));
}

void GearSlots::Command(GearSlot* slot, Inventory* inventory){
	slot->use(inventory);
}

void GearSlots::draw(sf::RenderTarget & target, sf::RenderStates states) const{
	for (unsigned int i = 0; i < slots.size(); i++)
	{
		states.transform = slots[i].getTransform();
		target.draw(slotSprite, states);
		if (slots[i].Items.size() > 0)
		{
			slots[i].Items[0].draw(target, states);
		}
	}
}