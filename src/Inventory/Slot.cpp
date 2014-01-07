#include "Inventory\Slot.h"

Slot::Slot(int x, int y) 
	: x(x), y(y){	
}

Slot::Slot(){

}

Slot::~Slot(){

}

void Slot::use(){
	
}

void Slot::draw(sf::RenderTarget & target, sf::RenderStates states) const{
	states.transform = getTransform();
	if (!Items.empty())
	{
		Items[0].draw(target, states);
		/*sf::Text text;
		text.setFont(*fontPtr);
		text.setScale(0.55f, 0.55f);
		text.setColor(sf::Color::White);
		text.setPosition(getPosition().x + 17, getPosition().y + SLOTHEIGHT/2);
		text.setString(std::to_string(Items.size()));
		target.draw(text);*/
	}
}