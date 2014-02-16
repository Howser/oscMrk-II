#include "Application.h"

#include <iostream>

Application::Application()
	:
	mWindow(sf::VideoMode(1280, 720), "Descending Endora", sf::Style::Close),
	mTextures(),
	mFonts(),
	mMouse(&mWindow),
	mStateStack(State::Context(mWindow, mTextures, mFonts, mMouse)),
	mFrameCount(0),
	mFrameTimer(),
	mFrameCountText(),
	mFocused(true)
{
	mWindow.setMouseCursorVisible(false);
	mWindow.setVerticalSyncEnabled(true);
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

		lag += dt.getElapsedTime();
		dt.restart();

		handleEvents();

		while (lag >= updateTime)
		{
			update(updateTime);
			lag -= updateTime;
		}
		draw();
	}
}

void Application::update(sf::Time dt)
{
	if (mFocused)
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
		if (e.type == sf::Event::GainedFocus){
			mFocused = true;
		} else if (e.type == sf::Event::LostFocus) {
			mFocused = false;
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
	mStateStack.registerState<LoseState>(States::Lose);
	mStateStack.registerState<WinState>(States::Win);
	mStateStack.pushState(States::Title);
}

void Application::loadResources()
{
	try 
	{
		// Textures
		mTextures.loadTexture(Textures::Titlescreen, "resources/graphics/titlescreen.png");
		mTextures.loadTexture(Textures::Losescreen, "resources/graphics/losescreen.png");
		mTextures.loadTexture(Textures::Winscreen, "resources/graphics/winscreen.png");
		mTextures.loadTexture(Textures::Mini_Map_sheet, "resources/graphics/map/mini.png");
		mTextures.loadTexture(Textures::Prison_Sheet, "resources/graphics/map/tileset.png");
		mTextures.loadTexture(Textures::Cave_Sheet, "resources/graphics/map/cave_tileset.png");
		mTextures.loadTexture(Textures::Hell_Sheet, "resources/graphics/map/tileset.png");
		mTextures.loadTexture(Textures::Button, "resources/graphics/gui/baseButton.png");
		mTextures.loadTexture(Textures::Player, "resources/graphics/player/player.png");
		mTextures.loadTexture(Textures::MouseClick, "resources/graphics/gui/cursor_click.png");
		mTextures.loadTexture(Textures::Mouse, "resources/graphics/gui/cursor.png");	
		mTextures.loadTexture(Textures::InventorySlot, "resources/graphics/gui/inventorySlot.png");
		mTextures.loadTexture(Textures::StackManager, "resources/graphics/gui/stackManager.png");
		mTextures.loadTexture(Textures::Tooltip, "resources/graphics/gui/tooltip.png");
		mTextures.loadTexture(Textures::Border, "resources/graphics/gui/border.png"); 
		mTextures.loadTexture(Textures::DeleteItem, "resources/graphics/gui/deleteItem.png");
		mTextures.loadTexture(Textures::PlayerOverlay, "resources/graphics/player/overlay.png");
		mTextures.loadTexture(Textures::HealthFull, "resources/graphics/player/healthFull.png");
		mTextures.loadTexture(Textures::HealthEmpty, "resources/graphics/player/healthEmpty.png");
		mTextures.loadTexture(Textures::LoadingScreen, "resources/graphics/loadingscreen.png");
		mTextures.loadTexture(Textures::Ability, "resources/graphics/player/ability.png");
		mTextures.loadTexture(Textures::gui_Inventory, "resources/graphics/player/inventory.png");
		mTextures.loadTexture(Textures::gui_LootInventory, "resources/graphics/player/loot_inventory.png");

#pragma region Spells
		mTextures.loadTexture(Textures::TestAoe, "resources/graphics/item/world/testAOE.png");
#pragma endregion

#pragma region Mob Textures
		mTextures.loadTexture(Textures::Skeleton, "resources/graphics/Mob/skeleton.png");
		mTextures.loadTexture(Textures::Spider, "resources/graphics/Mob/spider.png");
		mTextures.loadTexture(Textures::TestMobDead, "resources/graphics/Mob/dead.png");
		mTextures.loadTexture(Textures::Boss, "resources/graphics/Mob/boss.png");
#pragma endregion

#pragma region Item Textures
		mTextures.loadTexture(Textures::Arrow, "resources/graphics/item/inv/arrow.png");
		mTextures.loadTexture(Textures::Helmet_Destruction, "resources/graphics/item/inv/helmet_destruction.png");
		mTextures.loadTexture(Textures::Helmet_Darkness, "resources/graphics/item/inv/helmet_darkness.png");
		mTextures.loadTexture(Textures::Helmet_Chaos, "resources/graphics/item/inv/helmet_chaos.png");
		mTextures.loadTexture(Textures::Armor_Destruction, "resources/graphics/item/inv/armor_destruction.png");
		mTextures.loadTexture(Textures::Armor_Darkness, "resources/graphics/item/inv/armor_darkness.png");
		mTextures.loadTexture(Textures::Armor_Chaos, "resources/graphics/item/inv/armor_chaos.png");
		mTextures.loadTexture(Textures::Bow, "resources/graphics/item/inv/bow.png");

		mTextures.loadTexture(Textures::d_Arrow, "resources/graphics/item/world/arrow.png");
		mTextures.loadTexture(Textures::d_Helmet_Destruction, "resources/graphics/item/world/helmet_destruction.png");
		mTextures.loadTexture(Textures::d_Helmet_Darkness, "resources/graphics/item/world/helmet_darkness.png");
		mTextures.loadTexture(Textures::d_Helmet_Chaos, "resources/graphics/item/world/helmet_chaos.png");
		mTextures.loadTexture(Textures::d_Armor_Destruction, "resources/graphics/item/world/armor_destruction.png");
		mTextures.loadTexture(Textures::d_Armor_Darkness, "resources/graphics/item/world/armor_darkness.png");
		mTextures.loadTexture(Textures::d_Armor_Chaos, "resources/graphics/item/world/armor_chaos.png");
		mTextures.loadTexture(Textures::d_Bow, "resources/graphics/item/world/test.png");
		mTextures.loadTexture(Textures::d_Armor_None, "resources/graphics/item/world/armor_none.png");
		mTextures.loadTexture(Textures::d_Helmet_None, "resources/graphics/item/world/helmet_none.png");
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