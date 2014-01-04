#include "Gui\Label.h"

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

gui::Label::Label(std::string const& text, sf::Font* font)
	:
	mText(text, *font, 16)
{
}

gui::Label::~Label()
{
}

bool gui::Label::isSelectable() const
{
	return false;
}

void gui::Label::handleEvent(sf::Event const& event)
{

}

void gui::Label::setText(std::string const& text)
{
	mText.setString(text);
}

void gui::Label::setFontSize(int size)
{
	mText.setCharacterSize(size);
}

void gui::Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mText, states);
}