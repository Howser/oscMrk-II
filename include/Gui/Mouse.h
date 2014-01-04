#pragma once

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Mouse.hpp>


#include "Animation\Animation.h"
#include "ResourceHolders\TextureHolder.h"

namespace gui
{
	class Mouse : public sf::Drawable, public sf::Transformable
	{
		public:
			enum State
			{
				Neutral,
				Attack,
				Invalid
			};

		public:
								Mouse(sf::RenderWindow* window);
			virtual				~Mouse();

			void				update(sf::Time dt);

			void				setTextures(TextureHolder* textures);
								
								///<summary>Changes the state of the mouse for displaying different sprites at different times.</summary>
			void				setState(State state);

								///<summary>Get's the mouse position relative to the window.</summary>
			sf::Vector2i		getLocalMousePosition() const;
								///<summary>Get's the mouse position in the world.</summary>
			sf::Vector2i		getWorldMousePosition() const;

			void				handleEvent(sf::Event const& event);

		private:
			void				draw(sf::RenderTarget& target, sf::RenderStates states) const;
			void				click(int x, int y);

		private:
			sf::Sprite			mMouseSprite;
			sf::Sprite			mClickSprite;
			
			Animation			mClickAnimation;
			State				mState;

			sf::RenderWindow*	mWindow;
	};
}