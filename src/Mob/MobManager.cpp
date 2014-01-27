#include "Mob\MobManager.h"

MobManager::MobManager(TextureHolder & textureHolder, std::vector<std::vector<gen::Tile>>* tiles, PathFinder* pathFinder) : textures(&textureHolder), pathFinder(pathFinder), tiles(tiles), playerPosition(-100, -100){
	for (unsigned int i = 0; i <= TYPE::special; i++)
	{
		LootTables.push_back(LootTable((TYPE)i));
	}
}

MobManager::MobManager(){

}

MobManager::~MobManager(){

}

void MobManager::Add(Mob & mob){
	if (!IsSpecial(mob.type))
	{
		MinorMob minor_mob = MinorMob(mob, &deadMobs);
		minor_mob.lootTable = LootTables[minor_mob.type];
		minorMobs.push_back(minor_mob);
	}else
	{
		MajorMob major_mob = MajorMob(mob, &deadMobs);
		major_mob.lootTable = LootTables[major_mob.type];
		majorMobs.push_back(major_mob);
	}
	std::cout << "x: " << mob.startPos.x << "     y: " << mob.startPos.y << "\n";
}

void MobManager::Build_Tree(){
	m_tree = mobtree::Tree();
	std::vector<std::vector<gen::Tile*>> p_tiles;
	p_tiles.resize(tiles->size(), std::vector<gen::Tile*>(tiles->back().size(), NULL));

	for (int x = 0, y = 0; x < tiles->size(); x++)
	{
		for (y = 0; y < tiles->back().size(); y++)
		{
			p_tiles[x][y] = (&(*tiles)[x][y]);
		}
	}
	m_tree.build(p_tiles, mobs);
}

void MobManager::Update(sf::Time & deltaTime, sf::Vector2f const& playerPosition){
	for (Mobs::const_iterator i = mobs.begin(), j = m_tree.search(*mobs[(*i)->ID])->mobs.begin(); i != mobs.end(); ++i)
	{
		if (!(*i)->dead)
		{
			(*i)->update(tiles, deltaTime, (sf::Vector2f)playerPosition);
			if (math::distance((*i)->getPosition(), playerPosition) <= 735 || (*i)->aggro)//if it's on the screen, give it a path/check other mobs/do other shit. This isn't noticable to the player, but it gives us pretty drastic performance boosts
			{
				if (math::distance((*i)->prevPos, (*i)->getPosition()) >= 32)
				{
					m_tree.move((*i), m_tree.search(*mobs[(*i)->ID]), m_tree.search((*i)->getPosition()));
					(*i)->prevPos = (*i)->getPosition();
				}
#pragma region Mob-Mob Collision
				if (math::distance((sf::Vector2i)(*i)->getPosition(), (sf::Vector2i)playerPosition) > 48) //(!(*i)->playerCollision)
				{
					for (j = m_tree.search(*mobs[(*i)->ID])->mobs.begin(); j != m_tree.search(*mobs[(*i)->ID])->mobs.end(); ++j)
					{
						if ((*i)->ID != (*j)->ID)
						{
							if (!(*j)->dead && !(*i)->worldCollision && !(*j)->worldCollision && math::distance((sf::Vector2i)(*i)->getPosition(), (sf::Vector2i)playerPosition) > 48)//!(*j)->playerCollision)
							{
								if (math::distance((*i)->getPosition(), (*j)->getPosition()) <= 32)
								{
									if (math::distance(sf::Vector2i((*i)->getPosition().x, 0), sf::Vector2i((*j)->getPosition().x, 0)) >= math::distance(sf::Vector2i(0, (*i)->getPosition().y), sf::Vector2i(0, (*j)->getPosition().y)))
									{
										//x
										if (!(*i)->worldCollision && !(*j)->worldCollision)
										{
											if ((*i)->getPosition().x < (*j)->getPosition().x)
											{
												if (std::abs((*i)->velocity.x) < std::abs((*j)->velocity.x))
												{
													//(*i)->stop();
													if (!(*i)->IntersectsWall(sf::Rect<int>((*i)->getPosition().x - ((*i)->getPosition().x + (*i)->width - (*j)->getPosition().x)/2 - (*i)->width/2, (*i)->getPosition().y - (*i)->height/2, (*i)->width, (*i)->height), *tiles))
													{
														(*i)->setPosition((*i)->getPosition().x - ((*i)->getPosition().x + (*i)->width - (*j)->getPosition().x)/2 + math::random(-1, 1, 0), (*i)->getPosition().y);
													}
												}else
												{
													//(*j)->stop();
													if (!(*j)->IntersectsWall(sf::Rect<int>((*j)->getPosition().x + ((*i)->getPosition().x + (*i)->width - (*j)->getPosition().x)/2 - (*j)->width/2, (*j)->getPosition().y - (*j)->height/2, (*j)->width, (*j)->height), *tiles))
													{
														(*j)->setPosition((*j)->getPosition().x + ((*i)->getPosition().x + (*i)->width - (*j)->getPosition().x)/2 + math::random(-1, 1, 0), (*j)->getPosition().y);
													}
												}
											}else {
												if (std::abs((*i)->velocity.x) < std::abs((*j)->velocity.x))
												{
													//(*i)->stop();
													if (!(*i)->IntersectsWall(sf::Rect<int>((*i)->getPosition().x + ((*j)->getPosition().x + (*j)->width - (*i)->getPosition().x)/2 - (*i)->width/2, (*i)->getPosition().y - (*i)->height/2, (*i)->width, (*i)->height), *tiles))
													{
														(*i)->setPosition((*i)->getPosition().x + ((*j)->getPosition().x + (*j)->width - (*i)->getPosition().x)/2 + math::random(-1, 1, 0), (*i)->getPosition().y);
													}
												}else
												{
													//(*j)->stop();
													if (!(*j)->IntersectsWall(sf::Rect<int>((*j)->getPosition().x - ((*j)->getPosition().x + (*j)->width - (*i)->getPosition().x)/2 - (*j)->width/2, (*j)->getPosition().y - (*j)->height/2, (*j)->width, (*j)->height), *tiles))
													{
														(*j)->setPosition((*j)->getPosition().x - ((*j)->getPosition().x + (*j)->width - (*i)->getPosition().x)/2 + math::random(-1, 1, 0), (*j)->getPosition().y);
													}
												}
											}
										}else
										{
											break;
										}
									}else
									{
										//y
										if (!(*i)->worldCollision && !(*j)->worldCollision)
										{
											if ((*i)->getPosition().y < (*j)->getPosition().y)
											{
												if (std::abs((*i)->velocity.y < std::abs((*j)->velocity.y)))
												{
													//(*i)->stop();
													if (!(*i)->IntersectsWall(sf::Rect<int>((*i)->getPosition().x - (*i)->width/2, (*i)->getPosition().y - ((*i)->getPosition().y + (*i)->height - (*j)->getPosition().y)/2 - (*i)->height/2, (*i)->width, (*i)->height), *tiles))
													{
														(*i)->setPosition((*i)->getPosition().x, (*i)->getPosition().y - ((*i)->getPosition().y + (*i)->height - (*j)->getPosition().y)/2 + math::random(-1, 1, 0));
													}
												}else
												{
													//(*j)->stop();
													if (!(*j)->IntersectsWall(sf::Rect<int>((*j)->getPosition().x - (*i)->width/2, (*j)->getPosition().y + ((*i)->getPosition().y + (*i)->height - (*j)->getPosition().y)/2 - (*j)->height/2, (*j)->width, (*j)->height), *tiles))
													{
														(*j)->setPosition((*j)->getPosition().x, (*j)->getPosition().y + ((*i)->getPosition().y + (*i)->height - (*j)->getPosition().y)/2 + math::random(-1, 1, 0));
													}
												}
											}else
											{
												if (std::abs((*i)->velocity.y < std::abs((*j)->velocity.y)))
												{
													//(*i)->stop();
													if (!(*i)->IntersectsWall(sf::Rect<int>((*i)->getPosition().x - (*i)->width/2, (*i)->getPosition().y + ((*j)->getPosition().y + (*j)->height - (*i)->getPosition().y)/2 - (*i)->height/2, (*i)->width, (*i)->height), *tiles))
													{
														(*i)->setPosition((*i)->getPosition().x, (*i)->getPosition().y + ((*j)->getPosition().y + (*j)->height - (*i)->getPosition().y)/2 + math::random(-1, 1, 0));
													}
												}else
												{
													//(*j)->stop();
													if (!(*j)->IntersectsWall(sf::Rect<int>((*j)->getPosition().x - (*j)->width/2, (*j)->getPosition().y - ((*j)->getPosition().y + (*i)->height - (*j)->getPosition().y)/2 - (*j)->height/2, (*j)->width, (*j)->height), *tiles))
													{
														(*j)->setPosition((*j)->getPosition().x, (*j)->getPosition().y - ((*j)->getPosition().y + (*i)->height - (*j)->getPosition().y)/2 + math::random(-1, 1, 0));
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
#pragma endregion
				if (math::distance((*i)->getPosition(), playerPosition) <= GetAggroDist((*i)->type))
				{
					if (!(*i)->aggro)
					{
						(*i)->stop();
					}
					(*i)->aggro = true;
				}else
				{
					if ((*i)->aggro)
					{
						(*i)->path = pathFinder->GetPath(sf::Vector2<int>((*i)->getPosition().x/WIDTH, (*i)->getPosition().y/HEIGHT), sf::Vector2<int>((*i)->startPos.x/WIDTH, (*i)->startPos.y/HEIGHT), true);
						for (int j = 0; j < (*i)->path.size(); j++)
						{
							(*i)->path[j].x*=WIDTH;
							(*i)->path[j].y*=HEIGHT;
						}
						(*i)->aggro = false;
					}
				}
				if (!(*i)->aggro)
				{
#pragma region Random Movement
					if ((*i)->path.empty())
					{
						if ((*i)->timeSincePath >= GetTimeBetweenPathing((*i)->type))
						{
							if (math::random(1, 100) <= 100)
							{
								std::vector<sf::Vector2<int>> availableTiles;
								for (int x = (*i)->startPos.x/WIDTH - GetPathingDistance((*i)->type), y = (*i)->startPos.y - GetPathingDistance((*i)->type); x < (*i)->startPos.x/WIDTH + GetPathingDistance((*i)->type); x++)
								{
									for (y = (*i)->startPos.y/HEIGHT - GetPathingDistance((*i)->type); y < (*i)->startPos.y/HEIGHT + GetPathingDistance((*i)->type); y++)
									{
										if (x > 0 && x < tiles->size())
										{
											if (y > 0 && y < (*tiles)[x].size())
											{
												if ((*tiles)[x][y].type == 1)
												{
													availableTiles.push_back(sf::Vector2<int>(x, y));
												}
											}
										}
									}
								}
								if (!availableTiles.empty())
								{
									int index = math::random(0, availableTiles.size() - 1);
									(*i)->path = pathFinder->GetPath(sf::Vector2i((*i)->getPosition().x/WIDTH, (*i)->getPosition().y/HEIGHT), sf::Vector2i(availableTiles[index].x, availableTiles[index].y), true);
									for (int j = 0; j < (*i)->path.size(); j++)
									{
										(*i)->path[j].x*=WIDTH;
										(*i)->path[j].y*=HEIGHT;
									}
									availableTiles.clear();
								}
							}else
							{
								(*i)->timeSincePath = 0;
							}
						}
					}
#pragma endregion
				}else
				{
#pragma region Move to the Player
					if (!(*i)->playerCollision)
					{
						float dist = math::distance((*i)->getPosition(), playerPosition);
						if (dist > WIDTH/2 && dist <= 735 && ((*i)->path.size() <= 1 || (*i)->updatePath <= 0))
						{
							std::auto_ptr<Mob> p_mob = (std::auto_ptr<Mob>)m_tree.m_branches[(*i)->m_branch].GetMobWithTarget((sf::Vector2i)playerPosition, *(*i));
							if (p_mob.get() && p_mob->aggro && math::distance((*i)->getPosition(), playerPosition) > math::distance((*i)->getPosition(), p_mob->getPosition()))
							{
								(*i)->path = pathFinder->GetPath(sf::Vector2i((*i)->getPosition().x/WIDTH, (*i)->getPosition().y/HEIGHT), sf::Vector2i((p_mob->getPosition().x)/WIDTH, (p_mob->getPosition().y)/HEIGHT), false);
							}else
							{
								(*i)->path = pathFinder->GetPath(sf::Vector2i((*i)->getPosition().x/WIDTH, (*i)->getPosition().y/HEIGHT), sf::Vector2i((playerPosition.x)/WIDTH, (playerPosition.y + 10)/HEIGHT), false);
							}
							(*i)->updatePath = 1.f;
							p_mob.release();
						}
					}
#pragma endregion
				}
			}
		}
	}
	this->playerPosition = playerPosition;
}

bool MobManager::update(int const& id){
	/*for (int i = 0; i < update_IDs.size(); i++)
	{
	if (update_IDs[i] == id)
	{
	return true;
	}
	}*/
	return true;
}

void MobManager::SetView(sf::View const& view){
	this->view = view;
}

void MobManager::draw(sf::RenderTarget & target, sf::RenderStates states) const{
	sf::Rect<int>bounds((int)(view.getCenter().x - view.getSize().x / 2), (int)(view.getCenter().y - view.getSize().y / 2), (int)(view.getCenter().x + view.getSize().x / 2) - (int)(view.getCenter().x - view.getSize().x / 2), (int)(view.getCenter().y + view.getSize().y / 2) - (int)(view.getCenter().y - view.getSize().y / 2));
	for (unsigned int i = 0; i < deadMobs.size(); ++i)
	{
		if (bounds.intersects(sf::Rect<int>(deadMobs[i].getPosition().x, deadMobs[i].getPosition().y, GetWidth(*deadMobs[i].type), GetHeight(*deadMobs[i].type))))
		{
			target.draw(deadMobs[i], states);
		}
	}
	for (Mobs::const_iterator i = mobs.begin(); i != mobs.end(); ++i)
	{
		if (!(*i)->dead)
		{
			if (bounds.intersects(sf::Rect<int>((*i)->getPosition().x, (*i)->getPosition().y, (*i)->width, (*i)->height)))
			{
				(*i)->draw(target, states);
			}
		}
	}
}

LootTable* MobManager::GetLootTable(TYPE const& type){
	return &LootTables[type];
}

Items StringToItemEnum(std::string const& itemString)
{
	for (int t = Arrow; t != Items::end; t++)
	{
		std::string name = ItemNames[t];
		if (name == itemString)
		{
			return (Items)t;
		}
	}
	return Items::end;
}

LootTable::LootTable(TYPE type){
	std::string s = MobNames[type];
	std::string path = "resources/loot tables/" + s;
	std::ifstream file;
	file.open(path);
	if (file.is_open())
	{
		std::cout << "loading loot table @ " << path << "\n";
		while (!file.eof())
		{
			std::string line;
			LootItem item;
			std::string itemString;
			std::string minQ;
			std::string maxQ;
			std::string c;
			std::getline(file, line);
			int space = 0;
			for (unsigned int i = 0; i < line.length(); i++)
			{
				if (line[i] == ' ')
				{
					space++;
				}else
				{
					switch (space)
					{
					case 0:
						for (unsigned int j = i; j < line.length(); j++){
							if (line[j] == ' ')
							{
								item.item = StringToItemEnum(itemString);
								space++;
								i += j-i;
								break;
							}else
							{
								itemString += line[j];
							}
						}
						break;
					case 1:
						for (unsigned int j = i; j < line.length(); j++){
							if (line[j] == ' ')
							{
								item.minQ = atoi(minQ.c_str());
								i += j-i;
								space++;
								break;
							}else
							{
								minQ += line[j];
							}
						}
						break;
					case 2:
						for (unsigned int j = i; j < line.length(); j++){
							if (line[j] == ' ')
							{
								item.maxQ = atoi(maxQ.c_str());
								i += j-i;
								space++;
								break;
							}else
							{
								maxQ += line[j];
							}
						}
						break;
					case 3:
						for (unsigned int j = i; j < line.length(); j++){
							if (j == line.length() - 1)
							{
								c += line[j];
								item.c = atoi(c.c_str());
								i += j-i;
								break;
							}else
							{
								c += line[j];
							}
						}
						break;
					}
				}
			}
			lootItems.push_back(item);
		}
		file.close();
	}
	for (int i = 0; i < lootItems.size(); i++)
	{
		std::cout << "Item: " << ItemNames[lootItems[i].item] << " Chance: " << lootItems[i].c << " Quantity range: " << lootItems[i].minQ << " - " << lootItems[i].maxQ << "\n";
	}
};

LootTable::LootTable(){
	/*std::string path = "resources/loot tables/test.txt";
	std::ifstream file;
	file.open(path);
	if (file.is_open())
	{
	std::cout << "opened file for empty \n";
	}*/
};

int MobManager::AliveMobs(){
	int m = 0;
	for (int i = 0; i < mobs.size(); i++)
	{
		if (!mobs[i]->dead)
		{
			m += 1;
		}
	}
	return m;
}

int MobManager::GetIndexOf(Mob const& mob){
	for (int i = 0; i < mobs.size(); i++)
	{
		if (!mobs[i]->dead && mobs[i]->getPosition().x == mob.getPosition().x && mobs[i]->getPosition().y == mob.getPosition().y)
		{
			return i;
		}
	}
	return 0;
}

Mob* MobManager::getAtPosition(float x, float y)
{
	Mob* p_mob = nullptr;
	for (int i = 0; i != mobs.size(); ++i)
	{
		if (sf::Rect<float>(mobs[i]->getPosition().x - mobs[i]->width/2, mobs[i]->getPosition().y - mobs[i]->height/2, mobs[i]->width, mobs[i]->height).intersects(sf::Rect<float>(x, y, 1, 1)))
		{
			p_mob = mobs[i];
			if (!mobs[i]->dead)
			{
				return p_mob;
			}
		}
	}
	return p_mob;
}

Mob* MobManager::getAtPosition(sf::Vector2f position)
{
	for (int i = 0; i < m_tree.search(position)->mobs.size(); i++)
	{
		if (sf::Rect<float>(m_tree.search(position)->mobs[i]->getPosition().x - m_tree.search(position)->mobs[i]->width/2, m_tree.search(position)->mobs[i]->getPosition().y - m_tree.search(position)->mobs[i]->height/2, m_tree.search(position)->mobs[i]->width, m_tree.search(position)->mobs[i]->height).intersects(sf::Rect<float>(position.x, position.y, 1, 1)))
		{
			if (!mobs[i]->dead)
			{
				return m_tree.search(position)->mobs[i];
			}
		}
	}
	return getAtPosition(position.x, position.y);
}