#pragma once
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\System\Time.hpp>
#include <string>

enum Items //sort by type
{
	Arrow,
	Helmet_Destruction,
	Helmet_Darkness,
	Helmet_Chaos,
	Armor_Destruction,
	Armor_Darkness,
	Armor_Chaos,
	Bow,
	TestSpell,
	TestAOE,
	NOITEM,
	end,
};

///<summary>Strings that equate to the enum names.</summary>
static const char* ItemNames[10] = {"Arrow", "Helmet_Destruction", "Helmet_Darkness", "Helmet_Chaos", "Armor_Destruction", "Armor_Darkness", "Armor_Chaos", "Bow", "Test_Spell", "Test_AOE"};

///<summary>Strings for item names to display in tooltips, etc.</summary>
static const char* w_ItemNames[10] = {"Arrow", "Helmet of Destruction", "Helmet of Darkness", "Helmet of Chaos", "Armor of Destruction", "Armor of Darkness", "Armor of Chaos", "Bow", "Test Spell", "Test AOE"};

enum eGearSlot{
	lHand,
	rHand,
	TwoHand,
	OneHand,
	Helmet,
	Armor,
	NoSlot,
};

static const char* GearSlotNames[6] = {"Left Hand", "Right Hand", "Two Hander", "One Hand", "Helmet", "Armor"};

enum itemType{
	Normal,
	Gear,
};

static int StackSize(Items const& item){
	switch (item)
	{
	case Helmet_Destruction:
		return 1;
		break;
	case Helmet_Darkness:
		return 1;
		break;
	case Helmet_Chaos:
		return 1;
		break;
	case Armor_Chaos:
		return 1;
		break;
	case Armor_Darkness:
		return 1;
		break;
	case Armor_Destruction:
		return 1;
		break;
	case Bow:
		return 1;
		break;
	case TestSpell:
		return 20;
		break;
	default:
		return 20;
		break;
	}
}

static itemType GetType(Items item){
	switch (item)
	{
	case Arrow:
		return Normal;
		break;
	case Helmet_Destruction:
		return Gear;
		break;
	case Helmet_Darkness:
		return Gear;
		break;
	case Helmet_Chaos:
		return Gear;
		break;
	case Armor_Chaos:
		return Gear;
		break;
	case Armor_Darkness:
		return Gear;
		break;
	case Armor_Destruction:
		return Gear;
		break;
	case Bow:
		return Gear;
		break;
	case TestSpell:
		return Gear;
		break;
	case TestAOE:
		return Gear;
		break;
	default:
		return Normal;
		break;
	}
}

static int GetDamage(Items item){
	switch (item)
	{
	case Bow:
		return 5;
		break;
	case TestSpell:
		return 5;
		break;
	case TestAOE:
		return 5;
		break;
	default:
		return 0;
		break;
	}
}

static float GetSpeed(Items item){
	switch (item)
	{
	case Bow:
		return 0.4f;
		break;
	case TestSpell:
		return 0.01f;
		break;
	default:
		return 0;
		break;
	}
}

static int GetArmor(Items item){
	switch (item)
	{
	case Helmet_Destruction:
		return 1;
		break;
	case Helmet_Darkness:
		return 1;
		break;
	case Helmet_Chaos:
		return 1;
		break;
	case Armor_Chaos:
		return 1;
		break;
	case Armor_Darkness:
		return 1;
		break;
	case Armor_Destruction:
		return 1;
		break;
	default:
		return 0;
		break;
	}
}

static eGearSlot GetSlot(Items item){
	switch (item)
	{
	case Helmet_Destruction:
		return Helmet;
		break;
	case Helmet_Darkness:
		return Helmet;
		break;
	case Helmet_Chaos:
		return Helmet;
		break;
	case Armor_Chaos:
		return Armor;
		break;
	case Armor_Darkness:
		return Armor;
		break;
	case Armor_Destruction:
		return Armor;
		break;
	case Bow:
		return TwoHand;
		break;
	case TestSpell:
		return OneHand;
		break;
	case TestAOE:
		return OneHand;
		break;
	default:
		return NoSlot;
		break;
	}
}

static Items GetAmmo(const Items & p_item){
	switch (p_item)
	{
	case Items::TestSpell:
		return TestSpell;
		break;
	case TestAOE:
		return TestAOE;
		break;
	case Items::Bow:
		return Arrow;
		break;
	default:
		return p_item;
		break;
	}
	return p_item;
}

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

namespace _AOE{
	static float GetRadius(const Items & p_item){
		if (IsSpell(p_item))
		{
			switch (p_item)
			{
			default:
				return 242/2;
				break;
			}
		}
		return 0;
	}

	///<summary>Seconds.</summary>
	static float GetDuration(const Items & p_item){
		if (IsSpell(p_item))
		{
			switch (p_item)
			{
			default:
				return .25f;
				break;
			}
		}
		return 0;
	}
}

eGearSlot;
///<summary>FORMAT: TYPE, case gear{GEAR SLOT, case weapon{DAMAGE, SPEED} case armor{ARMOR}} case misc{} case consumable{}</summary>
static std::string GetStats(Items const& item, sf::Vector2i* size){
	sf::Text text;
	std::string stats = (std::string)w_ItemNames[item];
	if (GetType(item) == itemType::Gear)
	{
		if (GetSlot(item) == eGearSlot::rHand || GetSlot(item) == eGearSlot::lHand || GetSlot(item) == eGearSlot::OneHand|| GetSlot(item) == eGearSlot::TwoHand)
		{
			//weapon
			stats += "\nDamage: " + std::to_string(GetDamage(item)) + "\nSlot: " + GearSlotNames[GetSlot(item)] + "\nSpeed: " + std::to_string(GetSpeed(item));
			stats.erase (stats.find_last_not_of('0') + 1, std::string::npos);
		}else
		{
			//armor
			stats += "\nArmor: " + std::to_string(GetArmor(item));
		}
		text.setString(stats);
		size->x = text.getLocalBounds().width;
		size->y = text.getLocalBounds().height;
		return stats;
	}else if (GetType(item) == itemType::Normal)
	{
		text.setString(stats);
		size->x = text.getLocalBounds().width;
		size->y = text.getLocalBounds().height;
		return stats;
	}
}

namespace buff{
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
			return 3.f;
			break;
		}
		return 0.f;
	}

	static void execute(int* ptr_value, const Items & p_item){
		switch (p_item)
		{
		default:
			(*ptr_value) = 0;
			break;
		}
	}

	static Buff GetBuff(const Items & p_item){
		return Buff(nullptr, (Items)p_item);
	}
}