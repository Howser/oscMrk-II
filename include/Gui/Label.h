#pragma once

#include "Component.h"

#include <SFML\Graphics\Text.hpp>

namespace gui
{
	class Label : public Component
	{
		public:
							Label(std::string const& text, sf::Font* font);
			virtual			~Label();

			virtual bool			isSelectable() const; // returns false
			virtual void	handleEvent(sf::Event const& event);

			void			setText(std::string const& text);
			void			setFontSize(int size);

		private:
			virtual	void	draw(sf::RenderTarget& target, sf::RenderStates states) const;

		private:
			sf::Text		mText;
	};
}