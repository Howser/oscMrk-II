#include "StateStack\State.h"
#include "StateStack\StateStack.h"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, gui::Mouse& mouse)
	:
	window(&window),
	textures(&textures),
	fonts(&fonts),
	mouse(&mouse)
{
}

State::State(StateStack& stateStack, Context context, States::ID id)
	:
	mStack(&stateStack),
	mContext(context),
	mId(id),
	mIsPersistent(false)
{
}

State::~State()
{
}

bool State::isPersistent() const
{
	return mIsPersistent;
}

void State::setPersistent(bool persitent)
{
	mIsPersistent = persitent;
}

States::ID State::getID() const
{
	return mId;
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStackClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}