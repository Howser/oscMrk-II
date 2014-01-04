#include "Item\LootItem.h"

LootItem::LootItem(Items item, int minQ, int maxQ, int c) : item(item), minQ(minQ), maxQ(maxQ), c(c){
	
}

LootItem::LootItem() : item(Items::NOITEM), minQ(0), maxQ(0), c(0){
	
}