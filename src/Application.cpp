#include "Application.h"

#include <iostream>

Application::Application()
	:
	mWindow(sf::VideoMode(1280, 720), "OscMrk-II", sf::Style::Close),
	mTextures(),
	mFonts(),
	mMouse(&mWindow),
	mStateStack(State::Context(mWindow, mTextures, mFonts, mMouse)),
	mFrameCount(0),
	mFrameTimer(),
	mFrameCountText()
{
	mWindow.setFramerateLimit(60);
	mWindow.setMouseCursorVisible(false);
}

void Application::run()
{
	loadResources();
	registerStates();

	sf::Clock dt;
	sf::Time lag;
	sf::Time updateTime = sf::seconds(1.f/60.f);

	while (mWindow.isOpen())
	{
		updateFrameCount(dt.getElapsedTime());

		/*lag += dt.getElapsedTime();
		dt.restart();*/

		handleEvents();

		/*while (lag >= updateTime)
		{

			lag -= updateTime;
		}*/
		update(updateTime);
		draw();
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
	if (mStateStack.isEmpty())
	{
		mWindow.close();
	}
	mMouse.update(dt);
}

void Application::draw()
{
	mWindow.clear(sf::Color::Black);

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mFrameCountText);
	mWindow.draw(mMouse);
	mWindow.display();
}

void Application::handleEvents()
{
	sf::Event e;
	while (mWindow.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			mWindow.close();
		}
		mStateStack.handleEvent(e);
		mMouse.handleEvent(e);
	}
}

void Application::updateFrameCount(sf::Time dt)
{
	mFrameCount++;
	mFrameTimer += dt;
	if (mFrameTimer.asSeconds() >= 1.0f)
	{
		mFrameCountText.setString("FPS: " + std::to_string(mFrameCount));
		mFrameCount = 0;
		mFrameTimer = sf::Time::Zero;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<InventoryState>(States::Inventory);
	mStateStack.pushState(States::Title);
}

void Application::loadResources()
{
	try 
	{
		// Textures
		mTextures.loadTexture(Textures::Titlescreen, "resources/graphics/titlescreen.png");
		mTextures.loadTexture(Textures::Mini_Map_sheet, "resources/graphics/map/mini.png");
		mTextures.loadTexture(Textures::Inventory, "resources/graphics/inventoryyo.png");
		mTextures.loadTexture(Textures::Tilesheet, "resources/graphics/map/tileset.png");
		mTextures.loadTexture(Textures::Button, "resources/graphics/gui/baseButton.png");
		mTextures.loadTexture(Textures::Player, "resources/graphics/player/player.png");
		mTextures.loadTexture(Textures::MouseClick, "resources/graphics/gui/cursor_click.png");
		mTextures.loadTexture(Textures::Mouse, "resources/graphics/gui/cursor.png");	
		mTextures.loadTexture(Textures::InventorySlot, "resources/graphics/gui/inventorySlot.png");
		mTextures.loadTexture(Textures::StackManager, "resources/graphics/gui/stackManager.png");
		mTextures.loadTexture(Textures::Tooltip, "resources/graphics/gui/tooltip.png");
		mTextures.loadTexture(Textures::DeleteItem, "resources/graphics/gui/deleteItem.png");

#pragma region Mob Textures
		mTextures.loadTexture(Textures::TestMob, "resources/graphics/Mob/test.png");
		mTextures.loadTexture(Textures::TestMobDead, "resources/graphics/Mob/dead.png");
#pragma endregion

#pragma region Item Textures
		mTextures.loadTexture(Textures::Arrow, "resources/graphics/item/inv/arrow.png");
		mTextures.loadTexture(Textures::Helmet_Pain, "resources/graphics/item/inv/test.png");
		mTextures.loadTexture(Textures::Helmet_Doubt, "resources/graphics/item/inv/test.png");
		mTextures.loadTexture(Textures::Helmet_Torture, "resources/graphics/item/inv/test.png");
		mTextures.loadTexture(Textures::Chest_Suffering, "resources/graphics/item/inv/test.png");
		mTextures.loadTexture(Textures::Chest_Cold, "resources/graphics/item/inv/test.png");
		mTextures.loadTexture(Textures::Chest_Destruction, "resources/graphics/item/inv/test.png");
		mTextures.loadTexture(Textures::Legs_Enslaved, "resources/graphics/item/inv/test.png");
		mTextures.loadTexture(Textures::Legs_Darkness, "resources/graphics/item/inv/test.png");
		mTextures.loadTexture(Textures::Legs_Chaos, "resources/graphics/item/inv/test.png");
		mTextures.loadTexture(Textures::Shield, "resources/graphics/item/inv/test.png");
		mTextures.loadTexture(Textures::Sword, "resources/graphics/item/inv/sword.png");
		mTextures.loadTexture(Textures::Bow, "resources/graphics/item/inv/bow.png");
		mTextures.loadTexture(Textures::Mace, "resources/graphics/item/inv/mace.png");

		mTextures.loadTexture(Textures::d_Arrow, "resources/graphics/item/world/arrow.png");
		mTextures.loadTexture(Textures::d_Helmet_Pain, "resources/graphics/item/world/test.png");
		mTextures.loadTexture(Textures::d_Helmet_Doubt, "resources/graphics/item/world/test.png");
		mTextures.loadTexture(Textures::d_Helmet_Torture, "resources/graphics/item/world/test.png");
		mTextures.loadTexture(Textures::d_Chest_Suffering, "resources/graphics/item/world/test.png");
		mTextures.loadTexture(Textures::d_Chest_Cold, "resources/graphics/item/world/test.png");
		mTextures.loadTexture(Textures::d_Chest_Destruction, "resources/graphics/item/world/test.png");
		mTextures.loadTexture(Textures::d_Legs_Enslaved, "resources/graphics/item/world/test.png");
		mTextures.loadTexture(Textures::d_Legs_Darkness, "resources/graphics/item/world/test.png");
		mTextures.loadTexture(Textures::d_Legs_Chaos, "resources/graphics/item/world/test.png");
		mTextures.loadTexture(Textures::d_Shield, "resources/graphics/item/world/test.png");
		mTextures.loadTexture(Textures::d_Sword, "resources/graphics/item/world/test.png");
		mTextures.loadTexture(Textures::d_Bow, "resources/graphics/item/world/test.png");
		mTextures.loadTexture(Textures::d_Mace, "resources/graphics/item/world/test.png");
#pragma endregion

		// Fonts
		mFonts.loadFont(Fonts::Main, "resources/fonts/00TT.TTF");

		// Debug info
		mFrameCountText.setPosition(10, 10);
		mFrameCountText.setColor(sf::Color::White);
		mFrameCountText.setCharacterSize(16);
		mFrameCountText.setFont(*mFonts.getFont(Fonts::Main));
	}
	catch (std::runtime_error e)
	{
		std::cout << "EXCEPTION: " << e.what() << std::endl;
		system("pause");
		exit(-1);
	}

	mMouse.setTextures(&mTextures);

}