#include "Gui/HealthBar.h"
#include <iostream>

gui::HealthBar::HealthBar(sf::Texture const& base, sf::Texture const& loaded)
	:
	gui::StatusBar(base, loaded)	
{

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
	mLoaded.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(mSize.x, mSize.y-y)));
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