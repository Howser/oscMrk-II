#include "Map\Properties\Torch.h"

Torch::Torch(sf::Vector3f & p_position, sf::Color & p_color, float p_radius) : m_light(p_color, sf::Vector3f(0, 0, p_position.z), sf::Vector3f(.001f, 4.f, 1.f), false), m_position(p_position), m_radius(p_radius), m_draw(false){}

void Torch::update(sf::View* ptr_view){
	if (sf::Rect<float>(m_position.x, m_position.y, m_radius*2, m_radius*2).intersects(ptr_view->getViewport()))
	{
		m_draw = true;
	}else
	{
		m_draw = false;
	}
}

void Torch::draw(){
	
}