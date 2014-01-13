#include "Item\ProjectileManager.h"

ProjectileManager::ProjectileManager(){}
ProjectileManager::ProjectileManager(MobManager* const& p_mobManager) : m_mobManager(p_mobManager){}
ProjectileManager::~ProjectileManager(){}

void ProjectileManager::update(){
	for (unsigned int i = 0; i < m_arrows.size(); i++)
	{
		if (!m_arrows[i].dead)
		{
			m_arrows[i].update();
			Mob* mob = m_mobManager->getAtPosition(m_arrows[i].getPosition());
			if (mob != nullptr && !mob->dead)
			{
				mob->takeDamage(m_arrows[i].m_damage);
				mob->StickArrow(m_arrows[i]);
				m_arrows[i].kill();
				m_arrows.erase(m_arrows.begin() + i);
			}
		}
	}
}

void ProjectileManager::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	for (unsigned int i = 0; i < m_arrows.size(); i++)
	{
		m_arrows[i].draw(target, states);
	}
}