#include "StateStack\MenuState.h"
#include "Gui\Label.h"

#include <SFML\Graphics\RenderWindow.hpp>
#include <memory>


MenuState::MenuState(StateStack& stateStack, Context context, States::ID id)
	:
	State(stateStack, context, id),
	mButtonContainer()
{
	std::unique_ptr<gui::Button> exit(new gui::Button(context.textures->getTexture(Textures::Button), 
		context.fonts->getFont(Fonts::Main)));
	exit->setAction([this] ()
	{
		requestStackPop();
	});
	exit->setText("Exit");
	exit->setPosition(0, 40);

	std::unique_ptr<gui::Button> play(new gui::Button(context.textures->getTexture(Textures::Button), 
		context.fonts->getFont(Fonts::Main)));
	play->setAction([this] ()
	{
		requestStackPop();
		requestStackPush(States::Game);
	});
	play->setText("Play");
	
	std::unique_ptr<gui::Label> text(new gui::Label("Menu", context.fonts->getFont(Fonts::Main)));
	text->setPosition(10, -40);
	text->setFontSize(36);

	mButtonContainer.add(std::move(play));
	mButtonContainer.add(std::move(exit));
	mButtonContainer.add(std::move(text));

	mButtonContainer.setPosition(400, 400);
	mButtonContainer.select();

	context.mouse->setState(gui::Mouse::Neutral);
}

MenuState::~MenuState()
{
}

bool MenuState::update(sf::Time dt)
{
	return false;
}

void MenuState::draw()
{
	sf::RenderWindow* window = getContext().window;

	window->clear(sf::Color::Black);
	window->draw(mButtonContainer);
}

bool MenuState::handleEvents(const sf::Event& event)
{
	mButtonContainer.handleEvent(event);
	return false;
}