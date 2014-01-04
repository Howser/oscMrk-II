#pragma once

#include "State.h"
#include "Inventory\Inventory.h"
#include "Inventory\GearSlots.h"
#include "Item\GearItem.h"
#include "Player\Player.h"

#include <SFML\Graphics\Sprite.hpp>

class InventoryState : public State
{
	public:
								InventoryState(StateStack& stateStack, Context context, States::ID id);
		virtual 				~InventoryState();

		bool					update(sf::Time dt);
		void					draw();
		bool					handleEvents(const sf::Event& event);

		States::ID				getID() const;

		Inventory* inventory;
		GearSlots GearState;
		Slot mouseSlot;

		bool leftMousePressed, rightMousePressed;
	private:
		sf::Sprite				mSprite;
};