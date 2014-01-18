#include "StateStack\GameState.h"
#include "Mob\MobTypeManager.h"

#include <iostream>

GameState::GameState(StateStack& stateStack, Context context, States::ID id)
	:
	State(stateStack, context, id),
	mMap(context.textures, context.fonts, &mobManager, &m_light_manager, mMutex),
	mapSize(mMap.size.x, mMap.size.y),
	mView(sf::Rect<float>(0, 0, context.window->getSize().x, context.window->getSize().y)),
	mNormalRender(),
	mDiffuseRender(),
	mNormalTextures(),
	mShader(),
	mobManager(),
	mPlayer(context.textures, context.fonts, &mobManager.mobs, &m_projectile_manager, &mMap.tiles, &m_light_manager),
	mPlayerController(&mPlayer, &pathFinder, &mMap, &mobManager),
	mParticleSystem(),
	mLoadingThread(&gen::Map::Gen, &mMap)
{
	mLoadingSprite.setTexture(*context.textures->getTexture(Textures::LoadingScreen));

	pathFinder = PathFinder(&mMap.tiles, mMap.size);
	mobManager = MobManager(*context.textures, &mMap.tiles, &pathFinder);
	m_projectile_manager = ProjectileManager(&mobManager, &mParticleSystem);

	mCurrentType = gen::Cave;
	mMap.type = mCurrentType;
	mView.zoom(1.f);

	sf::Vector2u size = context.window->getSize();
	mNormalRender.create(size.x, size.y);
	mDiffuseRender.create(size.x, size.y);

	mShader.loadFromFile("resources/shaders/shader.frag", sf::Shader::Fragment);
	loadNormals();



	mSpawnRadius = std::sqrtf((float)std::pow(size.x/2, 2) + (float)std::pow(size.y/2, 2));
	context.mouse->setState(gui::Mouse::Attack);


}

GameState::~GameState()
{
}

bool GameState::update(sf::Time dt)
{
	if (!mMap.loaded)
	{
		mLoading = true;
	}
	if (!mLoading)
	{
		mView.setCenter(mPlayer.getPosition());
		
		if (!mPlayer.inventoryState)
		{
			mPlayerController.update(dt, *getContext().window, mView, &m_light_manager);
			mPlayer.update(dt, *getContext().window);
			if (!mobManager.mobs.empty())
			{
				mobManager.Update(dt, mPlayer.getPosition());
			}
			mMap.update(dt);
			m_projectile_manager.update(dt);
			mParticleSystem.update(dt);
			m_light_manager.update(&mView, dt);
		}else
		{
			mPlayer.updateInventory(*getContext().window, *getContext().textures);
		}
	} else 
	{
		mMutex.lock();
		// Update loading bar
		if (!mMap.generating && !mMap.loaded)
		{
			mLoadingThread.launch();

			// CLEAR STUFF
			mobManager.mobs.clear();
			mobManager.minorMobs.clear();
			mobManager.majorMobs.clear();
			m_light_manager.m_lights.clear();
			mMap.mobSpawners.clear();
			mPlayer.m_path.clear();
			pathFinder.Clear();
			std::cout << "starting map gen" << std::endl;
		}
		else if (!mMap.generating && mMap.loaded)
		{
			// Set all the other map stuff after generating it
			mLoading = false;
			std::cout << "Finished gening map" << std::endl;
			if (mMap.rooms.size() > 0)
			{
				mView.setCenter(sf::Vector2f(mMap.rooms[0].x*32, mMap.rooms[0].y*32));
				mPlayer.setPosition((mMap.rooms[0].x + mMap.rooms[0].width/2) * 32, (mMap.rooms[0].y + mMap.rooms[0].height/2) *32);
			}else
			{
				mView.setCenter(mMap.size.x/2*32, mMap.size.y/2*32);
				mPlayer.setPosition(mMap.size.x/2*32, mMap.size.y/2*32);
			}
			for (unsigned int i = 0; i < mMap. mobSpawners.size(); i++)
			{
				if (mMap.mobSpawners[i].amount > 0 || mMap.mobSpawners[i].s_amount > 0)
				{
					sf::Vector2f pos(mMap.mobSpawners[i].x*WIDTH, mMap.mobSpawners[i].y*HEIGHT);
					if (math::distance(mPlayer.getPosition(), pos) < mapSize.x*WIDTH*2)
					{
						mMap.mobSpawners[i].SpawnMobs(&mobManager, mobManager.textures);
						mMap.mobSpawners[i].amount = 0;
						mMap.mobSpawners.erase(mMap.mobSpawners.begin() + i);
					}
				}
			}
			mMap.mobSpawners.clear();
			for (int i = 0; i < mobManager.minorMobs.size(); i++)
			{
				mobManager.minorMobs[i].ID = mobManager.mobs.size();
				mobManager.mobs.push_back(&mobManager.minorMobs[i]);
			}
			for (int i = 0; i < mobManager.majorMobs.size(); i++)
			{
				mobManager.majorMobs[i].ID = mobManager.mobs.size();
				mobManager.mobs.push_back(&mobManager.majorMobs[i]);
			}
			mobManager.Build_Tree();

			pathFinder.mapSize = mMap.size;
			pathFinder.GetMap(&mMap.tiles);

			Light l1(sf::Color(175, 175, 175, 150), sf::Vector3f(0.5f, 0.5f, .075f), sf::Vector3f(0.f, 5.f, 0.f), true);
			m_light_manager.m_lights.push_back(l1);
		}
		mMutex.unlock();
	}
	return false;
}

bool GameState::handleEvents(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
			requestStackPop();
		else if (event.key.code == sf::Keyboard::Tab){

		} else if (event.key.code == sf::Keyboard::Space) {
			mMap.loaded = false;
		}
	}
	mPlayerController.handleEvents(event);
	mPlayer.handleEvents(event);
	return false;
}

void GameState::draw()
{
	sf::RenderWindow* window = getContext().window;

	if (!mLoading)
	{
		// set the camera of the map
		mMap.setView(mView);
		mobManager.SetView(mView);

		// Render everything to the diffuse map
		mDiffuseRender.clear();
		mDiffuseRender.setView(mView);

		// Map
		mMap.SetBounds();
		mDiffuseRender.draw(mMap);

		// Mobs and player
		mDiffuseRender.draw(m_projectile_manager);
		mDiffuseRender.draw(mobManager);
		mDiffuseRender.draw(mPlayer);
		mDiffuseRender.draw(mParticleSystem);

		mDiffuseRender.display();

		// Render normals to the normal map
		mNormalRender.clear();
		mNormalRender.setView(mView);
		mNormalRender.draw(mMap, mNormalTextures.getTexture(Textures::Tilesheet));
		mNormalRender.display();

		// Render diffuse texture to window using shader with normal map
		// Set the all the shader properties

		mShader.setParameter("texture", sf::Shader::CurrentTexture);
		mShader.setParameter("normal", mNormalRender.getTexture());
		mShader.setParameter("Resolution", window->getSize().x,  window->getSize().y);
		mShader.setParameter("AmbientColor", .1f, .1f, .1f, 0.f);

		passLightsToShader(&mShader, m_light_manager.m_lights, &mView);

		sf::Sprite sprite(mDiffuseRender.getTexture());
		window->setView(window->getDefaultView());
		window->draw(sprite, &mShader);
		mMap.draw_mini_map(window, mView.getCenter().x, mView.getCenter().y);
		mPlayer.drawGUI(window);
	} 
	else 
	{
		// Draw loading state
		window->setView(window->getDefaultView());
		window->draw(mLoadingSprite);
	}
}

void GameState::loadNormals()
{
	mNormalTextures.loadTexture(Textures::Tilesheet, "resources/graphics/map/normals.png");
}