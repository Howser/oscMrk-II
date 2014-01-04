#include "Inventory\DeleteItem.h"

DeleteItem::DeleteItem(TextureHolder & textureHolder) : show(false){
	sprite.setTexture(*textureHolder.getTexture(Textures::DeleteItem));
	font.loadFromFile("resources/fonts/00TT.TTF");
	setPosition(1280/2 - sprite.getTextureRect().width/2, 32);
	deleteButton = sf::Rect<int>(getPosition().x + 57, getPosition().y + 61, 67, 25);
	cancelButton = sf::Rect<int>(getPosition().x + 126, getPosition().y + 61, 67, 25);
}

DeleteItem::~DeleteItem(){}

void DeleteItem::Toggle(bool toggle, Slot* p_slot){
	switch (toggle)
	{
	case true:
		if (!p_slot->Items.empty())
		{
			show = true;
			this->p_slot = p_slot;
		}
		break;
	case false:
		show = false;
		this->p_slot = NULL;
		break;
	default:
		show = false;
		this->p_slot = NULL;
		break;
	}
}

void DeleteItem::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	if (show && p_slot != NULL)
	{
		states.transform *= getTransform();
		target.draw(sprite, states);
		sf::Text text;
		std::string name, q;
		name = ItemNames[p_slot->Items[0].item];
		q = " x" + std::to_string(p_slot->Items.size());
		if (p_slot->Items.size() > 1)
		{
			text.setString("Delete items: " + name + q);
		}else
		{
			text.setString("Delete item: " + name + q);
		}
		text.setScale(0.5f, 0.5f);
		text.setPosition(getPosition().x + sprite.getTextureRect().width/2 - ((name.size() + q.size())*10)/2 - 45, getPosition().y + 5);
		text.setFont(font);
		text.setColor(sf::Color::Black);
		target.draw(text);
	}
}