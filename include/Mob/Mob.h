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
#include "Item\ItemManager.h"
#include "Mob\MobTypeManager.h"
#include "Mob\DeadMob.h"
#include "Spells and Projectiles\Spell.h"
#include "Spells and Projectiles\Arrow.h"
#include "Spells and Projectiles\SpellTypeManager.h"
#include "Animation\Animation.h"

class Mob : public sf::Transformable, public sf::Drawable{

public:
	virtual void update(std::vector<std::vector<gen::Tile>>* map, sf::Time& deltaTime, sf::Vector2f & playerPosition, int* p_health, Mob* ptr_mob, std::vector<projectile::Spell>* ptr_spells, std::vector<projectile::Arrow>* ptr_arrows) = 0;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const = 0;
	virtual void takeDamage(int damage) = 0;

	bool hasBuff(const Items & p_item);
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
	std::vector<_BUFF::Buff> m_buffs;

	std::vector<sf::Vector2i> path;
	int ID, width, height, health;//, damage, aggroDist, armor, speed, attackSpeed;
	int m_branch;
	float timeSincePath;

	///<summary>Get a new path to the player when this is <= 0.</summary>
	float updatePath;
	///<summary>Attack the target when this is <= 0.</summary>
	float m_attack_timer;
	void checkCollision(std::vector<std::vector<gen::Tile>>* map, sf::Vector2f & playerPosition);
	virtual void die() = 0;
	void StickArrow(const projectile::Arrow & arrow);

	void followPath(sf::Time& dt);

	virtual void dealDamage(int* health) = 0;

protected:
	Animation m_animation;
	std::vector<DeadMob>* p_deadMobs;

	///<summary>Updates the animation and the direction of the mob.</summary>
	void UpdateAnimation(const sf::Vector2<float> & playerPosition);

	enum Direction{
		Walking,
		Up,
		Down,
		Left,
		Right,
	};

	///<summary>To know which animation to loop if the mob is standing still.</summary>
	Direction m_direction;
};