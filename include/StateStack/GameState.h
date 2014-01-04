#pragma once

#include "State.h"
#include "Map\Map.h"
#include "Effects\Light.h"
#include "PathFinder\PathFinder.h"
#include "Player\Player.h"
#include "Player\PlayerController.h"
#include "Mob\MobManager.h"
#include "PathFinder\PathFinder.h"

#include <SFML\Graphics\RenderTexture.hpp>
#include <SFML\Graphics\Shader.hpp>

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
	gen::Map				mMap;

	sf::View				mView;

	sf::RenderTexture		mNormalRender;
	sf::RenderTexture		mDiffuseRender;
	sf::Shader				mShader;

	TextureHolder			mNormalTextures;	

	std::vector<Light>		mLights;

	PlayerController		mPlayerController;
	Player					mPlayer;
	
	float					mSpawnRadius;
};