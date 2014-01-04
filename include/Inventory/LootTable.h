#pragma once

#include "Mob\MobTypeManager.h"
#include "Item\LootItem.h"

#include <iostream>
#include <vector>

struct LootTable{
	LootTable(TYPE type);
	LootTable();
	std::vector<LootItem> lootItems;
};