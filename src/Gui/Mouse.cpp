#include "Gui\Mouse.h"

#include <SFML\Window\Event.hpp>

gui::Mouse::Mouse(sf::RenderWindow* window)
	:
	mMouseSprite(),
	mClickSprite(),
	mClickAnimation(sf::Vector2i(32, 32), 8, .05),
	mState(Mouse::Neutral),
	mWindow(window)
{
	mMouseSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));

	mClickSprite.setOrigin(16, 16);
}

gui::Mouse::~Mouse()
{
}

void gui::Mouse::update(sf::Time dt)
{
	int width = mMouseSprite.getTextureRect().width;
	int height = mMouseSprite.getTextureRect().height;
	mMouseSprite.setTextureRect(sf::IntRect(mState*width, 0, width, height));

	mClickAnimation.update();
	mClickSprite.setTextureRect(mClickAnimation.getFrame());
}

void gui::Mouse::setTextures(TextureHolder* textures)
{
	mMouseSprite.setTexture(*textures->getTexture(Textures::Mouse));
	mClickSprite.setTexture(*textures->getTexture(Textures::MouseClick));
}

void gui::Mouse::setState(State state)
{
	mState = state;
}

void gui::Mouse::click(int x, int y)
{
	mClickSprite.setPosition(x, y);
	mClickAnimation.play(0);
}

sf::Vector2i gui::Mouse::getLocalMousePosition() const
{
	return sf::Mouse::getPosition(*mWindow);
}

sf::Vector2i gui::Mouse::getWorldMousePosition() const
{
	sf::Vector2i pos = getLocalMousePosition();
	pos.x += mWindow->getView().getCenter().x - mWindow->getView().getSize().x;
	pos.y += mWindow->getView().getCenter().y - mWindow->getView().getSize().y;
	return pos;
}

void gui::Mouse::handleEvent(sf::Event const& event)
{
	if (event.type == sf::Event::MouseButtonPressed 
		&& event.mouseButton.button == sf::Mouse::Button::Left)
	{
		click(getLocalMousePosition().x, getLocalMousePosition().y);
	}
	else if (event.type == sf::Event::MouseMoved)
	{
		sf::Vector2f mp = sf::Vector2f(getLocalMousePosition().x, getLocalMousePosition().y);
		mMouseSprite.setPosition(mp);
	}
}

void gui::Mouse::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.setView(target.getDefaultView());

	target.draw(mMouseSprite);

	if (mClickAnimation.isPlaying())
	{
		target.draw(mClickSprite);
	}
}