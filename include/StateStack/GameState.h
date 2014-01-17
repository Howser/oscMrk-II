#pragma once

#include "Effects\Lights\LightManager.h"
#include "Item\ProjectileManager.h"
#include "State.h"
#include "Map\Map.h"
#include "PathFinder\PathFinder.h"
#include "Player\Player.h"
#include "Player\PlayerController.h"
#include "Mob\MobManager.h"
#include "PathFinder\PathFinder.h"
#include "Effects\Particles\ParticleSystem.h"

#include <SFML\Graphics\RenderTexture.hpp>
#include <SFML\Graphics\Shader.hpp>
#include <SFML\System\Thread.hpp>

class GameState : public State
{
public:	
	GameState(StateStack& stateStack, Context context, States::ID id);
	virtual					~GameState();

	bool					update(sf::Time dt);
	bool					handleEvents(const sf::Event& event);
	
	void					draw();
	
	MobManager				mobManager;
	sf::Vector2i mapSize;
	PathFinder pathFinder;
private:

	void					loadNormals();

private:
	LightManager m_light_manager;
	ProjectileManager m_projectile_manager;

	gen::Map				mMap;

	sf::View				mView;

	ParticleSystem			mParticleSystem;

	sf::RenderTexture		mNormalRender;
	sf::RenderTexture		mDiffuseRender;
	sf::Shader				mShader;

	TextureHolder			mNormalTextures;	

	std::vector<Light>		mLights;

	PlayerController		mPlayerController;
	Player					mPlayer;
	
	float					mSpawnRadius;
	bool					mLoading;
	sf::Sprite				mLoadingSprite;
	sf::Thread				mLoadingThread;
	gen::Type				mCurrentType;
	sf::Mutex				mMutex;
};