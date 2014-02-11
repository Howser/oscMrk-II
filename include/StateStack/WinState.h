#pragma once

#include "State.h"

#include <Gui\Button.h>

class WinState : public State 
{
	public:
		WinState(StateStack& stateStack, Context context, States::ID id);
		virtual			~WinState();

		bool			update(sf::Time dt);
		void			draw();
		bool			handleEvents(sf::Event const& event);

	private:
		
		gui::Button		m_menu;
		gui::Button		m_quit;

		sf::Sprite		m_background;
};