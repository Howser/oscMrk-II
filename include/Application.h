#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>
#include <SFML\System\Time.hpp>
#include <SFML\System\Clock.hpp>
#include <SFML\System\NonCopyable.hpp>

#include "StateStack\StateStack.h"
#include "StateStack\TitleState.h"
#include "StateStack\MenuState.h"
#include "StateStack\GameState.h"
#include "StateStack\LoseState.h"
#include "StateStack\WinState.h"

#include "ResourceHolders\FontHolder.h"

#include "Gui\Mouse.h"


class Application : private sf::NonCopyable
{
public:
	Application();

	void					run();
	bool					mFocused;


private:
	void					update(sf::Time dt);
	void					draw();
	void					handleEvents();

	void					updateFrameCount(sf::Time dt);

	void					registerStates();
	void					loadResources();

private:

	StateStack				mStateStack;
	sf::RenderWindow		mWindow;
	TextureHolder			mTextures;
	FontHolder				mFonts;

	int						mFrameCount;
	sf::Time				mFrameTimer;
	sf::Text				mFrameCountText;

	gui::Mouse				mMouse;

};