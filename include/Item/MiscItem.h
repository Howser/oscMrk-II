#pragma once
#include "Item\BaseItem.h"

class MiscItem : public BaseItem{
public:
	MiscItem(Items item, TextureHolder & textureHolder, int ID);
	~MiscItem();
	void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};