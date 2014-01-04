#pragma once
#include "Inventory\Slot.h"
#include "Inventory\Inventory.h"

class GearSlot : public Slot{
public:
	void GearSlot::use(Inventory* inventory);
	eGearSlot slot;
};