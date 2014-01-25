#include "Inventory\Inventory.h"

Inventory::Inventory(int WIDTH, int HEIGHT, int offset, TextureHolder const& textures) 
	: width(WIDTH), height(HEIGHT){
		Slot slot = Slot(0, 0);
		slots.resize(width, std::vector<Slot>(height, slot));
		font.loadFromFile("resources/fonts/00TT.TTF");
		for (unsigned int x = 0, y = 0; x < width; x++)
		{
			for (y = 0; y < height; y++)
			{
				slots[x][y].x = x;
				slots[x][y].y = y;
				slots[x][y].fontPtr = &font;
				slots[x][y].setPosition(x * SLOTWIDTH + 5*x + offset, y * SLOTHEIGHT + 5*y);
			}
		}
		slotSprite.setTexture(*textures.getTexture(Textures::InventorySlot));
}

Inventory::Inventory(){

}

Inventory::~Inventory(){

}

void Inventory::Command(Slot * slot, Slot * mouseSlot, inv::Command command, StackManager* stackManagerPtr){
	switch (command)
	{
	case inv::Move:
		if (mouseSlot->Items.empty())
		{
			if (stackManagerPtr->show)
			{
				BaseItem* item = &slot->Items[0];
				for (int i = 0; i < stackManagerPtr->count; i++)
				{
					mouseSlot->Items.push_back(*item);
					slot->Items.pop_back();
				}
			}else
			{
				mouseSlot->Items = slot->Items;
				slot->Items.clear();
			}
			break;
		}else
		{
			if (!slot->Items.empty())
			{
				if (mouseSlot->Items[0].item != slot->Items[0].item)
				{
					std::vector<BaseItem> items = mouseSlot->Items;
					mouseSlot->Items = slot->Items;
					slot->Items = items;
				}else
				{
					int j = 0;
					for (unsigned int i = 0; i < mouseSlot->Items.size(); i++, j++)
					{
						if (slot->Items.size() < StackSize(mouseSlot->Items[0].item))
						{
							slot->Items.push_back(mouseSlot->Items[i]);
						}else
						{
							break;
						}
					}
					for (int i = 0; i < j; i++)
					{
						mouseSlot->Items.pop_back();
					}
				}
			}else
			{
				slot->Items = mouseSlot->Items;
				mouseSlot->Items.clear();
			}
		}
		break;
	case inv::Use:
		if (mouseSlot->Items.empty())
		{
			if (slot->Items.size() > 0)
			{
				slot->use();
				slot->Items.pop_back();
			}
		}
		break;
	}
}

Slot Inventory::GetFirstAvailableSlot(Items item){
	Slot slot = Slot(-1, -1);
	for (unsigned int y = 0, x = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			if (slots[x][y].Items.size() > 0 && slots[x][y].Items.size() < StackSize(slots[x][y].Items[0].item))
			{
				if (slots[x][y].Items[0].item == item)
				{
					return slots[x][y];
				}
			}else if (slots[x][y].Items.size() == 0) 
			{
				return slots[x][y];
			}
		}
	}
	return slot;
}

void Inventory::draw(sf::RenderTarget & target, sf::RenderStates states) const{
	for (unsigned int x = 0, y = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			states.transform = slots[x][y].getTransform();
			target.draw(slotSprite, states);
			slots[x][y].draw(target, states);
		}
	}
}

void Inventory::clear(bool removePlayerItems){
	for (int x = 0, y = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			if (removePlayerItems)
			{
				slots[x][y].Items.clear();
			}else
			{
				if (!slots[x][y].Items.empty())
				{
					if (slots[x][y].Items[0].ID != -1)
					{
						slots[x][y].Items.clear();
					}
				}
			}
		}
	}
}

bool Inventory::empty(){
	for (int x = 0, y = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			if (!slots[x][y].Items.empty())
			{
				return false;
			}
		}
	}
	return true;
}

bool Inventory::full(){
	for (int x = 0, y = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			if (!slots[x][y].Items.empty())
			{
				if (slots[x][y].Items.size() < StackSize(slots[x][y].Items[0].item))
				{
					return false;
				}else
				{
					if (x == width && y == height)
					{
						return true;
					}
				}
			}else
			{
				return false;
			}
		}
	}
	return false;
}

void Inventory::RemoveItem(Items item, int Quantity){
	int q = Quantity;
	for (int x = 0, y = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			if (!slots[x][y].Items.empty())
			{
				if (slots[x][y].Items[0].item == item)
				{
					for (int i = 0; i < Quantity; i++)
					{
						if (!slots[x][y].Items.empty() && q > 0)
						{
							slots[x][y].Items.pop_back();
							q--;
						}if (q <= 0)
						{
							return;
						}
					}
				}
			}
		}
	}
}

bool Inventory::contains(const Items & item){
	for (unsigned int x = 0, y = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			if (!slots[x][y].Items.empty())
			{
				if (slots[x][y].Items[0].item == item)
				{
					return true;
				}
			}
		}
	}
	return false;
}

int Inventory::amount(const Items & p_item){
	unsigned int i = 0;
	for (unsigned int x = 0, y = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			if (!slots[x][y].Items.empty())
			{
				if (slots[x][y].Items[0].item == p_item)
				{
					i += slots[x][y].Items.size();
				}
			}
		}
	}
	return i;
}