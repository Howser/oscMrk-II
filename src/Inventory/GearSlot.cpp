#pragma once
#include "Inventory\GearSlot.h"
#include <iostream>
void GearSlot::use(Inventory* inventory){
	if (GearSlot::Items.size() > 0)
	{
		if (inventory->GetFirstAvailableSlot(Items[0].item).x != -1)
		{
			inventory->slots[inventory->GetFirstAvailableSlot(Items[0].item).x][inventory->GetFirstAvailableSlot(Items[0].item).y].Items.push_back(Items[0]);
			Items.clear();
		}else
		{
			//maybe display an error saying that the inventory is full or something...
		}
	}
}