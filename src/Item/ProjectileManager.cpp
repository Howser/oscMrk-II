#include "Item\ProjectileManager.h"

ProjectileManager::ProjectileManager(){}
ProjectileManager::ProjectileManager(MobManager* const& p_mobManager, ParticleSystem* const& p_particleSystem) : 
	m_mobManager(p_mobManager),
	m_particleSystem(p_particleSystem)
{}
ProjectileManager::~ProjectileManager(){}

void ProjectileManager::update(sf::Time & p_dt){
	for (int i = 0; i < m_spells.size(); i++)
	{
		switch (m_spells[i].m_item)
		{
		case TestSpell:
			if (!m_spells[i].dead)
			{
				m_spells[i].update(p_dt);
				Mob* mob = m_mobManager->getAtPosition(m_spells[i].getPosition());
				if (mob != nullptr && !mob->dead)
				{
					mob->takeDamage(m_spells[i].m_damage);
					m_spells[i].kill();
				}
			}else
			{
				m_spells[i].kill();
				m_spells.erase(m_spells.begin() + i);
			}
			break;
		}
	}
	for (int i = 0; i < m_arrows.size(); i++)
	{
		switch (m_arrows[i].m_item)
		{
		case Arrow:
			if (!m_arrows[i].dead)
			{
				m_arrows[i].update(p_dt);
				Mob* mob = m_mobManager->getAtPosition(m_arrows[i].getPosition());
				if (mob != nullptr && !mob->dead)
				{
					mob->takeDamage(m_arrows[i].m_damage);
					m_arrows[i].kill();
					m_arrows.erase(m_arrows.begin() + i);
				}
			}
			break;
		}
	}
}

void ProjectileManager::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	for (int i = 0; i < m_spells.size(); i++)
	{
		if (!m_spells[i].dead)
		{
			m_spells[i].draw(target, states);
		}
	}
	for (int i = 0; i < m_arrows.size(); i++)
	{
		m_arrows[i].draw(target, states);
	}
}