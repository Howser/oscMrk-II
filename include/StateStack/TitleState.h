#pragma once

#include "State.h"

#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\Sprite.hpp>



class TitleState : public State
{
	public:

							TitleState(StateStack& stateStack, Context context, States::ID id);
	virtual 				~TitleState();

	bool					update(sf::Time dt);
	void					draw();
	bool					handleEvents(const sf::Event& event);

	private:
		sf::Sprite			mTitleSprite;

		int 				m_max_alpha;
		int 				m_min_alpha;
		int					m_current_alpha;
		bool				m_inc_alpha;
	
		sf::Text			m_text;
};