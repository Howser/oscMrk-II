#include "Inventory\StackManager.h"

StackManager::StackManager(TextureHolder & textureHolder) : count(0), show(false){
	sprite.setTexture(*textureHolder.getTexture(Textures::StackManager));
	font.loadFromFile("resources/fonts/00TT.TTF");
}

void StackManager::Show(Slot* slotPtr){
	this->slotPtr = slotPtr;
	setPosition(slotPtr->getPosition());
	SetRects();
	count = 0;
	show = true;
}

void StackManager::Hide(){
	slotPtr = nullptr;
	count = 0;
	show = false;
}

void StackManager::Increase(){
	if (count+1 <= slotPtr->Items.size())
	{
		count++;
	}
}

void StackManager::Decrease(){
	if (count-1 >= 0)
	{
		count--;
	}
}

void StackManager::SetRects(){
	increase = sf::Rect<int>(slotPtr->getPosition().x, slotPtr->getPosition().y, sprite.getTextureRect().width / 2, sprite.getTextureRect().height/2);
	decrease = sf::Rect<int>(slotPtr->getPosition().x, slotPtr->getPosition().y + sprite.getTextureRect().height/2, sprite.getTextureRect().width / 2, sprite.getTextureRect().height/2);
	pickUp = sf::Rect<int>(slotPtr->getPosition().x + 50, slotPtr->getPosition().y, sprite.getTextureRect().width / 2, sprite.getTextureRect().height);
}

void StackManager::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	states.transform *= getTransform();
	target.draw(sprite, states);
	sf::Text text;
	text.setFont(font);
	text.setColor(sf::Color::White);
	text.setScale(0.55f, 0.55f);
	text.setPosition(increase.left + increase.width/2 - 4, increase.top + increase.height/2 + 2);
	text.setString(std::to_string(count));
	target.draw(text);
}