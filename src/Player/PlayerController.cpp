#include "Player\PlayerController.h"

PlayerController::PlayerController(Player* player, PathFinder* pathfinder, gen::Map* map, MobManager* mobManager)
	:
	playerPtr(player),
	pathFinderPtr(pathfinder),
	mapPtr(map),
	mPathStart(),
	mobManagerPtr(mobManager),
	targetPtr(NULL)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::update(sf::Time dt, sf::RenderWindow const& window, sf::View const& view)
{
	if (targetPtr != NULL)
	{
		if (vec::length(mPathEnd - targetPtr->getPosition()) < (WIDTH+HEIGHT)/2)
		{
			getPath(targetPtr->getPosition().x/WIDTH, targetPtr->getPosition().y/HEIGHT);
			mPathEnd = targetPtr->getPosition();
		}
		if (!targetPtr->dead && playerPtr->isInRange(targetPtr))
		{
			playerPtr->attack(window, targetPtr);
			targetPtr->stop();
			targetPtr = nullptr;
		}
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i mpos = sf::Mouse::getPosition(window);
		mpos.x += playerPtr->getPosition().x - view.getSize().x/2;
		mpos.y += playerPtr->getPosition().y - view.getSize().y/2;

		targetPtr = mobManagerPtr->getAtPosition(mpos.x, mpos.y);
		mPathEnd = sf::Vector2f(mpos.x, mpos.y);

		mpos.x /= WIDTH;
		mpos.y /= HEIGHT;

		sf::Vector2f diff = mPathStart - playerPtr->getPosition();
		diff.x = std::abs(diff.x);
		diff.y = std::abs(diff.y);
		if (((diff.x > WIDTH || diff.y > HEIGHT) || !playerPtr->isPathing()) && targetPtr == NULL)
		{
			getPath(mpos.x, mpos.y);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
	{
		if (leftMouseClicked)
		{
			playerPtr->attack(window, nullptr);
		}
		playerPtr->stop();
		targetPtr = nullptr;
	}
	if (rightMouseClicked && sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		playerPtr->attack(window, nullptr);
	}
	if (leftMouseClicked)
	{
		if (targetPtr != NULL)
		{
			if (targetPtr->dead && math::distance(targetPtr->getPosition(), playerPtr->getPosition()) < WIDTH)
			{
				playerPtr->m_lootInventory.clear(false);
				for (int i = 0; i < mobManagerPtr->mobs.size(); i++)
				{
					if (math::distance(mobManagerPtr->mobs[i]->getPosition(), playerPtr->getPosition()) <= LOOT_DISTANCE)
					{
						if (!mobManagerPtr->mobs[i]->inventory.empty())
						{
							bool broken = false;
							playerPtr->m_lootMobs.push_back(mobManagerPtr->mobs[i]->ID);
							for (int x = 0, y = 0; x < mobManagerPtr->mobs[i]->inventory.width; x++)
							{
								if (broken)
								{
									break;
								}
								for (y = 0; y < mobManagerPtr->mobs[i]->inventory.height; y++)
								{
									if (broken)
									{
										break;
									}
									for (int it = 0; it < mobManagerPtr->mobs[i]->inventory.slots[x][y].Items.size(); it++)
									{
										int X = playerPtr->m_lootInventory.GetFirstAvailableSlot(mobManagerPtr->mobs[i]->inventory.slots[x][y].Items[0].item).x;
										int Y = playerPtr->m_lootInventory.GetFirstAvailableSlot(mobManagerPtr->mobs[i]->inventory.slots[x][y].Items[0].item).y;
										if (X != -1 && Y != -1)
										{
											playerPtr->m_lootInventory.slots[X][Y].Items.push_back(mobManagerPtr->mobs[i]->inventory.slots[x][y].Items[it]);
											if (playerPtr->m_lootInventory.full())
											{
												broken = true;
												break;
											}
										}
									}
								}
							}
						}
					}
				}
				if (!playerPtr->m_lootInventory.empty())
				{
					for (int x = 0, y = 0; x < targetPtr->inventory.width; x++)
					{
						for (y = 0; y < targetPtr->inventory.height; y++)
						{
							if (!targetPtr->inventory.slots[x][y].Items.empty())
							{
								std::cout << "-Mob[" << targetPtr->ID << "]: " <<  targetPtr->inventory.slots[x][y].Items[0].item <<  " Q = " << targetPtr->inventory.slots[x][y].Items.size() << " @(" << x << ";" << y << ")\n";
							}
						}
					}
					playerPtr->lootState = true;
					playerPtr->inventoryState = true;
					std::cout << "\n";
				}
				targetPtr = nullptr;
			}
		}
	}
	leftMouseClicked = false;
	rightMouseClicked = false;
}

void PlayerController::handleEvents(sf::Event const& event)
{
	if (event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		leftMouseClicked = true;
	}
	if (event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
	{
		rightMouseClicked = true;
	}
}

void PlayerController::getPath(float x, float y)
{
	if (mapPtr->isPathable(x, y))
	{
		sf::Vector2i start(playerPtr->getPosition().x / WIDTH, playerPtr->getPosition().y / HEIGHT);
		mPathStart = playerPtr->getPosition();
		std::vector<sf::Vector2i> tilepath = pathFinderPtr->GetPath(start, sf::Vector2i(x, y), true);
		std::vector<sf::Vector2f> pospath;

		for (auto v = tilepath.begin(); v != tilepath.end()-1; ++v)
		{
			sf::Vector2f w;
			w.x = v->x * WIDTH + WIDTH/2;
			w.y = v->y * HEIGHT + HEIGHT/2;
			pospath.push_back(w);
		}
		playerPtr->path(pospath);
	}
}