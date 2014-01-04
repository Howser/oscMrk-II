#include "Inventory\Tooltip.h"
#include <iostream>
Tooltip::Tooltip(){

}

Tooltip::Tooltip(TextureHolder const& textureHolder) : show(false){
	sprite.setTexture(*textureHolder.getTexture(Textures::Tooltip));
	font.loadFromFile("resources/fonts/00TT.TTF");
}

Tooltip::~Tooltip(){

}

void Tooltip::SetStats(Slot* slot){
	show = true;
	stats = GetStats(slot->Items[0].item, &size);
	setPosition(slot->getPosition().x + SLOTWIDTH, slot->getPosition().y);
	setScale(size.x, size.y);
	this->slot = slot;
}

void Tooltip::Hide(){
	slot = NULL;
	show = false;
}

void Tooltip::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	states.transform *= getTransform();
	//target.draw(sprite, states);
	sf::Text text;
	text.setFont(font);
	text.setColor(sf::Color::White);
	text.setScale(0.55f, 0.55f);
	text.setPosition(getPosition());
	text.setString(stats);
	target.draw(text);
}