#include "StateStack\TitleState.h"

#include <SFML\Graphics\RenderWindow.hpp>


TitleState::TitleState(StateStack& stateStack, Context context, States::ID id)
	:
	State(stateStack, context, id),
	mTitleSprite()
{
	mTitleSprite.setTexture(*(getContext().textures->getTexture(Textures::Titlescreen)));
	mTitleSprite.setPosition(640 - mTitleSprite.getTextureRect().width/2, 360 - mTitleSprite.getTextureRect().height/2);
	context.mouse->setState(gui::Mouse::Invalid);
}

TitleState::~TitleState()
{

}

bool TitleState::update(sf::Time dt)
{
	return false;
}

bool TitleState::handleEvents(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}
	return false;
}

void TitleState::draw()
{
	sf::RenderWindow* window = getContext().window;
	window->draw(mTitleSprite);
}