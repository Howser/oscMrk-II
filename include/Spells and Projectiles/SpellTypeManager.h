#pragma once
#include <SFML\System\Time.hpp>
#include "Effects\Lights\Light.h"
#include <SFML\System\Vector3.hpp>
#include <iostream>
#include "Item\Items.h"

namespace _SPELL{
	static bool IsSpell(const Items & item){
		switch (item)
		{
		case Helmet_Destruction:
			return false;
			break;
		case Helmet_Darkness:
			return false;
			break;
		case Helmet_Chaos:
			return false;
			break;
		case Armor_Chaos:
			return false;
			break;
		case Armor_Darkness:
			return false;
			break;
		case Armor_Destruction:
			return false;
			break;
		case Bow:
			return false;
			break;
		case TestSpell:
			return true;
			break;
		case TestAOE:
			return true;
			break;
		default:
			return false;
			break;
		}
	}

	///<summary>Returns the damage for spell-type attacks and buffs/debuffs.</summary>
	static int GetDamage(const Items & p_item){
		switch (p_item)
		{
		default:
			return 1;
			break;
		}
	}
	
	static Light GetLight(const Items & p_item){
		switch (p_item)
		{
		default:
			return Light(sf::Color(255, 80, 40, 100), sf::Vector3f(0, 0, 0.075f), sf::Vector3f(0.f, 5.f, 0.f), false);
			break;
		}
	}

	static float GetSpeed(const Items & p_item){
		switch (p_item)
		{
		default:
			return 10.f;
			break;
		}
	}
}

namespace _AOE{
	static float GetRadius(const Items & p_item){
		if (_SPELL::IsSpell(p_item))
		{
			switch (p_item)
			{
			default:
				return 121;
				break;
			}
		}
		return 0;
	}

	///<summary>Seconds.</summary>
	static float GetDuration(const Items & p_item){
		if (_SPELL::IsSpell(p_item))
		{
			switch (p_item)
			{
			default:
				return 2.5f;
				break;
			}
		}
		return 0;
	}
}

namespace _BUFF{
	struct Buff{
		Buff(int* ptr_value, Items & p_buff);
		Buff();
		~Buff();

		void update(sf::Time & p_dt);

		float m_duration;

		///<summary>An id generated from the player class.</summary>
		int ID;

		int* ptr_value;
		float m_interval;
		Items m_buff;
	};

	///<summary>Seconds.</summary>
	static float GetDuration(const Items & p_item){
		switch (p_item)
		{
		default:
			return 9.f;
			break;
		}
		return 0.f;
	}

	///<summary>Seconds.</summary>
	static float GetInterval(const Items & p_item){
		switch (p_item)
		{
		default:
			return 0.5f;
			break;
		}
		return 0.f;
	}

	static void execute(int* ptr_value, const Items & p_item){
		switch (p_item)
		{
		default:
			(*ptr_value) -= _SPELL::GetDamage(p_item);
			break;
		}
	}

	static Buff GetBuff(const Items & p_item){
		return Buff(nullptr, (Items)p_item);
	}
}