#pragma once
#include <SFML\Graphics\Sprite.hpp>
#include <string>

enum Items //sort by type
{
	Test,
	TestWeapon,
	TestArmor,
	TestArmorH,
	TestArmorC,
	NOITEM,
	end,
};

static const char* ItemNames[2] = {"Test", "TestWeapon"};

enum eGearSlot{
	lHand,
	rHand,
	Helmet,
	Chestpiece,
	Leggings,
	NoSlot,
};

static const char* GearSlotNames[5] = {"Left Hand", "Right Hand", "Helmet", "Chest", "Legs"};

enum itemType{
	Normal,
	Gear,
};

static int StackSize(Items const& item){
	switch (item)
	{
	case Items::Test:
		return 20;
		break;
	default:
		return 1;
		break;
	}
}

static itemType GetType(Items item){
	switch (item)
	{
	case Test:
		return itemType::Normal;
		break;
	case TestWeapon:
		return itemType::Gear;
		break;
	case TestArmor:
		return itemType::Gear;
		break;
	case TestArmorH:
		return itemType::Gear;
		break;
	case TestArmorC:
		return itemType::Gear;
		break;
	case NOITEM:
		return itemType::Normal;
		break;
	default:
		return itemType::Normal;
		break;
	}
}

static int GetDamage(Items item){
	switch (item)
	{
	case TestWeapon:
		return 1;
		break;
	default:
		return 0;
		break;
	}
}

static int GetArmor(Items item){
	switch (item)
	{
	case TestArmor:
		return 1;
	case TestArmorH:
		return 2;
		break;
	case TestArmorC:
		return 2;
		break;
	default:
		return 0;
		break;
	}
}

static eGearSlot GetSlot(Items item){
	switch (item)
	{
	case TestWeapon:
		return lHand;
		break;
	case TestArmor:
		return Helmet;
		break;
	case TestArmorH:
		return Helmet;
		break;
	case TestArmorC:
		return Chestpiece;
		break;
	default:
		return NoSlot;
		break;
	}
}

///<summary>FORMAT: TYPE, case gear{GEAR SLOT, case weapon{DAMAGE, SPEED} case armor{ARMOR}} case misc{} case consumable{}</summary>
static std::string GetStats(Items const& item, sf::Vector2i* size){
	if (GetType(item) == itemType::Gear)
	{
		std::string stats = (std::string)ItemNames[item] + "\nSlot: " + GearSlotNames[GetSlot(item)];
		if (item >= Items::TestWeapon && item <= Items::TestWeapon)
		{
			stats += "\nDamage: " + std::to_string(GetDamage(item)) + "\nSpeed: " + "TODO: Make weapon speed";
			size->x = (6 + std::to_string(GetDamage(item)).length())*20;
			size->y = 4*20;
		}else
		{
			//armor
		}
		return stats;
	}else if (GetType(item) == itemType::Normal)
	{
		return (std::string)ItemNames[item] + "";
	}
}