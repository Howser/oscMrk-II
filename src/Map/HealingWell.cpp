#include "Map\HealingWell.h"

gen::HealingWell::HealingWell(sf::Vector2f position){
	setPosition(position);
	charges = 2;
}

void gen::HealingWell::use(){
	charges--;
}
