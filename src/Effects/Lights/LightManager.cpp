#include "Effects\Lights\LightManager.h"

void LightManager::update(sf::View* ptr_view, sf::Time & p_dt){
	for (int i = 0; i < m_lights.size(); i++)
	{
		m_lights[i].m_draw = true;
		m_lights[i].update(p_dt);
		if (!m_lights[i].m_screen)
		{
			if (sf::Rect<float>(m_lights[i].position.x, m_lights[i].position.y, 1, 1).intersects(sf::Rect<float>(ptr_view->getCenter().x - ptr_view->getSize().x/2, ptr_view->getCenter().y - ptr_view->getSize().y/2, ptr_view->getSize().x, ptr_view->getSize().x)))
			{
				m_lights[i].m_draw = true;
			}else
			{
				m_lights[i].m_draw = false;
			}
		}
	}
}

Light* LightManager::add(Light & p_light){
	m_lights.push_back(p_light);
	return &m_lights[m_lights.size() - 1];
}