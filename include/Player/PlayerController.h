#pragma once

#include "Player.h"
#include "PathFinder\PathFinder.h"
#include "Map\Map.h"
#include "Mob\MobManager.h"

#include <SFML\Window\Keyboard.hpp>
#include <SFML\Window\Mouse.hpp>
#include <SFML\Window\Event.hpp>

#include <SFML\System\NonCopyable.hpp>

class PlayerController
	:
	private sf::NonCopyable
{
public:
	PlayerController(Player* player, PathFinder* pathfinder, gen::Map* map, MobManager* mobManager);
	virtual			~PlayerController();

	void			update(sf::Time dt, sf::RenderWindow const& window, sf::View const& view, LightManager* ptr_light_manager);
	void			handleEvents(sf::Event const& event);

private:
	PathFinder*		pathFinderPtr;
	Player*			playerPtr;
	gen::Map*		mapPtr;
	MobManager*		mobManagerPtr;

	void			getPath(float x, float y);

	void move_up(), move_down(), move_left(), move_right(), check_collision();

	sf::Vector2f	mPathStart;
	sf::Vector2f    mPathEnd;

	Mob*			targetPtr;

	bool leftMouseClicked, rightMouseClicked;
};