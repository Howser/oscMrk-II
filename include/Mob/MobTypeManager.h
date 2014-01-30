#pragma once
#include <iostream>
#include <vector>

#define MOBS_per_FRAME 50
#define MAX_MOBS_PATHING 1

enum TYPE{
	test,
	special,

	//used for targeting
	PLAYER,
};

static const char* MobNames[2] = {"test.txt", "special.txt"};

static int GetHp(TYPE const& type){
	switch (type)
	{
	case test:
		return 15;
		break;
	case special:
		return 35;
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
	case test:
		return 5.f;
		break;
	case special:
		return 12;
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
	case test:
		return 0.5f;
		break;
	case special:
		return 2.f;
		break;
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
	case test:
		return 64.f;
		break;
	case special:
		return 64.f;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

static float GetSpeed(TYPE const& type){
	switch (type)
	{
	case test:
		return 175;
		break;
	case special:
		return 100;
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
	case test:
		return 32;
		break;
	case special:
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
	case test:
		return 32;
		break;
	case special:
		return 32;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}


///<summary>Seconds.</summary>
static int GetTimeBetweenPathing(TYPE const& type){
	switch (type)
	{
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
	case test:
		return false;
		break;
	case special:
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
		case test:
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