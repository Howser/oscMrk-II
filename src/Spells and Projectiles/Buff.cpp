#include "Spells and Projectiles\SpellTypeManager.h"

_BUFF::Buff::Buff(int* ptr_value, Items & p_item) : ptr_value(ptr_value), m_buff(p_item), m_duration(_BUFF::GetDuration(m_buff)), m_interval(_BUFF::GetInterval(m_buff)){}

_BUFF::Buff::Buff() : ptr_value(NULL), m_buff(Items::NOITEM), m_duration(0.f), m_interval(0.f){}

_BUFF::Buff::~Buff(){}

void _BUFF::Buff::update(sf::Time & p_dt){
	if (m_duration >= 0)
	{
		if (m_interval > 0)
		{
			m_interval -= p_dt.asSeconds();
		}else
		{
			_BUFF::execute(ptr_value, m_buff);
			m_interval = _BUFF::GetInterval(m_buff);
		}
		m_duration -= p_dt.asSeconds();
	}
}