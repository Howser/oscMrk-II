#pragma once
#include <iostream>
#include <vector>
#include "Map\Tile.h"
#include "Math\GeneralMath.h"

#define MOBS_per_FRAME 50
#define MAX_MOBS_PATHING 1

enum TYPE{
	skeleton,
	special,
	boss,

	//used for targeting
	PLAYER,
};

static const char* MobNames[3] = {"skeleton.txt", "special.txt", "boss.txt"};

static int GetHp(TYPE const& type){
	switch (type)
	{
	case skeleton:
		return 15;
		break;
	case special:
		return 35;
		break;
	case boss:
		return 100;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

static float GetDamage(TYPE const& type){
	switch (type)
	{
	case skeleton:
		return 5.f;
		break;
	case special:
		return 12;
		break;
	case boss:
		return 20;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

///<summary>Seconds.</summary>
static float GetAttackSpeed(TYPE const& type){
	switch (type)
	{
	case skeleton:
		return 2.f;
		break;
	case special:
		return 2.f;
		break;
	case boss:
		return 1.f;
	default:
		return 0;
		break;
	}
	return 0;
}

///<summary>Pixels.</summary>
static float GetAtackDistance(const TYPE & p_type){
	switch (p_type)
	{
	case skeleton:
		return 256.f;
		break;
	case special:
		return 64.f;
		break;
	case boss:
		return 80.f;
	default:
		return 0;
		break;
	}
	return 0;
}

///<summary>Pixels per second.</summary>
static float GetSpeed(TYPE const& type){
	switch (type)
	{
	case skeleton:
		return 175;
		break;
	case special:
		return 100;
		break;
	case boss:
		return 125;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

static int GetAggroDist(TYPE const& type){
	switch (type)
	{
	case boss:
		return 400;
		break;
	default:
		return 250;
		break;
	}
	return 0;
}

static int GetUnAggroDist(const TYPE & type){
	switch (type)
	{
	default:
		return GetAggroDist(type) + 64;
		break;
	}
}

static int GetWidth(TYPE const& type){
	switch (type)
	{
	case skeleton:
		return 32;
		break;
	case special:
		return 32;
		break;
	case boss:
		return 32;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

static int GetHeight(TYPE const& type){
	switch (type)
	{
	case skeleton:
		return 32;
		break;
	case special:
		return 32;
		break;
	case boss:
		return 32;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

///<summary>Seconds.</summary>
static float GetTimeBetweenPathing(TYPE const& type){
	switch (type)
	{
	case boss:
		return 0.4;
	default:
		return 1;
		break;
	}
}

static int GetPathingDistance(TYPE const& type){
	switch (type)
	{
	case TYPE::special:
		return 2;
		break;
	case boss:
		return 2;
		break;
	default:
		return 10;
		break;
	}
	return 0;
}

///<summary>Seconds.</summary>
static int GetSpecialTimer(TYPE const& type){
	switch (type)
	{
	case special:
		return 5;
		break;
	case boss:
		return 10; 
	default:
		return 0;
		break;
	}
	return 0;
}



static std::vector<TYPE> GetTargets(TYPE const& type){
	std::vector<TYPE> targets;
	switch (type)
	{
	case special:
		TYPE TARGETS[1] = {TYPE::PLAYER};
		targets = std::vector<TYPE>(TARGETS, TARGETS + sizeof TARGETS / sizeof TARGETS[0]);
		return targets;
		break;
	}
	return targets;
}

static bool IsSpecial(TYPE const& type){
	switch (type)
	{
	case skeleton:
		return false;
		break;
	case special:
	case boss:
		return true;
		break;
	case PLAYER:
		return false;
		break;
	default:
		return false;
		break;
	}
}

namespace spcl{
	static void Special(TYPE const& type, float* specialTimer){
		std::vector<TYPE> targets = GetTargets(type);
		switch (type)
		{
		case skeleton:
			break;
		case special:
			*specialTimer = 0;
			break;
		case PLAYER:
			break;
		default:
			break;
		}
	}
}


template <typename T>
static bool LineOfSight(const sf::Vector2<T> & p_A, const sf::Vector2<T> & p_B, std::vector<std::vector<gen::Tile>>* ptr_map){
	if (p_A == p_B)
	{
		return true;
	}

	/*float k = (p_A.y - p_B.y)/((p_A.x != p_B.x) ? p_A.x - p_B.x:1);
	int m = ((p_A.x < p_B.x) ? p_A.y:p_B.y)/32;
	std::vector<sf::Rect<int>> tiles;
	for (int x = (p_A.x < p_B.x) ? p_A.x/32:p_B.x/32; x < ((p_A.x > p_B.x) ? p_A.x/32:p_B.x/32); x++)
	{
	tiles.push_back(sf::Rect<int>(x*32, k*x*32+m, 32, 32));
	}*/

	for (int x = (p_A.x < p_B.x) ? p_A.x/32:p_B.x/32; x < ((p_A.x > p_B.x) ? p_A.x/32:p_B.x/32); x++)
	{
		for (int y = (p_A.y < p_B.y) ? p_A.y/32:p_B.y/32; y < ((p_A.y > p_B.y) ? p_A.y/32:p_B.y/32); y++)
		{
			if ((*ptr_map)[x][y].type != 1)
			{
				if (math::LineIntersectsRect(p_A, p_B, sf::Rect<T>(x*32, y*32, 32, 32)))
				{
					return false;
				}
			}
		}
	}
	return true;
}