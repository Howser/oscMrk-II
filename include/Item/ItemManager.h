#pragma once
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\System\Time.hpp>
#include <string>
#include "Items.h"

static enum eGearSlot{
	lHand,
	rHand,
	TwoHand,
	OneHand,
	Helmet,
	Armor,
	NoSlot,
};

static enum itemType{
	Normal,
	Gear,
};

///<summary>Strings that equate to the enum names.</summary>
static const char* ItemNames[10] = {"Arrow", "Helmet_Destruction", "Helmet_Darkness", "Helmet_Chaos", "Armor_Destruction", "Armor_Darkness", "Armor_Chaos", "Bow", "Test_Spell", "Test_AOE"};

///<summary>Strings for item names to display in tooltips, etc.</summary>
static const char* w_ItemNames[10] = {"Arrow", "Helmet of Destruction", "Helmet of Darkness", "Helmet of Chaos", "Armor of Destruction", "Armor of Darkness", "Armor of Chaos", "Bow", "Test Spell", "Test AOE"};

static const char* GearSlotNames[6] = {"Left Hand", "Right Hand", "Two Hander", "One Hand", "Helmet", "Armor"};

///<summary>Returns the maximum stack size for an item. Standard = 20 at the moment.</summary>
static int StackSize(const Items & item){
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

///<summary>Returns either "Normal", or "Gear"</summary>
static itemType GetType(const Items & item){
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

static int GetDamage(const Items & item){
	switch (item)
	{
	case Bow:
		return 5;
		break;
	default:
		return 0;
		break;
	}
}

///<summary>Returns the time between attacks for an item.</summary>
static float GetSpeed(const Items & item){
	switch (item)
	{
	case Bow:
		return 0.4f;
		break;
	case TestSpell:
		return 0.01f;
		break;
	case Arrow:
		return 10.f;
		break;
	default:
		return 0;
		break;
	}
}

///<summary>Returns the armor amount for an item. This value will be used when calculating the amount of damage to deal to the player.</summary>
static int GetArmor(const Items & item){
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

///<summary>Returns the slot. Can either be "Helmet", "Armor", "TwoHand", "OneHand", or "NoSlot" for items that cannot be equipped.</summary>
static eGearSlot GetSlot(const Items & item){
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

///<summary>Returns an item required to attack with the given item.</summary>
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

///<summary>FORMAT: TYPE, case gear{GEAR SLOT, case weapon{DAMAGE, SPEED} case armor{ARMOR}} case misc{} case consumable{}</summary>
static std::vector<std::string> GetStats(Items const& item){
	std::vector<std::string> stats;
	stats.push_back((std::string)w_ItemNames[item]);
	if (GetType(item) == itemType::Gear)
	{
		if (GetSlot(item) == eGearSlot::rHand || GetSlot(item) == eGearSlot::lHand || GetSlot(item) == eGearSlot::OneHand|| GetSlot(item) == eGearSlot::TwoHand)
		{
			//weapon
			//stats += "\nDamage: " + std::to_string(GetDamage(item)) + "\nSlot: " + GearSlotNames[GetSlot(item)] + "\nSpeed: " + std::to_string(GetSpeed(item));
			
			stats.push_back("Damage: " + std::to_string(GetDamage(item)));
			stats.push_back("Slot: " + (std::string)GearSlotNames[GetSlot(item)]);
			stats.push_back("Speed: " + std::to_string(GetSpeed(item)));
			//stats.erase (stats.find_last_not_of('0') + 1, std::string::npos);
		}else
		{
			//armor
			stats.push_back("Armor: " + std::to_string(GetArmor(item)));
		}
		return stats;
	}else if (GetType(item) == itemType::Normal)
	{
		return stats;
	}
}