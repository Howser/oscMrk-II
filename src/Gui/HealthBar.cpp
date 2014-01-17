#include "Gui/HealthBar.h"

#include "SFML\Graphics\RenderTarget.hpp"
#include "SFML\Graphics\RenderStates.hpp"

gui::HealthBar::HealthBar(sf::Texture const& base, sf::Texture const& loaded)
	:
	gui::StatusBar(base, loaded),
	m_animation(sf::Vector2i(156, 128), 6)
{
	m_animation.loop(0);
}

gui::HealthBar::~HealthBar() {}

void gui::HealthBar::updateStatus(float x)
{
	mStatus = x;
	if (mStatus < 0)
		mStatus = 0;
	if (mStatus > 1)
		mStatus = 1;
	float y = mSize.y * (1.f-mStatus);
	//mLoaded.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(mSize.x, mSize.y-y)));
	mLoaded.setPosition(0, y);
}

void gui::HealthBar::updateStatus(float current, float max) // this shouldn't be used... 
{
	mStatus = current / max;
	if (mStatus < 0)
		mStatus = 0;
	if (mStatus > 1)
		mStatus = 1;
	mLoaded.setTextureRect(sf::IntRect(sf::Vector2i(0, mSize.y * (1.f-mStatus)), sf::Vector2i(mSize.x, mSize.y)));
}

void gui::HealthBar::update()
{
	m_animation.update();
	mLoaded.setTextureRect(m_animation.getFrame());
}

void gui::HealthBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(mLoaded, states);
	target.draw(mBase, states);
}