#pragma once
#include <SFML\Graphics\Sprite.hpp>
#include <string>

enum Items //sort by type
{
	Arrow,
	Helmet_Pain,
	Helmet_Doubt,
	Helmet_Torture,
	Chest_Suffering,
	Chest_Cold,
	Chest_Destruction,
	Legs_Enslaved,
	Legs_Darkness,
	Legs_Chaos,
	Shield,
	Sword,
	Bow,
	Mace,
	NOITEM,
	end,
};

///<summary>Strings that equate to the enum names.</summary>
static const char* ItemNames[14] = {"Arrow", "Helmet_Pain", "Helmet_Doubt", "Helmet_Torture", "Chest_Suffering", "Chest_Cold", "Chest_Destruction", "Legs_Enslaved", "Legs_Darkness", "Legs_Chaos", "Shield", "Sword", "Bow", "Mace"};

///<summary>Strings for item names to display in tooltips, etc.</summary>
static const char* w_ItemNames[14] = {"Arrow", "Helmet of Pain", "Helmet of Doubt", "Helmet of Torture", "Chest of Suffering", "Chest of Cold", "Chest of Destruction", "Legs of Enslaved", "Legs of Darkness", "Legs of Chaos", "Shield", "Sword", "Bow", "Mace"};

enum eGearSlot{
	lHand,
	rHand,
	TwoHand,
	Helmet,
	Chestpiece,
	Leggings,
	NoSlot,
};

static const char* GearSlotNames[6] = {"Left Hand", "Right Hand", "Two Hander", "Helmet", "Chest", "Legs"};

enum itemType{
	Normal,
	Gear,
};

static int StackSize(Items const& item){
	switch (item)
	{
	case Helmet_Pain:
		return 1;
		break;
	case Helmet_Doubt:
		return 1;
		break;
	case Helmet_Torture:
		return 1;
		break;
	case Chest_Suffering:
		return 1;
		break;
	case Chest_Cold:
		return 1;
		break;
	case Chest_Destruction:
		return 1;
		break;
	case Legs_Enslaved:
		return 1;
		break;
	case Legs_Darkness:
		return 1;
		break;
	case Legs_Chaos:
		return 1;
		break;
	case Shield:
		return 1;
		break;
	case Sword:
		return 1;
		break;
	case Bow:
		return 1;
		break;
	case Mace:
		return 1;
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
	case Helmet_Pain:
		return Gear;
		break;
	case Helmet_Doubt:
		return Gear;
		break;
	case Helmet_Torture:
		return Gear;
		break;
	case Chest_Suffering:
		return Gear;
		break;
	case Chest_Cold:
		return Gear;
		break;
	case Chest_Destruction:
		return Gear;
		break;
	case Legs_Enslaved:
		return Gear;
		break;
	case Legs_Darkness:
		return Gear;
		break;
	case Legs_Chaos:
		return Gear;
		break;
	case Shield:
		return Gear;
		break;
	case Sword:
		return Gear;
		break;
	case Bow:
		return Gear;
		break;
	case Mace:
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
	case Sword:
		return 5;
	case Mace:
		return 5;
		break;
	case Bow:
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
	case Sword:
		return 0.5f;
	case Mace:
		return 1.f;
		break;
	case Bow:
		return 0.7f;
		break;
	default:
		return 0;
		break;
	}
}

static int GetArmor(Items item){
	switch (item)
	{
	case Helmet_Pain:
		return 1;
		break;
	case Helmet_Doubt:
		return 5;
		break;
	case Helmet_Torture:
		return 12;
		break;
	case Chest_Suffering:
		return 3;
		break;
	case Chest_Cold:
		return 8;
		break;
	case Chest_Destruction:
		return 16;
		break;
	case Legs_Enslaved:
		return 2;
		break;
	case Legs_Darkness:
		return 7;
		break;
	case Legs_Chaos:
		return 14;
		break;
	case Shield:
		return 5;
		break;
	default:
		return 0;
		break;
	}
}

static eGearSlot GetSlot(Items item){
	switch (item)
	{
	case Helmet_Pain:
		return Helmet;
		break;
	case Helmet_Doubt:
		return Helmet;
		break;
	case Helmet_Torture:
		return Helmet;
		break;
	case Chest_Suffering:
		return Chestpiece;
		break;
	case Chest_Cold:
		return Chestpiece;
		break;
	case Chest_Destruction:
		return Chestpiece;
		break;
	case Legs_Enslaved:
		return Leggings;
		break;
	case Legs_Darkness:
		return Leggings;
		break;
	case Legs_Chaos:
		return Leggings;
		break;
	case Shield:
		return lHand;
		break;
	case Sword:
		return rHand;
		break;
	case Bow:
		return TwoHand;
		break;
	case Mace:
		return TwoHand;
		break;
	default:
		return NoSlot;
		break;
	}
}

///<summary>FORMAT: TYPE, case gear{GEAR SLOT, case weapon{DAMAGE, SPEED} case armor{ARMOR}} case misc{} case consumable{}</summary>
eGearSlot;
static std::string GetStats(Items const& item, sf::Vector2i* size){
	if (GetType(item) == itemType::Gear)
	{
		std::string stats = (std::string)w_ItemNames[item] + "\nSlot: " + GearSlotNames[GetSlot(item)];
		if (GetSlot(item) == eGearSlot::rHand ||GetSlot(item) == eGearSlot::TwoHand)
		{
			//weapon
			stats += "\nDamage: " + std::to_string(GetDamage(item)) + "\nSlot: " + GearSlotNames[GetSlot(item)] + "\nSpeed: " + std::to_string(GetSpeed(item));
			stats.erase ( stats.find_last_not_of('0') + 1, std::string::npos );
			size->x = 1;
			size->y = 1;
		}else
		{
			//armor
			stats += "\nArmor: " + std::to_string(GetArmor(item));
			size->x = 1;
			size->y = 1;
		}
		return stats;
	}else if (GetType(item) == itemType::Normal)
	{
		return (std::string)w_ItemNames[item] + "";
	}
}