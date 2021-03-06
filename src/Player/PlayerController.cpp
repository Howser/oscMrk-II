#include "Player\PlayerController.h"


// CONSTS
namespace {
	const sf::Vector2f speed(5, 5);
}

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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		playerPtr->m_path.clear();
		move_up();
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		playerPtr->m_path.clear();
		move_down();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		playerPtr->m_path.clear();
		move_left();
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		playerPtr->m_path.clear();
		move_right();
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		stop();
	}

	// Constrain the speed if player is moving diagonaly 
	if (vec::length(playerPtr->m_velocity) > speed.x) {
		playerPtr->m_velocity = vec::normalize(playerPtr->m_velocity) * speed.x;
	}

	if (leftMouseClicked)
	{
		sf::Vector2i mpos = sf::Mouse::getPosition(window);
		mpos.x += playerPtr->getPosition().x - view.getSize().x/2;
		mpos.y += playerPtr->getPosition().y - view.getSize().y/2;

		targetPtr = mobManagerPtr->getAtPosition(mpos.x, mpos.y);
		if (targetPtr != NULL)
		{
			if (targetPtr->dead)// && math::distance(targetPtr->getPosition(), playerPtr->getPosition()) <= WIDTH)
			{
				playerPtr->m_lootInventory.clear(false);
				sf::Vector2<int> points[2] = {sf::Vector2<int>(playerPtr->getPosition().x - LOOT_DISTANCE, playerPtr->getPosition().y - LOOT_DISTANCE), sf::Vector2<int>(playerPtr->getPosition().x + LOOT_DISTANCE, playerPtr->getPosition().y + LOOT_DISTANCE)};
				std::vector<mobtree::Branch*> branches;
				for (int x = points[0].x/mobtree::SIZE, y = points[0].y/mobtree::SIZE; x < points[1].x/mobtree::SIZE; x += mobtree::SIZE)
				{
					for (int y = points[0].y/mobtree::SIZE; y < points[1].y/mobtree::SIZE; y += mobtree::SIZE)
					{
						mobtree::Branch* ptr_branch = mobManagerPtr->m_tree.search(sf::Vector2f(x*mobtree::SIZE, y*mobtree::SIZE));
						if (ptr_branch != NULL)
						{
							branches.push_back(ptr_branch);
						}
					}
				}
				for (int b = 0; b < branches.size(); b++)
				{
					for (int i = 0; i < branches[b]->mobs.size(); i++)
					{
						if (branches[b]->mobs[i]->dead && math::distance(branches[b]->mobs[i]->getPosition(), playerPtr->getPosition()) <= LOOT_DISTANCE)
						{
							if (!branches[b]->mobs[i]->inventory.empty())
							{
								bool broken = false;
								playerPtr->m_lootMobs.push_back(branches[b]->mobs[i]->ID);
								for (int x = 0, y = 0; x < branches[b]->mobs[i]->inventory.width; x++)
								{
									if (broken)
									{
										break;
									}
									for (y = 0; y < branches[b]->mobs[i]->inventory.height; y++)
									{
										if (broken)
										{
											break;
										}
										for (int it = 0; it < branches[b]->mobs[i]->inventory.slots[x][y].Items.size(); it++)
										{
											int X = playerPtr->m_lootInventory.GetFirstAvailableSlot(branches[b]->mobs[i]->inventory.slots[x][y].Items[0].item).x;
											int Y = playerPtr->m_lootInventory.GetFirstAvailableSlot(branches[b]->mobs[i]->inventory.slots[x][y].Items[0].item).y;
											if (X != -1 && Y != -1)
											{
												playerPtr->m_lootInventory.slots[X][Y].Items.push_back(branches[b]->mobs[i]->inventory.slots[x][y].Items[it]);
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
				}
				targetPtr = NULL;
			}
		}
	}
	if ((rightMouseClicked && sf::Mouse::isButtonPressed(sf::Mouse::Right)) || (leftMouseClicked && sf::Mouse::isButtonPressed(sf::Mouse::Left)))
	{
		sf::Vector2i mpos = sf::Mouse::getPosition(window);
		mpos.x += playerPtr->getPosition().x - view.getSize().x/2;
		mpos.y += playerPtr->getPosition().y - view.getSize().y/2;
		targetPtr = mobManagerPtr->getAtPosition(mpos.x, mpos.y);
		if (targetPtr == NULL)
		{
			playerPtr->attack(window, (Attack)!sf::Mouse::isButtonPressed(sf::Mouse::Left));
		}else if (!(targetPtr->dead && math::distance(targetPtr->getPosition(), playerPtr->getPosition()) > WIDTH && !targetPtr->inventory.empty()))
		{
			playerPtr->attack(window, (Attack)!sf::Mouse::isButtonPressed(sf::Mouse::Left));
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


void PlayerController::stop(){
	playerPtr->m_velocity.x = 0;
	playerPtr->m_velocity.y = 0;
}

bool PlayerController::intersects_wall(const sf::Rect<int> & p_rect){
	for (int x = ((p_rect.left)/WIDTH - 1 >= 0) ? (p_rect.left)/WIDTH - 1:0, y = ((p_rect.top)/HEIGHT - 1 >= 0) ? (p_rect.top)/HEIGHT - 1:0; x < (p_rect.left + p_rect.width)/WIDTH + 1; x++)
	{
		for (int y = ((p_rect.top)/HEIGHT - 1 >= 0) ? (p_rect.top)/HEIGHT - 1:0; y < (p_rect.top + p_rect.height)/HEIGHT + 1; y++)
		{
			if (mapPtr->tiles[x][y].type != 1)
			{
				if (mapPtr->tiles[x][y].type == 3)
				{
					mapPtr->loaded = false;
				}
				if (p_rect.intersects(sf::Rect<int>(x*WIDTH, y*HEIGHT, WIDTH, HEIGHT)))
				{
					return true;
				}
			}
		}
	}
	return false;
}