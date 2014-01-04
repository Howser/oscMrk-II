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
};