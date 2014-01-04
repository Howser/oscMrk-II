#include <iostream>

#include "Gui\Container.h"

#include <SFML\Window\Event.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

gui::Container::Container()
	:
	mChildren(),
	mSelectedChild(-1)
{
}

gui::Container::~Container()
{
}

bool gui::Container::isSelectable() const
{
	return false;
}

void gui::Container::handleEvent(sf::Event const& event)
{
	// Pass on the event to the selected active item if there is one
	if (mSelectedChild != -1 && mChildren[mSelectedChild]->isActive())
	{
		mChildren[mSelectedChild]->handleEvent(event);
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Up)
		{
			selectPrevious();
		}
		else if (event.key.code == sf::Keyboard::Down)
		{
			selectNext();
		}
		else if (event.key.code == sf::Keyboard::Return 
			|| event.key.code == sf::Keyboard::Space)
		{
			if (mSelectedChild != -1)
				mChildren[mSelectedChild]->activate();
		}
	}
}

void gui::Container::add(gui::Component::ptr component)
{
	mChildren.push_back(std::move(component));
	if (mChildren.back()->isSelectable() && mSelectedChild == -1)
	{
		mSelectedChild = mChildren.size() - 1;
		mChildren[mSelectedChild]->select();
	}
}

void gui::Container::selectNext()
{
	// deselect the current item
	mChildren[mSelectedChild]->deselect();

	// Search for the next selectable item
	do
	{
		mSelectedChild = (mSelectedChild + 1) % mChildren.size();
	} while (!mChildren[mSelectedChild]->isSelectable());

	// Select it
	mChildren[mSelectedChild]->select();
}

void gui::Container::selectPrevious()
{
	// deselect the current item
	mChildren[mSelectedChild]->deselect();

	// Search for the next selectable item
	do
	{
		mSelectedChild = (mSelectedChild + mChildren.size() - 1) % mChildren.size();
	} while (!mChildren[mSelectedChild]->isSelectable());
	
	// Select it
	mChildren[mSelectedChild]->select();
}

void gui::Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
	{
		target.draw(*(i->get()), states);
	}
}