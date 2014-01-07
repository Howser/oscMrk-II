#pragma once
#include "Item\GearItem.h"
#include <iostream>

GearItem::GearItem(Items item, TextureHolder & textureHolder, int ID) : eGearSlot(GetSlot(item)){
	this->item = item;
	this->ID = ID;
	armor = (GetArmor(item));
	sprite.setTexture(*textureHolder.getTexture((Textures::ID)(item + 12)));
	setPosition(3, 3);
}

GearItem::~GearItem(){

}

void GearItem::Equip(GearSlot* gearSlot, Slot* slot, Inventory* inventory){
	if (gearSlot->Items.size() > 0)
	{
		inventory->slots[inventory->GetFirstAvailableSlot(gearSlot->Items[0].item).x][inventory->GetFirstAvailableSlot(gearSlot->Items[0].item).y].Items.push_back(gearSlot->Items[0]);
	}
	gearSlot->Items.clear();
	gearSlot->Items.push_back(GearItem(*this));
	slot->Items.clear();
}

void GearItem::draw(sf::RenderTarget& target, sf::RenderStates states)const{
	states.transform *= getTransform();
	target.draw(sprite, states);
}