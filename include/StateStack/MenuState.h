#pragma once

#include "State.h"
#include "Gui\Container.h"
#include "Gui\Button.h"

#include <SFML\Graphics\Sprite.hpp>

class MenuState : public State
{
	public:
								MenuState(StateStack& stateStack, Context context, States::ID id);
		virtual					~MenuState();

		bool					update(sf::Time dt);
		void					draw();
		bool					handleEvents(const sf::Event& event);

	private:
		gui::Container			mButtonContainer;
};