#pragma once

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Drawable.hpp> 
#include <SFML\System\Time.hpp>

#include <vector>

#include "Spells and Projectiles\ProjectileManager.h"
#include "Spells and Projectiles\SpellTypeManager.h"
#include "Spells and Projectiles\Arrow.h"
#include "Spells and Projectiles\Spell.h"
#include "ResourceHolders\TextureHolder.h"
#include "ResourceHolders\FontHolder.h"
#include "Animation\Animation.h"
#include "Inventory\Inventory.h"
#include "Item\BaseItem.h"
#include "Item\GearItem.h"
#include "Item\ItemManager.h"
#include "Inventory\GearSlots.h"
#include "Mob\Mob.h"
#include "Inventory\StackManager.h"
#include "Inventory\Tooltip.h"
#include "Inventory\DeleteItem.h"
#include "Gui\HealthBar.h"

#define LOOT_DISTANCE 100
#define MELEE_DISTANCE 30

struct D_Gear;

enum Direction{
	Up,
	Down,
	Left,
	Right
};

enum Attack{
	aLeft,
	aRight,
};

class Player : 
	public sf::Transformable, 
	public sf::Drawable
{
public:
	Player(TextureHolder* textures, FontHolder* fonts, std::vector<Mob*>* mobsPtr, ProjectileManager* p_projectile_manager, std::vector<std::vector<gen::Tile>>* ptr_tiles, LightManager* ptr_light_manager);
	virtual ~Player();

	void stop();
	void path(std::vector<sf::Vector2f>& path);

	void update(sf::Time dt, sf::RenderWindow const& window);
	void updateInventory(sf::RenderWindow const& window, TextureHolder & textures);
	void handleEvents(sf::Event const& event);
	void drawGUI(sf::RenderWindow* p_window, FontHolder* ptr_font_holder);

	bool isInRange(Mob* ptr_mob);
	void attack(const sf::RenderWindow & window, Attack p_attack);
	void spell_attack(const Items & p_item, const sf::RenderWindow & p_window);

	bool isPathing();

	Items m_HelmetSlot;
	Items m_ArmorSlot;
	Items m_lHandSlot;
	Items m_rHandSlot;

	Inventory m_inventory;
	GearSlots m_Gear;
	Slot m_mouseSlot;
	Inventory m_lootInventory;

	StackManager m_stackManager;
	Tooltip m_tooltip;
	DeleteItem m_deleteItem;

	std::vector<Mob*>* p_mobs;
	std::vector<int> m_lootMobs;

	bool inventoryState, lootState;
	std::vector<sf::Vector2f> m_path;
	sf::Vector2f m_velocity;

	int m_health;

	mobtree::Tree* ptr_tree;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void resetInputs();

	sf::Sprite m_sprite;

	Animation m_animation;
	sf::Rect<int> m_invRect, m_lootInvRect;
	std::vector<std::vector<gen::Tile>>* ptr_tiles;

	///<summary>[0] = background, [1] = left hand, [2] = right hand.</summary>
	sf::Sprite m_ability[3];
	sf::Sprite m_gui_inventory, m_gui_loot_inventory;

	///<summary>Quantity for the left/right hand.</summary>
	int m_ability_q_1, m_ability_q_2;

	sf::Sprite m_overlay;
	gui::HealthBar m_healthbar;

	bool tabClicked;
	bool leftMouseClicked, rightMouseClicked;
	int mouseWheelDelta;
	
	TextureHolder* p_texture_holder;

	///<summary>[0]Helmet, [1]Armor, [2]LHand, [3]RHand</summary>
	std::vector<D_Gear> m_d_gear;
	ProjectileManager* p_projectile_manager;
	LightManager* ptr_light_manager;

	sf::Clock m_attackTimer;

	Direction m_dir;
};

///<summary>Used to draw the gear on the player.</summary>
struct D_Gear : public sf::Drawable, public sf::Transformable{
	sf::Sprite m_sprite;
	Animation m_animation;
	void draw(sf::RenderTarget & target, sf::RenderStates states)const;
};