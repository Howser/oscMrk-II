#include "Gui\StatusBar.h"

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

#include <SFML\Graphics\Texture.hpp>

gui::StatusBar::StatusBar(sf::Texture const& base, sf::Texture const& loaded)
	:
	mBase(base),
	mLoaded(loaded)
{
	mSize = loaded.getSize();
}

gui::StatusBar::~StatusBar()
{
}

void gui::StatusBar::updateStatus(float x)
{
	mStatus = x;
	if (mStatus < 0)
		mStatus = 0;
	if (mStatus > 1)
		mStatus = 1;
	mLoaded.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(mSize.x * mStatus, mSize.y)));
}

void gui::StatusBar::updateStatus(float current, float max)
{
	mStatus = current / max;
	if (mStatus < 0)
		mStatus = 0;
	if (mStatus > 1)
		mStatus = 1;
	mLoaded.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(mSize.x * mStatus, mSize.y)));
}

bool gui::StatusBar::isSelectable() const
{
	return false;
}

void gui::StatusBar::handleEvent(sf::Event const& event)
{
}

void gui::StatusBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(mLoaded, states);
	target.draw(mBase, states);
}