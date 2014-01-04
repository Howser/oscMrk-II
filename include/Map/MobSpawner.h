#pragma once
#include "Mob\MobManager.h"
#include "Map\MapManager.h"
#include "Mob\MinorMob.h"

namespace gen{
	struct MobSpawner{
		MobSpawner(int x, int y, int amount, TYPE type, int s_amount, TYPE s_type);

		int x, y, amount, s_amount;
		TYPE type;
		TYPE s_type;

		void SpawnMobs(MobManager* mobManagerPtr, TextureHolder* textureHolderPtr);
	};
}