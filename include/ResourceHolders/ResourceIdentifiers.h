#pragma once


namespace Textures
{
	enum ID
	{
		Tilesheet,
		Mini_Map_sheet,
		Titlescreen,
		Button,
		Player,
		TestMob,
		TestMobDead,
		MouseClick,
		Mouse,
		InventorySlot,
		StackManager,
		Tooltip,
		Border,
		DeleteItem,
		PlayerOverlay,
		HealthFull,
		HealthEmpty,
		LoadingScreen,
		Ability,
		gui_Inventory,
		gui_LootInventory,

		///<summary>Items.</summary>
		Arrow,
		Helmet_Destruction,
		Helmet_Darkness,
		Helmet_Chaos,
		Armor_Destruction,
		Armor_Darkness,
		Armor_Chaos,
		Shield,
		Sword,
		Bow,
		Mace,

		///<summary>Spells.</summary>
		TestAoe,

		///<summary>Items that will be displayed on the palyer.</summary>
		d_Arrow,
		d_Helmet_Destruction,
		d_Helmet_Darkness,
		d_Helmet_Chaos,
		d_Armor_Destruction,
		d_Armor_Darkness,
		d_Armor_Chaos,
		d_Shield,
		d_Sword,
		d_Bow,
		d_Mace,
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
	};
}