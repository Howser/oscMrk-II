#pragma once
#include "ItemManager.h"

struct LootItem{
	LootItem(Items item, int minQ, int maxQ, int c);
	LootItem();
	Items item;
	int minQ, maxQ, c;
};