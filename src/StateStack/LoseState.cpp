#include <StateStack\LoseState.h>

LoseState::LoseState(StateStack& stateStack, Context context, States::ID id)
	: 
	State(stateStack, context, id),
	m_menu(context.textures->getTexture(Textures::Button), context.fonts->getFont(Fonts::Main), false),
	m_quit(context.textures->getTexture(Textures::Button), context.fonts->getFont(Fonts::Main), false),
	m_background(*context.textures->getTexture(Textures::Losescreen))
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

LoseState::~LoseState(){}

bool LoseState::update(sf::Time dt){
	return true;
}

void LoseState::draw() {
	sf::RenderWindow* window = getContext().window;

	window->draw(m_menu);
	window->draw(m_quit);
	window->draw(m_background);
}

bool LoseState::handleEvents(sf::Event const& event){

	m_menu.handleEvent(event);
	m_quit.handleEvent(event);

	return false;
}