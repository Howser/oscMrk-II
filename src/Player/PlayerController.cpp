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

void PlayerController::update(sf::Time dt, sf::RenderWindow const& window, sf::View const& view, LightManager* ptr_light_manager)
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			playerPtr->m_path.clear();
			move_up();
		}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			playerPtr->m_path.clear();
			move_down();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			playerPtr->m_path.clear();
			move_left();
		}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			playerPtr->m_path.clear();
			move_right();
		}
	}

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

const sf::Vector2f speed(5, 5);

void PlayerController::move_up(){
	sf::Rect<int> rect = sf::Rect<int>(playerPtr->getPosition().x - 16 + 5, playerPtr->getPosition().y - speed.y + 1, 20, 15);
	if (!intersects_wall(rect))
	{
		playerPtr->m_velocity.y = -speed.y;
	}else
	{
		playerPtr->m_velocity.y = 0;
	}
}
	
void PlayerController::move_down(){
	sf::Rect<int> rect = sf::Rect<int>(playerPtr->getPosition().x - 16 + 5, playerPtr->getPosition().y + speed.y + 1, 20, 15);
	if (!intersects_wall(rect))
	{
		playerPtr->m_velocity.y = speed.y;
	}else
	{
		playerPtr->m_velocity.y = 0;
	}
}

void PlayerController::move_left(){
	sf::Rect<int> rect = sf::Rect<int>(playerPtr->getPosition().x - 16 - speed.x + 5, playerPtr->getPosition().y + 1, 20, 15);
	if (!intersects_wall(rect))
	{
		playerPtr->m_velocity.x = -speed.x;
	}else
	{
		playerPtr->m_velocity.x = 0;
	}
}

void PlayerController::move_right(){
	sf::Rect<int> rect = sf::Rect<int>(playerPtr->getPosition().x - 16 + speed.x + 5, playerPtr->getPosition().y + 1, 20, 15);
	if (!intersects_wall(rect))
	{
		playerPtr->m_velocity.x = speed.x;
	}else
	{
		playerPtr->m_velocity.x = 0;
	}
}

void PlayerController::check_collision(){
	int w = 0, h = 0;
	for (int x = playerPtr->getPosition().x/WIDTH - 1, y = playerPtr->getPosition().y/HEIGHT - 1; x < playerPtr->getPosition().x/WIDTH + 1; x++)
	{
		for (y = playerPtr->getPosition().y/HEIGHT - 1; y < playerPtr->getPosition().y/HEIGHT + 1; y++)
		{
			if (mapPtr->tiles[x][y].type != 1)
			{
				if (sf::Rect<int>(playerPtr->getPosition().x - 16, playerPtr->getPosition().y - 16, 32, 32).intersects(sf::Rect<int>(x*WIDTH, y*HEIGHT, WIDTH, HEIGHT)))
				{
					w += (playerPtr->getPosition().x - 16 > x*WIDTH) ? x*WIDTH + WIDTH - playerPtr->getPosition().x - 16:playerPtr->getPosition().x + 16 - x*WIDTH;
					h += (playerPtr->getPosition().y - 16 > y*HEIGHT) ? y*HEIGHT + HEIGHT - playerPtr->getPosition().y - 16 : playerPtr->getPosition().y + 16 - y*HEIGHT;
					if (h < w)
					{
						playerPtr->m_velocity.y = 0;
						if (playerPtr->getPosition().y < y*HEIGHT + HEIGHT/2)
						{
							playerPtr->setPosition(playerPtr->getPosition().x, y*HEIGHT - 16);
						}else
						{
							playerPtr->setPosition(playerPtr->getPosition().x, y*HEIGHT + HEIGHT + 16);
						}
						break;
					}else
					{
						playerPtr->m_velocity.x = 0;
						if (playerPtr->getPosition().x < x*WIDTH + WIDTH/2)
						{
							playerPtr->setPosition(x*WIDTH - 16, playerPtr->getPosition().y);
						}else
						{
							playerPtr->setPosition(x*WIDTH + WIDTH + 16, playerPtr->getPosition().y);
						}
						break;
					}
				}
			}
		}
	}
}

bool PlayerController::intersects_wall(const sf::Rect<int> & p_rect){
	for (int x = (p_rect.left)/WIDTH - 1, y = (p_rect.top)/HEIGHT - 1; x < (p_rect.left + p_rect.width)/WIDTH + 1; x++)
	{
		for (int y = (p_rect.top)/HEIGHT - 1; y < (p_rect.top + p_rect.height)/HEIGHT + 1; y++)
		{
			if (mapPtr->tiles[x][y].type != 1)
			{
				if (p_rect.intersects(sf::Rect<int>(x*WIDTH, y*HEIGHT, WIDTH, HEIGHT)))
				{
					return true;
				}
			}
		}
	}
	return false;
}