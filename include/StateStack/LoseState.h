#pragma once

#include "State.h"

#include <Gui\Button.h>

class LoseState : public State 
{
	public:
		LoseState(StateStack& stateStack, Context context, States::ID id);
		virtual			~LoseState();

		bool			update(sf::Time dt);
		void			draw();
		bool			handleEvents(sf::Event const& event);

	private:
		
		gui::Button		m_menu;
		gui::Button		m_quit;

		sf::Sprite		m_background;
};