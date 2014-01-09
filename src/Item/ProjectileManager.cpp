#include "Item\ProjectileManager.h"

ProjectileManager::ProjectileManager(){}
ProjectileManager::~ProjectileManager(){}

void ProjectileManager::update(){
	for (unsigned int i = 0; i < m_arrows.size(); i++)
	{
		if (!m_arrows[i].dead)
		{
			m_arrows[i].update();
		}else
		{
			m_arrows.erase(m_arrows.begin() + i);
		}
	}
}

void ProjectileManager::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	for (unsigned int i = 0; i < m_arrows.size(); i++)
	{
		m_arrows[i].draw(target, states);
	}
}