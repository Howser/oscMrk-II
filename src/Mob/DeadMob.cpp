#include "Mob\DeadMob.h"

DeadMob::DeadMob(sf::Sprite* sprite, sf::Vector2<float> const& position, TYPE* type) : sprite(sprite), type(type){
	setPosition(position);
}

void DeadMob::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	states.transform *= getTransform();
	target.draw(*sprite, states);
}