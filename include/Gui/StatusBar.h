#pragma once

#include "Component.h"
#include <SFML\Graphics\Sprite.hpp>

namespace gui
{
	class StatusBar : public Component
	{
		public:
							StatusBar(sf::Texture const& base, sf::Texture const& loaded);
			virtual			~StatusBar();

			virtual void	updateStatus(float x);
			virtual void	updateStatus(float current, float max);

			virtual bool	isSelectable() const;
		
			virtual void	handleEvent(sf::Event const& event);
			virtual void	draw(sf::RenderTarget& target, sf::RenderStates states) const;

		protected:
			sf::Sprite		mBase;			// This is the base of the bar
			sf::Sprite		mLoaded;		// This is what fills it up, it should be the whole bar loaded
			float			mStatus;		// Between 0.0 and 1.0 shows how much the bar is filled
			sf::Vector2u	mSize;
	};
}