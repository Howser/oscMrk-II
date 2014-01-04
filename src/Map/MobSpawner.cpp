#include "Map\MobSpawner.h"

gen::MobSpawner::MobSpawner(int x, int y, int amount, TYPE type, int s_amount, TYPE s_type)
	: x(x), y(y), amount(amount), type(type), s_amount(s_amount), s_type(s_type){}

void gen::MobSpawner::SpawnMobs(MobManager* mobManagerPtr, TextureHolder* textureHolderPtr){
	MinorMob mob = MinorMob(type, textureHolderPtr, &mobManagerPtr->deadMobs);
	sf::Vector2i pos(x*WIDTH, y*HEIGHT);
	mob.setPosition((sf::Vector2f)pos);
	mob.startPos = mob.getPosition();
	for (unsigned int i = 0; i < amount; i++)
	{
		mobManagerPtr->Add(mob);
	}
	MajorMob s_mob = MajorMob(s_type, textureHolderPtr, &mobManagerPtr->deadMobs);
	s_mob.setPosition((sf::Vector2f)pos);
	s_mob.startPos = s_mob.getPosition();
	for (unsigned int i = 0; i < s_amount; i++)
	{
		mobManagerPtr->Add(s_mob);
	}
}