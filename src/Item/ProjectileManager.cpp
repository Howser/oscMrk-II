#include "Item\ProjectileManager.h"

ProjectileManager::ProjectileManager(){}
ProjectileManager::ProjectileManager(MobManager* const& p_mobManager) : m_mobManager(p_mobManager){}
ProjectileManager::~ProjectileManager(){}

void ProjectileManager::update(){
	std::cout << m_mobManager->AliveMobs() << std::endl;
	for (unsigned int i = 0; i < m_arrows.size(); i++)
	{
		if (!m_arrows[i].dead)
		{
			m_arrows[i].update();
			// For hitting mobs
			Mob* mob = m_mobManager->getAtPosition(m_arrows[i].getPosition());
			if (mob != nullptr)
			{
				mob->takeDamage(m_arrows[i].m_damage);
				m_arrows[i].dead = true;
			}
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