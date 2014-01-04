#include "Gui\Button.h"

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

gui::Button::Button(sf::Texture* texture, sf::Font* font)
	:
	mTexture(texture),
	mText(),
	mSprite()
{
	mSprite.setTexture(*texture);
	mText.setFont(*font);
	mText.setCharacterSize(22);
	deactivate();
	mText.setColor(sf::Color::Black);
}

gui::Button::~Button()
{
}

bool gui::Button::isSelectable() const
{
	return true;
}

void gui::Button::setText(std::string text)
{
	// Set the string
	mText.setString(text);
	mText.setOrigin(mText.getLocalBounds().width/2, mText.getLocalBounds().height);

	// Set the position of the text 
	mText.setPosition(sf::Vector2f(mTexture->getSize() / 2u));
}

void gui::Button::activate()
{
	Component::activate();
	mAction();
	Component::deactivate();
}

void gui::Button::handleEvent(sf::Event const& event)
{

}

void gui::Button::setAction(std::function<void()> const& action) 
{
	mAction = action;
}

void gui::Button::select()
{
	Component::select();
	mText.setColor(sf::Color::White);
}

void gui::Button::deselect()
{
	Component::deselect();
	mText.setColor(sf::Color::Black);
}

void gui::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mText, states);
}