#include "StateStack\TitleState.h"

#include <SFML\Graphics\RenderWindow.hpp>


TitleState::TitleState(StateStack& stateStack, Context context, States::ID id)
	:
	State(stateStack, context, id),
	mTitleSprite(),
	m_text("Press any button to continue", *context.fonts->getFont(Fonts::Main)),
	m_max_alpha(255),
	m_min_alpha(100),
	m_current_alpha(160),
	m_inc_alpha(true)
{
	mTitleSprite.setTexture(*(getContext().textures->getTexture(Textures::Titlescreen)));
	mTitleSprite.setPosition(640 - mTitleSprite.getTextureRect().width/2, 360 - mTitleSprite.getTextureRect().height/2);
	context.mouse->setState(gui::Mouse::Invalid);

	m_text.setPosition(1280/2 + 100, 720-100);
	m_text.setColor(sf::Color(255,255,255,m_current_alpha));
}

TitleState::~TitleState()
{

}

bool TitleState::update(sf::Time dt)
{
	if (m_inc_alpha)
	{
		m_current_alpha += 3;
		if (m_current_alpha >= m_max_alpha)
		{
			m_current_alpha = m_max_alpha;
			m_inc_alpha = false;
		}
	}
	else 
	{
		m_current_alpha -= 3;
		if (m_current_alpha <= m_min_alpha)
		{
			m_current_alpha = m_min_alpha;
			m_inc_alpha = true;
		}
	}

	m_text.setColor(sf::Color(255, 255, 255, m_current_alpha));
	
	return false;
}

bool TitleState::handleEvents(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}
	return false;
}

void TitleState::draw()
{
	sf::RenderWindow* window = getContext().window;
	window->draw(mTitleSprite);
	window->draw(m_text);
}