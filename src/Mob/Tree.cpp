#include "Mob\Tree.h"

using namespace mobtree;

void Tree::build(std::vector<std::vector<gen::Tile*>> tiles, std::vector<Mob*> mobs){
	for (unsigned int x = 0, y = 0; x < tiles.size(); x += SIZE)
	{
		for (y = 0; y < tiles[x].size(); y += SIZE)
		{
			Branch branch;
			branch.key = m_branches.size();
			branch.tiles.resize(SIZE, std::vector<gen::Tile*>(SIZE, NULL));

			for (unsigned int X = x, Y = y; X < x + SIZE && X < tiles.size(); X++)
			{
				for (Y = y; Y < y + SIZE && Y < tiles[X].size(); Y++)
				{
					branch.tiles[X - x][Y - y] = (tiles[X][Y]);
				}
			}
			for (unsigned int X = branch.tiles.size() - 1, Y = branch.tiles.back().size(); X > 0; X--)
			{
				for (Y = branch.tiles.back().size() - 1; Y > 0; Y--)
				{
					if (branch.tiles[X][Y] == NULL)
					{
						branch.tiles[X].erase(branch.tiles[X].begin() + Y);
					}
				}
			}

			m_branches.push_back(branch);
		}
	}
	for (int i = 0; i < mobs.size(); i++)
	{
		m_branches[search(*mobs[i])->key].AddMob(mobs[i]);
	}
}

Branch* Tree::search(Mob const& mob){
	if (mob.m_branch >= 0)
	{
		return &m_branches[mob.m_branch];
	}else
	{
		search(mob.getPosition());
	}
}

Branch* Tree::search(sf::Vector2f const& position){
	return &m_branches[(position.x/32)/SIZE + (position.y/32)/SIZE];
}

void Tree::move(Mob* p_mob, Branch* p_from, Branch* p_to){
	if (p_from != NULL)
	{
		p_from->RemoveMob(p_mob);
	}
	if (p_to != NULL)
	{
		p_to->AddMob(p_mob);
	}
}

void Branch::AddMob(Mob* mob){
	if (mob->m_branch != key)
	{
		mob->m_branch = key;
		mobs.push_back(mob);
	}
}

void Branch::RemoveMob(Mob* mob){
	if (mob->m_branch == key)
	{
		for (int i = 0; i < mobs.size(); i++)
		{
			if (mobs[i]->ID == mob->ID)
			{
				mob->m_branch = -1;
				mobs.erase(mobs.begin() + i);
				break;
			}
		}
	}
}

Mob* Branch::GetMobWithTarget(const sf::Vector2i & position){
	for (unsigned int i = 0; i < mobs.size(); i++)
	{
		if (mobs[i]->aggro && !mobs[i]->path.empty())
		{
			if (sf::Vector2i(mobs[i]->path.back().x/32, mobs[i]->path.back().y/32) == sf::Vector2i(position.x/32, position.y/32))
			{
				return mobs[i];
			}
		}
	}
	return NULL;
}