#pragma once
#include <SFML\System\Time.hpp>
#include "Effects\Lights\Light.h"
#include <SFML\System\Vector3.hpp>
#include <iostream>
#include "Item\Items.h"

namespace _SPELL{

	static enum Spell{
		TestSpell,
		TestAOE,
		BossFireCircle,
		BossLaser,
		NOSPELL,
	};

	static Items ToItem(const Spell & p_spell){
		switch (p_spell)
		{
		case TestSpell:
			return Items::TestSpell;
			break;
		case TestAOE:
			return Items::TestAOE;
			break;
		case BossFireCircle:
			return Items::BossFireCircle;
			break;
		case BossLaser:
			return Items::BossLaser;
			break;
		default:
			std::cout << "!!!ERROR!!!: SPELL: " << p_spell << " is NOT an ITEM!\n";
			return Items::NOITEM;
			break;
		}
	}

	///<summary>Seconds, returns -1 for no timer.</summary>
	static float GetTimer(const Spell & p_spell){
		switch (p_spell)
		{
		case TestSpell:
			return -1;
			break;
		case BossLaser:
			return 2;
			break;
		case BossFireCircle:
			return 5;
			break;
		default:
			std::cout << "!!!ERROR!!!: SPELL: " << p_spell << " is not implemented for 'GetTimer'!\n";
			return -1;
			break;
		}
	}

	static Spell ToSpell(const Items & p_item){
		switch (p_item)
		{
		case Items::TestSpell:
			return Spell::TestSpell;
			break;
		case Items::TestAOE:
			return Spell::TestAOE;
			break;
		case Items::BossFireCircle:
			return Spell::BossFireCircle;
			break;
		case Items::BossLaser:
			return Spell::BossLaser;
			break;
		default:
			std::cout << "!!!ERROR!!!: ITEM: " << p_item << " is NOT a SPELL!\n";
			return NOSPELL;
			break;
		}
	}

	static bool IsSpell(const Items & item){
		switch (item)
		{
		case Items::Helmet_Destruction:
		case Items::Helmet_Darkness:
		case Items::Helmet_Chaos:
		case Items::Armor_Chaos:
		case Items::Armor_Darkness:
		case Items::Armor_Destruction:
		case Items::Bow:
			return false;
			break;

		case Items::TestSpell:
			return true;
			break;
		case Items::TestAOE:
			return true;
			break;
		case Items::BossFireCircle:
			return true;
			break;
		case Items::BossLaser:
			return true;
			break;
		default:
			return false;
			break;
		}
	}

	///<summary>Returns the damage for spell-type attacks and buffs/debuffs.</summary>
	static int GetDamage(const Spell & p_spell){
		switch (p_spell)
		{
		case Spell::BossFireCircle:
		case Spell::BossLaser:
			return 2;
			break;

		default:
			return 1;
			break;
		}
	}

	static Light GetLight(const Spell & p_spell){
		switch (p_spell)
		{
		default:
			return Light(sf::Color(255, 80, 40, 100), sf::Vector3f(0, 0, 0.075f), sf::Vector3f(0.f, 5.f, 0.f), false);
			break;
		}
	}

	static float GetSpeed(const Spell & p_spell){
		switch (p_spell)
		{
		case Spell::BossFireCircle:
			return 3;
			break;
		case Spell::BossLaser:
			return 4;
			break;
		default:
			return 1.f;
			break;
		}
	}
}

namespace _AOE{
	static float GetRadius(const _SPELL::Spell & p_spell){
		switch (p_spell)
		{
		default:
			return 121;
			break;
		}
		return 0;
	}

	///<summary>Seconds.</summary>
	static float GetDuration(const _SPELL::Spell & p_spell){
		switch (p_spell)
		{
		default:
			return 2.5f;
			break;
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
	static float GetDuration(const _SPELL::Spell & p_spell){
		switch (p_spell)
		{
		default:
			return 9.f;
			break;
		}
		return 0.f;
	}

	///<summary>Seconds.</summary>
	static float GetInterval(const _SPELL::Spell & p_spell){
		switch (p_spell)
		{
		default:
			return 0.5f;
			break;
		}
		return 0.f;
	}

	static void execute(int* ptr_value, const _SPELL::Spell & p_spell){
		switch (p_spell)
		{
		default:
			(*ptr_value) -= _SPELL::GetDamage(p_spell);
			break;
		}
	}

	static Buff GetBuff(const Items & p_item){
		return Buff(nullptr, (Items)p_item);
	}
}