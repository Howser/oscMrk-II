#include "Map\Properties\Torch.h"

Torch::Torch(Light & p_light, LightManager* ptr_light_manager, TextureHolder* ptr_texture_holder){
	ptr_light = ptr_light_manager->add(p_light);
	m_sprite.setTexture(*ptr_texture_holder->getTexture(Textures::Chest_Cold));
}

Torch::Torch(){}

Torch::~Torch(){}

void Torch::update(){

}

void Torch::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	states.transform *= getTransform();
	target.draw(m_sprite, states);
}