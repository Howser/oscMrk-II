#include "Spells and Projectiles\ProjectileManager.h"

ProjectileManager::ProjectileManager(){}
ProjectileManager::ProjectileManager(MobManager* const& p_mobManager, ParticleSystem* const& p_particleSystem) : 
	m_mobManager(p_mobManager),
	m_particleSystem(p_particleSystem)
{}
ProjectileManager::~ProjectileManager(){}

void ProjectileManager::update(sf::Time & p_dt){
	for (int i = 0; i < m_AOE_spells.size(); i++)
	{
		if (!m_AOE_spells[i].m_dead)
		{
			///<summary>An array of points to outline the border of the AOE spell in order to get the correct mob tree branches to search.</summary>
			sf::Vector2<int> points[2] = {sf::Vector2<int>(m_AOE_spells[i].getPosition().x - m_AOE_spells[i].m_radius, m_AOE_spells[i].getPosition().y - m_AOE_spells[i].m_radius), sf::Vector2<int>(m_AOE_spells[i].getPosition().x + m_AOE_spells[i].m_radius, m_AOE_spells[i].getPosition().y + m_AOE_spells[i].m_radius)};
			///<summary>A vector of branches to check for mobs that are within the radius of the AOE spell.</summary>
			std::vector<mobtree::Branch*> ptr_branches;

			for (int x = points[0].x/mobtree::SIZE, y = points[0].y/mobtree::SIZE; x < points[1].x/mobtree::SIZE; x++)
			{
				for (int y = points[0].y/mobtree::SIZE; y < points[1].y/mobtree::SIZE; y++)
				{
					mobtree::Branch* ptr_branch = m_mobManager->m_tree.search(sf::Vector2f(x*mobtree::SIZE, y*mobtree::SIZE));
					if (ptr_branch != NULL)
					{
						ptr_branches.push_back(ptr_branch);
					}
				}
			}
			for (int l = 0; l < ptr_branches.size(); l++)
			{
				for (int j = 0; j < ptr_branches[l]->mobs.size(); j++)
				{
					bool broken = false;
					for (int b = 0; b < ptr_branches[l]->mobs[j]->m_buffs.size(); b++)
					{
						if (ptr_branches[l]->mobs[j]->m_buffs[b].ID == m_AOE_spells[i].ID)
						{
							broken = true;
							break;
						}
					}
					if (broken)
					{
						break;
					}
					if (math::distance(ptr_branches[l]->mobs[j]->getPosition(), m_AOE_spells[i].getPosition()) <= m_AOE_spells[i].m_radius)
					{
						ptr_branches[l]->mobs[j]->m_buffs.push_back(buff::GetBuff(m_AOE_spells[i].m_spell_type));
						ptr_branches[l]->mobs[j]->m_buffs.back().ID = m_AOE_spells[i].ID;
					}
				}
			}
			m_AOE_spells[i].update(p_dt);
		}else
		{
			m_AOE_spells.erase(m_AOE_spells.begin() + i);
		}
	}
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
				m_spells[i].m_light.color.a *= 0.9f;
				m_spells[i].m_light.color.r *= 0.9f;
				m_spells[i].m_light.color.g *= 0.9f;
				m_spells[i].m_light.color.b *= 0.9f;
				if (m_spells[i].m_light.color.a <= 1)
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
	for (int i = 0; i < m_AOE_spells.size(); i++)
	{
		if (!m_AOE_spells[i].m_dead)
		{
			m_AOE_spells[i].draw(target, states);
		}
	}
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

namespace buff{
	///<summary>Get the targeted variable.</summary>
	static void GetTarget(Buff* ptr_buff, Mob* ptr_mob){
		switch (ptr_buff->m_buff)
		{
		default:
			ptr_buff->ptr_value = &ptr_mob->health;
			break;
		}
	}
};