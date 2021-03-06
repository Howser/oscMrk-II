#include "StateStack\GameState.h"
#include "Mob\MobTypeManager.h"
#include "Math\GeneralMath.h"

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
	mLoadingThread(&gen::Map::Gen, &mMap),
	mFirstLoad(true),
	m_lost(false),
	m_win(false)
{
	mLoadingSprite.setTexture(*context.textures->getTexture(Textures::LoadingScreen));

	pathFinder = PathFinder(&mMap.tiles, mMap.size);
	mobManager = MobManager(*context.textures, &mMap.tiles, &pathFinder, &m_projectile_manager.m_spells, &m_projectile_manager.m_arrows);
	m_projectile_manager = ProjectileManager(&mobManager, &mParticleSystem);
	mobManager.ptr_spells = &m_projectile_manager.m_spells;
	mobManager.ptr_arrows = &m_projectile_manager.m_arrows;
	mPlayer.ptr_tree = &mobManager.m_tree;
	mPlayer.setPosition(-1280, -720);

	mCurrentType = gen::Cave;
	mMap.type = mCurrentType;
	mCurrentMapTexture = Textures::Cave_Sheet;
	mView.zoom(1.f);
	mView.setCenter(-mView.getSize().x, -mView.getSize().y);

	sf::Vector2u size = context.window->getSize();
	mNormalRender.create(size.x, size.y);
	mDiffuseRender.create(size.x, size.y);

	mShader.loadFromFile("resources/shaders/shader.frag", sf::Shader::Fragment);
	loadNormals();

	context.mouse->setState(gui::Mouse::Attack);

	mShader.setParameter("texture", sf::Shader::CurrentTexture);
	mShader.setParameter("normal", mNormalRender.getTexture());
	mShader.setParameter("Resolution", getContext().window->getSize().x,  getContext().window->getSize().y);
	mShader.setParameter("AmbientColor", .1f, .1f, .1f, 0.f);
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
			if (!m_lost){
				mPlayerController.update(dt, *getContext().window, mView, &m_light_manager);
				mPlayer.update(dt, *getContext().window);
			}
			//if (!mobManager.mobs.empty())
			{
				mobManager.Update(dt, mPlayer.getPosition(), &mPlayer.m_health);
			}
			mMap.update(dt);
			SetPlayerValues();
			m_projectile_manager.update(dt, m_player_values);
			mParticleSystem.update(dt);
			m_light_manager.update(&mView, dt);
		}else
		{
			mPlayer.updateInventory(*getContext().window, *getContext().textures);
		}
		if (mPlayer.m_health < 0 && !m_lost && !m_win) {
			requestStackPush(States::Lose);
			m_lost = true;
		} else if (m_lost) {
			m_light_manager.m_lights.front().m_alpha = -1;
			m_light_manager.m_lights.front().color.a *= 0.99;
		}
		m_all_lights = m_light_manager.m_lights;
		if (!m_projectile_manager.m_spells.empty())
		{
			if (m_all_lights.size() + m_projectile_manager.m_spells.size() <= 130)
			{
				m_all_lights.insert(m_all_lights.end(), m_projectile_manager.m_spells.begin(), m_projectile_manager.m_spells.end());
			}else
			{
				m_all_lights.insert(m_all_lights.end(), m_projectile_manager.m_spells.begin(), m_projectile_manager.m_spells.begin() + 130 - m_light_manager.m_lights.size());
				/*for (int i = 0; i < m_projectile_manager.m_spells.size(); i++)
				{
				if (all_lights.size() < 130)
				{
				all_lights.push_back(m_projectile_manager.m_spells[i].m_light);
				}else
				{
				break;
				}
				}*/
			}
		}
	} else 
	{
		mMutex.lock();
		// Update loading bar
		if (!mMap.generating && !mMap.loaded)
		{
			// If we don't check this there is a chance that the map generates a Cave first
			if (!mFirstLoad)
			{
				int x = math::random(0, 10);
				switch (mCurrentType)
				{
				case gen::Prison:
					if (x < 6)
						mCurrentType = gen::Cave;
					break;
				case gen::Cave:
					if (x < 6)
						mCurrentType = gen::Hell;
					break;
				case gen::Hell:
					break;
				default:
					break;
				}
				mMap.type = mCurrentType;
			}

			mFirstLoad = false;
			mLoadingThread.launch();

			// CLEAR STUFF
			// THIS IS RETARDED AND DUMB DONT JUDGE
			mobManager.mobs.clear();
			mobManager.minorMobs.clear();
			mobManager.majorMobs.clear();
			mobManager.deadMobs.clear();
			m_light_manager.m_lights.clear();
			mMap.mobSpawners.clear();
			mPlayer.m_path.clear();
			pathFinder.Clear();
			mobManager.m_update = false;
			m_projectile_manager.m_AOE_spells.clear();
			m_projectile_manager.m_arrows.clear();
			m_projectile_manager.m_spells.clear();

			std::cout << "Started map gen" << std::endl;
		}
		else if (!mMap.generating && mMap.loaded)
		{
			// Set all the other map stuff after generating it
			mLoading = false;
			std::cout << "Finished map gen" << std::endl;
			if (mMap.rooms.size() > 0)
			{
				mView.setCenter(sf::Vector2f(mMap.rooms[0].x*WIDTH, mMap.rooms[0].y*HEIGHT));
				mPlayer.setPosition((mMap.rooms[0].x + mMap.rooms[0].width/2) * WIDTH, (mMap.rooms[0].y + mMap.rooms[0].height/2) *HEIGHT);
			}else
			{
				mView.setCenter(mMap.size.x/2*WIDTH, mMap.size.y/2*WIDTH);
				mPlayer.setPosition(mMap.size.x/2*WIDTH, mMap.size.y/2*HEIGHT);
			}
			for (unsigned int i = 0; i < mMap. mobSpawners.size(); i++)
			{
				if (mMap.mobSpawners[i].amount + mMap.mobSpawners[i].s_amount > 0)
				{
					sf::Vector2f pos(mMap.mobSpawners[i].x*WIDTH, mMap.mobSpawners[i].y*HEIGHT);
					mMap.mobSpawners[i].SpawnMobs(&mobManager, mobManager.textures);
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

			mobManager.spawn_boss(mPlayer.getPosition(), getContext().textures, [this](){
				requestStackPush(States::Win);
				m_win = true;
				mPlayer.m_health = -1;
			});

			mobManager.Build_Tree();

			pathFinder.mapSize = mMap.size;
			pathFinder.GetMap(&mMap.tiles);

			Light l1(sf::Color(175, 175, 175, 255), sf::Vector3f(.5f, .5f, .075f), sf::Vector3f(0.f, 5.f, 0.f), true);
			m_light_manager.m_lights.push_back(l1);
		}
		mobManager.m_update = true;
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

		//Map
		mDiffuseRender.draw(mMap);

		mDiffuseRender.draw(m_projectile_manager);

		// Walls for the map so that the spells can be under it
		mMap.draw_walls(&mDiffuseRender, sf::RenderStates());

		// Mobs and player
		mDiffuseRender.draw(mobManager);
		mDiffuseRender.draw(mPlayer);

		// Particle
		mDiffuseRender.draw(mParticleSystem);

		mDiffuseRender.display();

		// Render normals to the normal map
		mNormalRender.clear();
		mNormalRender.setView(mView);
		mNormalRender.draw(mMap, mNormalTextures.getTexture(mCurrentMapTexture));
		mMap.draw_walls(&mNormalRender, mNormalTextures.getTexture(mCurrentMapTexture));
		//mNormalRender.display();

		// Render diffuse texture to window using shader with normal map
		// Set the all the shader properties

		passLightsToShader(&mShader, &m_all_lights, &mView);

		sf::Sprite sprite(mDiffuseRender.getTexture());
		window->setView(window->getDefaultView());
		window->draw(sprite, &mShader);
		mobManager.draw_GUI(window);
		mMap.draw_mini_map(window, mView.getCenter().x, mView.getCenter().y);
		mPlayer.drawGUI(window, getContext().fonts);
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
	mNormalTextures.loadTexture(Textures::Cave_Sheet, "resources/graphics/map/normals.png");
	mNormalTextures.loadTexture(Textures::Prison_Sheet, "resources/graphics/map/tileset.png");
	mNormalTextures.loadTexture(Textures::Hell_Sheet, "resources/graphics/map/tileset.png");
}

void GameState::SetPlayerValues(){
	m_player_values.m_position = sf::Rect<int>(mPlayer.getPosition().x - 16, mPlayer.getPosition().y - 16, 32, 32);
	m_player_values.ptr_armor = NULL;
	m_player_values.ptr_health = &mPlayer.m_health;
}