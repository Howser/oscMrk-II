#include "Inventory\Tooltip.h"
#include <iostream>
Tooltip::Tooltip(){

}

Tooltip::Tooltip(TextureHolder const& textureHolder) : show(false){
	sprite.setTexture(*textureHolder.getTexture(Textures::Tooltip));
	font.loadFromFile("resources/fonts/00TT.TTF");
	for (int i = 0; i < 4; i++)
	{
		m_borders[i].setTexture(*textureHolder.getTexture(Textures::Border));
	}
	m_borders[1].setRotation(270);
	m_borders[2].setRotation(90);
	m_borders[3].setRotation(180);
}

Tooltip::~Tooltip(){

}

void Tooltip::SetStats(Slot* slot){
	show = true;
	stats = GetStats(slot->Items[0].item);
	setPosition(slot->getPosition().x + SLOTWIDTH, slot->getPosition().y);
	int width = 0, height = 0;
	sf::Text text;
	text.setFont(font);
	for (int i = 0; i < stats.size(); i++)
	{
		text.setString(stats[i]);
		if (text.getLocalBounds().width > width)
		{
			width = text.getLocalBounds().width;
		}
		height += text.getLocalBounds().height;
	}
	m_borders[0].setScale(width*0.55f, 1);
	m_borders[1].setScale(height, 1);
	m_borders[2].setScale(height, 1);
	m_borders[3].setScale(width*0.55f, 1);
	m_borders[0].setPosition(getPosition().x, getPosition().y);
	m_borders[1].setPosition(getPosition().x, getPosition().y + height);
	m_borders[2].setPosition(getPosition().x + width*0.55f, getPosition().y);
	m_borders[3].setPosition(getPosition().x + width*0.55f, getPosition().y + height);
	sprite.setScale(width*0.55f, height);
	this->slot = slot;
}

void Tooltip::Hide(){
	slot = NULL;
	show = false;
}

void Tooltip::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	states.transform *= getTransform();
	target.draw(sprite, states);
	sf::Text text;
	std::string string;
	for (int i = 0; i < stats.size(); i++)
	{
		if (i == stats.size() - 1)
		{
			string += stats[i];
		}else
		{
			string += stats[i] + "\n";
		}
	}
	text.setString(string);
	text.setFont(font);
	text.setColor(sf::Color::White);
	text.setScale(0.55f, 0.55f);
	text.setPosition(getPosition());
	for (int i = 0; i < 4; i++)
	{
		target.draw(m_borders[i]);
	}
	target.draw(text);
}