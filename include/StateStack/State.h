#pragma once

#include <SFML\Window\Event.hpp>
#include <SFML\System\Time.hpp>

#include <memory>

#include "StateIdentifiers.h"
#include "ResourceHolders\TextureHolder.h"
#include "ResourceHolders\FontHolder.h"

#include "Gui\Mouse.h"

namespace sf
{
	class RenderWindow;
}

class StateStack;

class State 
{
public:
	typedef std::unique_ptr<State> ptr;

	struct Context
	{
		Context(sf::RenderWindow& window, TextureHolder& textureHolder, FontHolder& fontHolder, gui::Mouse& mouse);

		sf::RenderWindow*	window;
		TextureHolder*		textures;
		FontHolder*			fonts;
		gui::Mouse*			mouse;
	};

public:
	State(StateStack& stateStack, Context context, States::ID id);
	virtual				~State();

	virtual bool		update(sf::Time dt) = 0;
	virtual void		draw() = 0;
	virtual bool		handleEvents(const sf::Event& event) = 0;

	bool				isPersistent() const;
	void				setPersistent(bool persitent);

	virtual States::ID	getID() const;

protected:
	void				requestStackPush(States::ID stateID);
	void				requestStackPop();
	void				requestStackClear();

	Context				getContext() const;

private:
	StateStack*			mStack;
	Context				mContext;
	bool				mIsPersistent;
	States::ID			mId;
};