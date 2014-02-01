#include "Item\ItemManager.h"
#include <SFML\System\Time.hpp>

struct Buff{
	Buff(int* ptr_value, Items & p_buff);
	Buff();
	~Buff();

	void update(sf::Time & p_dt);

	float m_duration;

	///<summary>An id generated from the player class.</summary>
	int ID;

private:
	int* ptr_value;
	float m_interval;
	Items m_buff;
};