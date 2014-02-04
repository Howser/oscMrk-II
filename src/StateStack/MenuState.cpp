#include "StateStack\MenuState.h"
#include "Gui\Label.h"

#include <SFML\Graphics\RenderWindow.hpp>
#include <memory>


MenuState::MenuState(StateStack& stateStack, Context context, States::ID id)
	:
	State(stateStack, context, id),
	m_play_button(context.textures->getTexture(Textures::Button), context.fonts->getFont(Fonts::Main)),
	m_exit_button(context.textures->getTexture(Textures::Button), context.fonts->getFont(Fonts::Main)),
	m_background(*context.textures->getTexture(Textures::Titlescreen)),
	m_background_move(true),
	m_button_fade(false)
{
	// Play button
	m_play_button.setAction([this] ()
	{
		requestStackPop();
		requestStackPush(States::Game);
	});
	m_play_button.setText("Play");
	m_play_button.setPosition(1280/2 - 20, 720/2 + 40);

	// Exit button
	m_exit_button.setAction([this] ()
	{
		requestStackClear();
	});
	m_exit_button.setText("Exit");
	m_exit_button.setPosition(1280/2 - 20, 720/2 + 80);

	// Background thing
	m_background.setPosition(640 - m_background.getLocalBounds().width/2, 360 - m_background.getLocalBounds().height/2);

	context.mouse->setState(gui::Mouse::Neutral);
}

MenuState::~MenuState()
{
}

bool MenuState::update(sf::Time dt)
{
	if (m_background_move)
	{
		if (m_background.getPosition().y > 15)
		{
			m_background.move(0, -m_background.getPosition().y * 0.05);
		}
		else 
		{
			m_background_move = false;
			m_button_fade = true;
		}
	}
	else if (m_button_fade)
	{
		m_button_fade = !m_play_button.fade_in();
		m_exit_button.fade_in();
	}
	return false;
}

void MenuState::draw()
{
	sf::RenderWindow* window = getContext().window;

	window->clear(sf::Color::Black);

	window->draw(m_play_button);
	window->draw(m_exit_button);

	window->draw(m_background);
}

bool MenuState::handleEvents(const sf::Event& event)
{
	if (!m_background_move && !m_button_fade)
	{
		m_play_button.handleEvent(event);
		m_exit_button.handleEvent(event);
	}
	if (event.KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
			requestStackClear();
	}
	return false;
}