#include <StateStack\WinState.h>

WinState::WinState(StateStack& stateStack, Context context, States::ID id)
	: 
	State(stateStack, context, id),
	m_menu(context.textures->getTexture(Textures::Button), context.fonts->getFont(Fonts::Main), false),
	m_quit(context.textures->getTexture(Textures::Button), context.fonts->getFont(Fonts::Main), false),
	m_background(*context.textures->getTexture(Textures::Winscreen))
{
	m_menu.setPosition(1280/2, 720/2);
	m_menu.setText("Menu");
	m_menu.setAction([this] () {
		requestStackPop();
		requestStackPush(States::Menu);
	});
	
	m_quit.setPosition(1280/2, 720/2 + 40);
	m_quit.setText("Quit");
	m_quit.setAction([this] () {
		requestStackClear();
	});

	m_background.setPosition(640 - m_background.getLocalBounds().width/2, 0);
}

WinState::~WinState(){}

bool WinState::update(sf::Time dt){
	return false;
}

void WinState::draw() {
	sf::RenderWindow* window = getContext().window;

	window->draw(m_menu);
	window->draw(m_quit);
	window->draw(m_background);
}

bool WinState::handleEvents(sf::Event const& event){

	m_menu.handleEvent(event);
	m_quit.handleEvent(event);

	return false;
}