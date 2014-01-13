#pragma once
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\System\Time.hpp>

#include "Math\GeneralMath.h"
#include "Math\VectorMath.h"
#include "ResourceHolders\TextureHolder.h"
#include "Map\Tile.h"
#include "Inventory\Inventory.h"
#include "Inventory\LootTable.h"
#include "Item\BaseItem.h"
#include "Item\GearItem.h"
#include "Item\MiscItem.h"
#include "Mob\MobTypeManager.h"
#include "Mob\DeadMob.h"
#include "Item\Arrow.h"

#define TERMINAL_VELOCITY 250

class Mob : public sf::Transformable, public sf::Drawable{
public:
	virtual void update(std::vector<std::vector<gen::Tile>>* map, sf::Time& deltaTime, sf::Vector2f & playerPosition) = 0;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const = 0;
	virtual void takeDamage(int damage) = 0;
	bool IntersectsWall(sf::Rect<int> const& position, std::vector<std::vector<gen::Tile>> const& map);
	void stop();
	bool worldCollision, playerCollision, dead, aggro, t_update;

	TYPE type;
	Inventory inventory;
	LootTable lootTable;

	sf::Sprite sprite;
	TextureHolder* textureHolder;
	
	sf::Vector2f velocity;
	sf::Vector2i startPos;

	///<summary>Used to relocate the mob in the tree.</summary>
	sf::Vector2f prevPos;
	
	std::vector<sf::Sprite> m_arrows;

	std::vector<sf::Vector2i> path;
	int ID, width, height, health;//, damage, aggroDist, armor, speed, attackSpeed;
	int m_branch;
	float timeSincePath;

	///<summary>Get a new path to the player when this is <= 0.</summary>
	float updatePath;
	void checkCollision(std::vector<std::vector<gen::Tile>>* map, sf::Vector2f & playerPosition);
	virtual void die() = 0;
	void StickArrow(const projectile::Arrow & arrow);
protected:
	void followPath(sf::Time& dt);

	virtual void dealDamage(int* health) = 0;
	std::vector<DeadMob>* p_deadMobs;
};