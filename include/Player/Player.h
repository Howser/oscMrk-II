#pragma once

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\System\Time.hpp>

#include <vector>

#include "ResourceHolders\TextureHolder.h"
#include "Animation\Animation.h"
#include "Inventory\Inventory.h"
#include "Item\BaseItem.h"
#include "Item\GearItem.h"
#include "Inventory\GearSlots.h"
#include "Mob\Mob.h"
#include "Inventory\StackManager.h"
#include "Inventory\Tooltip.h"
#include "Inventory\DeleteItem.h"

#define TERMINAL_VELOCITY 250
#define LOOT_DISTANCE 32

class Player : 
	public sf::Transformable, 
	public sf::Drawable
{
public:
	Player(TextureHolder & textures, std::vector<Mob*>* mobsPtr);
	virtual ~Player();

	void stop();
	void path(std::vector<sf::Vector2f>& path);

	void update(sf::Time dt, sf::RenderWindow const& window);
	void updateInventory(sf::RenderWindow const& window, TextureHolder & textures);
	void handleEvents(sf::Event const& event);
	void drawGUI(sf::RenderWindow* p_window);

	bool isPathing();

	Items m_HelmetSlot;
	Items m_ChestpieceSlot;
	Items m_BootsSlot;
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
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void resetInputs();

	sf::Sprite m_sprite;

	Animation m_animation;
	sf::Vector2f m_velocity;
	sf::Rect<int> m_invRect, m_lootInvRect;

	std::vector<sf::Vector2f> m_path;
	unsigned int m_health;
	unsigned int m_damage;
	// unsigned int mArmor;

	bool tabClicked;
	bool leftMouseClicked, rightMouseClicked;
	int mouseWheelDelta;
};