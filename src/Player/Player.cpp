#include "Player\Player.h"

#include "Math\VectorMath.h"

#include <SFML\Graphics\RenderTarget.hpp>

#include <iostream>

#include "Item\ItemManager.h"

Player::Player(TextureHolder* textures, FontHolder* fonts, std::vector<Mob*>* mobsPtr, ProjectileManager* p_projectile_manager, std::vector<std::vector<gen::Tile>>* ptr_tiles, LightManager* ptr_light_manager)
	:
	ptr_tiles(ptr_tiles),
	p_projectile_manager(p_projectile_manager),
	p_texture_holder(textures),
	p_mobs(mobsPtr),
	m_sprite(),
	m_velocity(),
	m_path(),
	m_animation(sf::Vector2i(24, 23), 3, .2f),
	m_inventory(10, 10, 640 - (textures->getTexture(Textures::gui_Inventory)->getSize().x + textures->getTexture(Textures::gui_LootInventory)->getSize().x - 3)/2 + 3, 360 - (textures->getTexture(Textures::gui_Inventory)->getSize().y)/2 + 5, *textures),
	m_lootInventory(10, 10, 640 - (textures->getTexture(Textures::gui_Inventory)->getSize().x + textures->getTexture(Textures::gui_LootInventory)->getSize().x - 3)/2 + textures->getTexture(Textures::gui_Inventory)->getSize().x + 20, 360 - (textures->getTexture(Textures::gui_Inventory)->getSize().y)/2 + 5, *textures),
	m_Gear(*textures, m_inventory.width*SLOTWIDTH + SLOTWIDTH, 640 - (textures->getTexture(Textures::gui_Inventory)->getSize().x + textures->getTexture(Textures::gui_LootInventory)->getSize().x - 3)/2 + 10, 360 - (textures->getTexture(Textures::gui_Inventory)->getSize().y)/2 + 44),
	inventoryState(false),
	lootState(false),
	tabClicked(false),
	leftMouseClicked(false),
	m_stackManager(*textures),
	m_tooltip(*textures),
	m_deleteItem(*textures),
	m_invRect(sf::Rect<int>(640 - (textures->getTexture(Textures::gui_Inventory)->getSize().x + textures->getTexture(Textures::gui_LootInventory)->getSize().x - 3)/2 + 3, 360 - (textures->getTexture(Textures::gui_Inventory)->getSize().y)/2 + 5, m_inventory.width*SLOTWIDTH + m_inventory.width*5, m_inventory.height*SLOTHEIGHT + m_inventory.height*5)),
	m_lootInvRect(sf::Rect<int>(640 - (textures->getTexture(Textures::gui_Inventory)->getSize().x + textures->getTexture(Textures::gui_LootInventory)->getSize().x - 3)/2 + textures->getTexture(Textures::gui_Inventory)->getSize().x + 20, 360 - (textures->getTexture(Textures::gui_Inventory)->getSize().y)/2 + 5, m_lootInventory.width*SLOTWIDTH + m_lootInventory.width*5, m_lootInventory.height*SLOTHEIGHT + m_lootInventory.width*5)),
	m_healthbar(*(textures->getTexture(Textures::HealthEmpty)), *(textures->getTexture(Textures::HealthFull))),
	ptr_light_manager(ptr_light_manager)
{
	m_sprite.setTexture(*(textures->getTexture(Textures::Player)));
	m_sprite.setScale(1 + (float)1/3, 1 + (float)1/3);
	m_overlay.setTexture(*(textures->getTexture(Textures::PlayerOverlay)));
	m_mouseSlot.fontPtr = fonts->getFont(Fonts::Main);
	setOrigin(16, 16);
	m_inventory.slots[0][0].Items.push_back(GearItem(Items::Sword, *textures, -1));
	m_inventory.slots[1][0].Items.push_back(GearItem(Items::Shield, *textures, -1));
	m_inventory.slots[2][0].Items.push_back(GearItem(Items::Mace, *textures, -1));
	m_inventory.slots[3][0].Items.push_back(GearItem(Items::Bow, *textures, -1));
	for (int i = 0; i < 101; i++)
	{
		m_inventory.slots[m_inventory.GetFirstAvailableSlot(Items::TestSpell).x][m_inventory.GetFirstAvailableSlot(Items::TestSpell).y].Items.push_back(MiscItem(Items::TestSpell, *textures, -1));
	}
	m_inventory.slots[m_inventory.GetFirstAvailableSlot(Items::Armor_Chaos).x][m_inventory.GetFirstAvailableSlot(Items::Armor_Chaos).y].Items.push_back(MiscItem(Items::Armor_Chaos, *textures, -1));
	m_inventory.slots[m_inventory.GetFirstAvailableSlot(Items::Helmet_Chaos).x][m_inventory.GetFirstAvailableSlot(Items::Helmet_Chaos).y].Items.push_back(MiscItem(Items::Helmet_Chaos, *textures, -1));
	m_inventory.slots[m_inventory.GetFirstAvailableSlot(Items::Armor_Destruction).x][m_inventory.GetFirstAvailableSlot(Items::Armor_Destruction).y].Items.push_back(MiscItem(Items::Armor_Destruction, *textures, -1));
	m_inventory.slots[m_inventory.GetFirstAvailableSlot(Items::Helmet_Destruction).x][m_inventory.GetFirstAvailableSlot(Items::Helmet_Destruction).y].Items.push_back(MiscItem(Items::Helmet_Destruction, *textures, -1));
	m_inventory.slots[m_inventory.GetFirstAvailableSlot(Items::Armor_Darkness).x][m_inventory.GetFirstAvailableSlot(Items::Armor_Darkness).y].Items.push_back(MiscItem(Items::Armor_Darkness, *textures, -1));
	m_inventory.slots[m_inventory.GetFirstAvailableSlot(Items::Helmet_Darkness).x][m_inventory.GetFirstAvailableSlot(Items::Helmet_Darkness).y].Items.push_back(MiscItem(Items::Helmet_Darkness, *textures, -1));
	Player::m_HelmetSlot = Items::NOITEM;
	Player::m_ArmorSlot = Items::NOITEM;
	Player::m_lHandSlot = Items::NOITEM;
	Player::m_rHandSlot = Items::NOITEM;
	m_Gear.slots[0].slot = eGearSlot::Helmet;
	m_Gear.slots[1].slot = eGearSlot::Armor;
	m_Gear.slots[2].slot = eGearSlot::lHand;
	m_Gear.slots[3].slot = eGearSlot::rHand;

	for (int i = 0; i < 4; i++)
	{
		m_d_gear.push_back(D_Gear());
		m_d_gear.back().setPosition(getPosition());
		m_d_gear.back().m_animation = Animation(sf::Vector2i(24, 23), 3, 0.1f);
	}
	m_attackTimer.restart();
	m_healthbar.setPosition(0, 720 - 129);
	m_healthbar.mLoaded.setColor(sf::Color(255, 100, 40, 255));
	m_overlay.setPosition(m_healthbar.getPosition());
	m_health = 100;

	m_ability[0].setTexture(*(textures->getTexture(Textures::Ability)));
	m_ability[0].setPosition(1280 - 130, 720 - 129);
	m_ability[1].setPosition(m_ability[0].getPosition().x + 15, m_ability[0].getPosition().y + 18);
	m_ability[2].setPosition(m_ability[0].getPosition().x + 80, m_ability[0].getPosition().y + 18);

	m_gui_inventory.setTexture(*(textures->getTexture(Textures::gui_Inventory)));
	m_gui_inventory.setPosition(640 - (textures->getTexture(Textures::gui_Inventory)->getSize().x + textures->getTexture(Textures::gui_LootInventory)->getSize().x - 3)/2, 360 - (textures->getTexture(Textures::gui_Inventory)->getSize().y)/2);
	m_gui_loot_inventory.setTexture(*(textures->getTexture(Textures::gui_LootInventory)));
	m_gui_loot_inventory.setPosition(m_gui_inventory.getPosition().x + m_gui_inventory.getTextureRect().width - 3, m_gui_inventory.getPosition().y);
	m_dir = Direction::Down;
}

Player::~Player()
{

}

void Player::stop()
{
	m_path.clear();
	m_velocity.x = 0;
	m_velocity.y = 0;
}

void Player::path(std::vector<sf::Vector2f>& path)
{
	m_path = path;
}

const sf::Vector2f speed = sf::Vector2f(5, 5);

void Player::update(sf::Time dt, sf::RenderWindow const& window)
{
	if (tabClicked)
	{
		inventoryState = true;
	}
	if (!inventoryState)
	{
		setPosition(getPosition().x + m_velocity.x, getPosition().y + m_velocity.y);
		
		m_velocity.x = 0;
		m_velocity.y = 0;

		if (std::abs(m_velocity.x) > std::abs(m_velocity.y))
		{
			if (m_velocity.x > 0)
			{
				m_dir = Direction::Right;
			}else if (m_velocity.x < 0)
			{
				m_dir = Direction::Left;
			}
		}else
		{
			if (m_velocity.y > 0)
			{
				m_dir = Direction::Down;
			}else if (m_velocity.y < 0)
			{
				m_dir = Direction::Up;
			}
		}

		switch (m_dir)
		{
		case Up:
			if (!m_animation.isPlaying(0))
			{
				m_animation.loop(0);
				for (int i = 0; i < m_d_gear.size(); i++)
				{
					m_d_gear[i].m_animation.loop(0);
				}
			}
			break;
		case Down:
			if (!m_animation.isPlaying(2))
			{
				m_animation.loop(2);
				for (int i = 0; i < m_d_gear.size(); i++)
				{
					m_d_gear[i].m_animation.loop(2);
				}
			}
			break;
		case Left:
			if (!m_animation.isPlaying(3))
			{
				m_animation.loop(3);
				for (int i = 0; i < m_d_gear.size(); i++)
				{
					m_d_gear[i].m_animation.loop(3);
				}
			}
			break;
		case Right:
			if (!m_animation.isPlaying(1))
			{
				m_animation.loop(1);
				for (int i = 0; i < m_d_gear.size(); i++)
				{
					m_d_gear[i].m_animation.loop(1);
				}
			}
			break;
		default:
			break;
		}
		if (m_velocity.x != 0 || m_velocity.y != 0)
		{
			m_animation.update();
			for (int i = 0; i < m_d_gear.size(); i++)
			{
				m_d_gear[i].m_animation.update();
			}
		}

		stop();

		m_sprite.setTextureRect(m_animation.getFrame());
		for (int i = 0; i < m_d_gear.size(); i++)
		{
			m_d_gear[i].m_sprite.setTextureRect(m_animation.getFrame());
		}

		m_d_gear[0].setPosition(getPosition().x - 16, getPosition().y - 16);
		m_d_gear[1].setPosition(getPosition().x - 16, getPosition().y - 16);
		m_d_gear[2].setPosition(getPosition().x - 16, getPosition().y - 16);
		m_d_gear[3].setPosition(getPosition().x - 16, getPosition().y - 16);
	}
	resetInputs();
	m_healthbar.updateStatus((float)m_health / 100.f);
	m_healthbar.update();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
		m_health -= 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		m_health += 1;
}

void Player::updateInventory(sf::RenderWindow const& window, TextureHolder & textures){
	if (tabClicked)
	{
		rightMouseClicked = false;
		m_stackManager.Hide();
		m_deleteItem.Toggle(false, NULL);
		m_tooltip.Hide();
		m_lootInventory.clear(false);
		m_lootMobs.clear();
		inventoryState = false;
		lootState = false;
	}
	m_mouseSlot.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

	if (!m_stackManager.show && !m_deleteItem.show)
	{
		if (sf::Rect<int>(m_mouseSlot.getPosition().x, m_mouseSlot.getPosition().y, 1, 1).intersects(m_invRect) || (lootState && sf::Rect<int>(m_mouseSlot.getPosition().x, m_mouseSlot.getPosition().y, 1, 1).intersects(m_lootInvRect)) || sf::Rect<int>(m_mouseSlot.getPosition().x, m_mouseSlot.getPosition().y, 1, 1).intersects(sf::Rect<int>(m_gui_inventory.getPosition().x + 372, m_gui_inventory.getPosition().y + 44, SLOTWIDTH*3, SLOTHEIGHT*2)))//here
		{
			bool broken = false;
			for (unsigned int i = 0; i < m_Gear.slots.size(); i++)
			{
				if (sf::Rect<int>(m_mouseSlot.getPosition().x, m_mouseSlot.getPosition().y, 1, 1).intersects(sf::Rect<int>(m_Gear.slots[i].getPosition().x, m_Gear.slots[i].getPosition().y, SLOTWIDTH, SLOTHEIGHT)))
				{
					if (!m_Gear.slots[i].Items.empty())
					{
						if (m_tooltip.slot != &m_Gear.slots[i])
						{
							m_tooltip.SetStats(&m_Gear.slots[i]);
						}
					}else
					{
						m_tooltip.Hide();
					}
				}
			}
			for (unsigned int x = 0, y = 0; x < m_inventory.width; x++)
			{
				for (y = 0; y < m_inventory.height; y++)
				{
					if (sf::Rect<int>(m_mouseSlot.getPosition().x, m_mouseSlot.getPosition().y, 1, 1).intersects(sf::Rect<int>(m_inventory.slots[x][y].getPosition().x, m_inventory.slots[x][y].getPosition().y, SLOTWIDTH, SLOTHEIGHT)))
					{
						if (!m_inventory.slots[x][y].Items.empty())
						{
							if (m_tooltip.slot != &m_inventory.slots[x][y])
							{
								m_tooltip.SetStats(&m_inventory.slots[x][y]);
							}
						}else
						{
							m_tooltip.Hide();
						}
						if (leftMouseClicked)
						{
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
							{
								m_stackManager.Show(&m_inventory.slots[x][y]);
							}else
							{
								m_inventory.Command(&m_inventory.slots[x][y], &m_mouseSlot, inv::Move, &m_stackManager);
								if (m_mouseSlot.Items.empty() && !m_inventory.slots[x][y].Items.empty())
								{
									if (m_inventory.slots[x][y].Items[0].ID != -1)
									{
										for (int i = 0; i < m_inventory.slots[x][y].Items.size(); i++)
										{
											m_inventory.slots[x][y].Items[i].ID = -1;
										}
									}
								}
							}
							broken = true;
							break;
						}
					}else if (lootState && sf::Rect<int>(m_mouseSlot.getPosition().x, m_mouseSlot.getPosition().y, 1, 1).intersects(sf::Rect<int>(m_lootInventory.slots[x][y].getPosition().x, m_lootInventory.slots[x][y].getPosition().y, SLOTWIDTH, SLOTHEIGHT)))
					{
						if (!m_lootInventory.slots[x][y].Items.empty())
						{
							if (m_tooltip.slot != &m_lootInventory.slots[x][y])
							{
								m_tooltip.SetStats(&m_lootInventory.slots[x][y]);
							}
						}else
						{
							m_tooltip.Hide();
						}
						if (leftMouseClicked)
						{
							if (!m_mouseSlot.Items.empty())// && lootInventory.slots[x][y].Items.empty())
							{
								for (int i = 0; i < m_lootMobs.size(); i++)
								{
									if ((*p_mobs)[m_lootMobs[i]]->inventory.GetFirstAvailableSlot(m_mouseSlot.Items[0].item).x != -1)
									{
										std::cout << &(*p_mobs)[m_lootMobs[i]] <<"\n";
										for (int j = 0; j < m_mouseSlot.Items.size(); j++)
										{
											m_mouseSlot.Items[j].ID = m_lootMobs[i];
											(*p_mobs)[m_lootMobs[i]]->inventory.slots[(*p_mobs)[m_lootMobs[i]]->inventory.GetFirstAvailableSlot(m_mouseSlot.Items[0].item).x][(*p_mobs)[m_lootMobs[i]]->inventory.GetFirstAvailableSlot(m_mouseSlot.Items[0].item).y].Items.push_back(m_mouseSlot.Items[j]);
										}
										std::cout << (*p_mobs)[m_lootMobs[i]]->inventory.slots[(*p_mobs)[m_lootMobs[i]]->inventory.GetFirstAvailableSlot(m_mouseSlot.Items[0].item).x][(*p_mobs)[m_lootMobs[i]]->inventory.GetFirstAvailableSlot(m_mouseSlot.Items[0].item).y].Items.size() << "\n";
										break;
									}
								}
							}else if (m_mouseSlot.Items.empty() && !m_lootInventory.slots[x][y].Items.empty())
							{
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
								{
									m_stackManager.Show(&m_lootInventory.slots[x][y]);
								}else
								{
									for (int i = 0; i < m_lootInventory.slots[x][y].Items.size(); i++)
									{
										(*p_mobs)[m_lootInventory.slots[x][y].Items[i].ID]->inventory.RemoveItem(m_lootInventory.slots[x][y].Items[0].item, 1);
									}
								}
							}
							if (!m_stackManager.show)
							{
								m_lootInventory.Command(&m_lootInventory.slots[x][y], &m_mouseSlot, inv::Move, &m_stackManager);
							}
							broken = true;
							break;
						}
					}
				}
				if (broken)
				{
					break;
				}
			}
		}else
		{
			m_tooltip.Hide();
			if (leftMouseClicked && !m_mouseSlot.Items.empty())
			{
				m_deleteItem.Toggle(true, &m_mouseSlot);
			}
		}
		if (rightMouseClicked)
		{
			for (unsigned int x = 0, y = 0; x < m_inventory.width; x++)
			{
				for (y = 0; y < m_inventory.height; y++)
				{
					if (sf::Rect<int>(m_mouseSlot.getPosition().x, m_mouseSlot.getPosition().y, 1, 1).intersects(sf::Rect<int>(m_inventory.slots[x][y].getPosition().x, m_inventory.slots[x][y].getPosition().y, SLOTWIDTH, SLOTHEIGHT)))
					{
						if (m_inventory.slots[x][y].Items.size() > 0)
						{
							if (GetType(m_inventory.slots[x][y].Items[0].item) == itemType::Gear)
							{
								GearItem item = GearItem(m_inventory.slots[x][y].Items[0].item, textures, -1);
								switch (item.eGearSlot)
								{
								case Helmet:
									item.Equip(&m_Gear.slots[0], &m_inventory.slots[x][y], &m_inventory);
									m_d_gear[0].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(Textures::d_Arrow + m_Gear.slots[0].Items[0].item))));
									break;
								case Armor:
									item.Equip(&m_Gear.slots[1], &m_inventory.slots[x][y], &m_inventory);
									m_d_gear[1].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(Textures::d_Arrow + m_Gear.slots[1].Items[0].item))));
									m_d_gear[1].m_sprite.setScale(1 + (float)1/3, 1 + (float)1/3);
									break;
								case lHand:
									if (!m_Gear.slots[2].Items.empty() && GetSlot(m_Gear.slots[2].Items.begin()->item) == eGearSlot::TwoHand)
									{
										if (m_inventory.GetFirstAvailableSlot(m_Gear.slots[2].Items.begin()->item).x != -1)
										{
											m_inventory.slots[m_inventory.GetFirstAvailableSlot(m_Gear.slots[2].Items.begin()->item).x][m_inventory.GetFirstAvailableSlot(m_Gear.slots[2].Items.begin()->item).y].Items.push_back(*m_Gear.slots[2].Items.begin());
											m_Gear.slots[2].Items.clear();

											item.Equip(&m_Gear.slots[3], &m_inventory.slots[x][y], &m_inventory);

											m_d_gear[3].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(Textures::d_Arrow + m_Gear.slots[3].Items[0].item))));
											m_ability[1] = item.sprite;
										}
									}else
									{
										item.Equip(&m_Gear.slots[3], &m_inventory.slots[x][y], &m_inventory);
										m_d_gear[3].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(Textures::d_Arrow + m_Gear.slots[3].Items[0].item))));
										m_ability[1] = item.sprite;
									}
									break;
								case rHand:
									if (!m_Gear.slots[2].Items.empty() && GetSlot(m_Gear.slots[2].Items.begin()->item) == eGearSlot::TwoHand)
									{
										if (m_inventory.GetFirstAvailableSlot(m_Gear.slots[2].Items.begin()->item).x != -1)
										{
											m_inventory.slots[m_inventory.GetFirstAvailableSlot(m_Gear.slots[2].Items.begin()->item).x][m_inventory.GetFirstAvailableSlot(m_Gear.slots[2].Items.begin()->item).y].Items.push_back(*m_Gear.slots[2].Items.begin());
											m_Gear.slots[2].Items.clear();

											item.Equip(&m_Gear.slots[2], &m_inventory.slots[x][y], &m_inventory);
											m_d_gear[2].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(Textures::d_Arrow + m_Gear.slots[2].Items[0].item))));
											m_d_gear[2].setPosition(getPosition().x, getPosition().y);
											m_ability[2] = item.sprite;
										}
									}else
									{
										item.Equip(&m_Gear.slots[2], &m_inventory.slots[x][y], &m_inventory);
										m_d_gear[2].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(Textures::d_Arrow + m_Gear.slots[2].Items[0].item))));
										m_ability[2] = item.sprite;
									}
									break;
								case TwoHand:
									if (!m_Gear.slots[3].Items.empty())
									{
										m_inventory.slots[m_inventory.GetFirstAvailableSlot(m_Gear.slots[3].Items.begin()->item).x][m_inventory.GetFirstAvailableSlot(m_Gear.slots[3].Items.begin()->item).y].Items.push_back(*m_Gear.slots[3].Items.begin());
										m_Gear.slots[3].Items.clear();
									}
									if (!m_Gear.slots[2].Items.empty())
									{
										m_inventory.slots[m_inventory.GetFirstAvailableSlot(m_Gear.slots[2].Items.begin()->item).x][m_inventory.GetFirstAvailableSlot(m_Gear.slots[2].Items.begin()->item).y].Items.push_back(*m_Gear.slots[2].Items.begin());
										m_Gear.slots[2].Items.clear();
									}
									if (m_Gear.slots[3].Items.empty() && m_Gear.slots[2].Items.empty())
									{
										item.Equip(&m_Gear.slots[2], &m_inventory.slots[x][y], &m_inventory);
										if (!IsSpell(m_Gear.slots[2].Items[0].item))
										{
											m_d_gear[2].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(Textures::d_Arrow + m_Gear.slots[2].Items[0].item))));
										}
										m_ability[2] = item.sprite;
									}
									break;
								case OneHand:
									if (!m_Gear.slots[2].Items.empty())
									{
										if (!m_Gear.slots[3].Items.empty())
										{
											m_inventory.slots[m_inventory.GetFirstAvailableSlot(m_Gear.slots[2].Items.begin()->item).x][m_inventory.GetFirstAvailableSlot(m_Gear.slots[2].Items.begin()->item).y].Items.push_back(*m_Gear.slots[2].Items.begin());
											m_Gear.slots[2].Items.clear();
											item.Equip(&m_Gear.slots[2], &m_inventory.slots[x][y], &m_inventory);
											if (!IsSpell(m_Gear.slots[2].Items[0].item))
											{
												m_d_gear[2].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(Textures::d_Arrow + m_Gear.slots[2].Items[0].item))));
											}
											m_ability[2] = item.sprite;
										}else
										{
											if (GetSlot(m_Gear.slots[2].Items[0].item) != eGearSlot::TwoHand)
											{
												item.Equip(&m_Gear.slots[3], &m_inventory.slots[x][y], &m_inventory);
												if (!IsSpell(m_Gear.slots[3].Items[0].item))
												{
													m_d_gear[3].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(Textures::d_Arrow + m_Gear.slots[3].Items[0].item))));
												}
												m_ability[2] = item.sprite;
											}else
											{
												m_inventory.slots[m_inventory.GetFirstAvailableSlot(m_Gear.slots[2].Items.begin()->item).x][m_inventory.GetFirstAvailableSlot(m_Gear.slots[2].Items.begin()->item).y].Items.push_back(*m_Gear.slots[2].Items.begin());
												m_Gear.slots[2].Items.clear();
												item.Equip(&m_Gear.slots[2], &m_inventory.slots[x][y], &m_inventory);
												if (!IsSpell(m_Gear.slots[2].Items[0].item))
												{
													m_d_gear[2].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(Textures::d_Arrow + m_Gear.slots[3].Items[0].item))));
												}
												m_ability[2] = item.sprite;
											}
										}
									}else
									{
										item.Equip(&m_Gear.slots[2], &m_inventory.slots[x][y], &m_inventory);
										if (!IsSpell(m_Gear.slots[2].Items[0].item))
										{
											m_d_gear[2].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(Textures::d_Arrow + m_Gear.slots[2].Items[0].item))));
										}
										m_ability[1] = item.sprite;
									}
									break;
								}
								m_ability[2].setPosition(m_ability[0].getPosition().x + 15, m_ability[0].getPosition().y + 18);
								m_ability[1].setPosition(m_ability[0].getPosition().x + 80, m_ability[0].getPosition().y + 18);
							}
							else
							{
								m_inventory.Command(&m_inventory.slots[x][y], &m_mouseSlot, inv::Use, &m_stackManager);
							}
						}
					}else if(sf::Rect<int>(m_mouseSlot.getPosition().x, m_mouseSlot.getPosition().y, 1, 1).intersects(sf::Rect<int>(m_lootInventory.slots[x][y].getPosition().x, m_lootInventory.slots[x][y].getPosition().y, SLOTWIDTH, SLOTHEIGHT)))
					{
						if (!m_lootInventory.slots[x][y].Items.empty())
						{
							for (int i = 0; i < m_lootInventory.slots[x][y].Items.size(); i++)
							{
								m_inventory.slots[m_inventory.GetFirstAvailableSlot(m_lootInventory.slots[x][y].Items[0].item).x][m_inventory.GetFirstAvailableSlot(m_lootInventory.slots[x][y].Items[0].item).y].Items.push_back(m_lootInventory.slots[x][y].Items[0]);
								(*p_mobs)[m_lootInventory.slots[x][y].Items[0].ID]->inventory.RemoveItem(m_lootInventory.slots[x][y].Items[0].item, 1);
							}
							m_lootInventory.slots[x][y].Items.clear();
						}
					}
				}
			}
			for (unsigned int i = 0; i < m_Gear.slots.size(); i++)
			{
				if (sf::Rect<int>(m_mouseSlot.getPosition().x, m_mouseSlot.getPosition().y, 1, 1).intersects(sf::Rect<int>(m_Gear.slots[i].getPosition().x, m_Gear.slots[i].getPosition().y, SLOTWIDTH, SLOTHEIGHT)))
				{
					m_Gear.Command(&m_Gear.slots[i], &m_inventory);
				}
			}
		}
	}else
	{
		if (!m_deleteItem.show)
		{
			if (leftMouseClicked || mouseWheelDelta != 0)
			{
				sf::Rect<int>mousePos = sf::Rect<int>(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 1, 1);
				if ((mousePos.intersects(m_stackManager.increase) && leftMouseClicked) || mouseWheelDelta == 1)
				{
					m_stackManager.Increase();
				}else if ((mousePos.intersects(m_stackManager.decrease) && leftMouseClicked) ||mouseWheelDelta == -1)
				{
					m_stackManager.Decrease();
				}else if (mousePos.intersects(m_stackManager.pickUp) && leftMouseClicked)
				{
					int id = m_stackManager.slotPtr->Items[0].ID;
					Items item = m_stackManager.slotPtr->Items[0].item;
					bool broken = false;
					for (int x = 0, y = 0; x < m_inventory.width; x++)
					{
						if (broken)
						{
							break;
						}
						for (y = 0; y < m_inventory.height; y++)
						{
							if (&m_inventory.slots[x][y] == m_stackManager.slotPtr)
							{
								m_inventory.Command(m_stackManager.slotPtr, &m_mouseSlot, inv::Move, &m_stackManager);
								broken = true;
								break;
							}
						}
					}
					if (!broken)
					{
						m_lootInventory.Command(m_stackManager.slotPtr, &m_mouseSlot, inv::Move, &m_stackManager);
					}
					if (id != -1)
					{
						(*p_mobs)[id]->inventory.RemoveItem(item, m_stackManager.count);
					}
					m_stackManager.Hide();
				}
			}
		}else if (!m_stackManager.show)
		{
			if (leftMouseClicked)
			{
				if (sf::Rect<int>(m_mouseSlot.getPosition().x, m_mouseSlot.getPosition().y, 1, 1).intersects(m_deleteItem.deleteButton))
				{
					m_mouseSlot.Items.clear();
					m_deleteItem.Toggle(false, NULL);
				}else if (sf::Rect<int>(m_mouseSlot.getPosition().x, m_mouseSlot.getPosition().y, 1, 1).intersects(m_deleteItem.cancelButton))
				{
					m_deleteItem.Toggle(false, NULL);
				}
			}
		}
	}
	resetInputs();
}

bool Player::isPathing()
{
	return !m_path.empty();
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_sprite, states);

	for (int i = 0; i < m_Gear.slots.size(); i++)
	{
		if (!m_Gear.slots[i].Items.empty())
		{
			if (!IsSpell(m_Gear.slots[i].Items[0].item))
			{
				m_d_gear[i].draw(target, states);
			}
		}
	}
}

void Player::drawGUI(sf::RenderWindow* p_window, FontHolder* ptr_font_holder){

	p_window->draw(m_healthbar);
	p_window->draw(m_overlay);
	p_window->draw(m_ability[0]);
	if (!m_Gear.slots[2].Items.empty())
	{
		p_window->draw(m_ability[1]);
		sf::Text text;
		text.setFont(*ptr_font_holder->getFont(Fonts::Main));
		int a = m_inventory.amount(m_Gear.slots[2].Items.begin()->item);
		text.setString((a <= 99) ? std::to_string(m_inventory.amount(m_Gear.slots[2].Items.begin()->item)):"??");
		text.setPosition(m_ability[1].getPosition().x, m_ability[1].getPosition().y + 60);
		text.setColor(sf::Color(255, 255, 255, 255));
		p_window->draw(text);
	}
	if (!m_Gear.slots[3].Items.empty())
	{
		p_window->draw(m_ability[2]);
		sf::Text text;
		text.setFont(*ptr_font_holder->getFont(Fonts::Main));
		int a = m_inventory.amount(m_Gear.slots[3].Items.begin()->item);
		text.setString((a <= 99) ? std::to_string(m_inventory.amount(m_Gear.slots[3].Items.begin()->item)):"??");
		text.setPosition(m_ability[2].getPosition().x, m_ability[2].getPosition().y + 60);
		text.setColor(sf::Color(255, 255, 255, 255));
		p_window->draw(text);
	}
	if (inventoryState)
	{
		p_window->draw(m_gui_inventory);
		p_window->draw(m_inventory);
		p_window->draw(m_Gear);
		if (lootState)
		{
			p_window->draw(m_gui_loot_inventory);
			p_window->draw(m_lootInventory);
		}
		if (!m_mouseSlot.Items.empty())
		{
			p_window->draw(m_mouseSlot);
		}
		if (m_stackManager.show)
		{
			p_window->draw(m_stackManager);
		}
		if (m_tooltip.show && !m_stackManager.show && !m_deleteItem.show)
		{
			p_window->draw(m_tooltip);
		}
		if (m_deleteItem.show)
		{
			p_window->draw(m_deleteItem);
		}
	}
}

bool Player::isInRange(Mob* ptr_mob){
	if (!m_Gear.slots[2].Items.empty())
	{
		switch (m_Gear.slots[2].Items[0].item)
		{
		case Bow:
			return (math::distance(ptr_mob->getPosition(), getPosition()) <= 735 + GetWidth(ptr_mob->type)/2);
		default:
			return (math::distance(ptr_mob->getPosition(), getPosition()) <= MELEE_DISTANCE);
			break;
		}
	}else
	{
		return (math::distance(ptr_mob->getPosition(), getPosition()) <= MELEE_DISTANCE);
	}
}

void Player::attack(const sf::RenderWindow & window, Attack p_attack){
	switch (p_attack)
	{
	case aLeft:
		if (!m_Gear.slots[3].Items.empty())
		{
			switch (m_Gear.slots[3].Items[0].item)
			{
			case Items::Bow:
				if (m_inventory.contains(Items::Arrow) && m_attackTimer.getElapsedTime().asSeconds() > GetSpeed(Items::Bow))
				{
					m_inventory.RemoveItem(Items::Arrow, 1);
					float angle = std::atan2f(sf::Mouse::getPosition(window).y - 720/2, sf::Mouse::getPosition(window).x - 1280/2);
					sf::Sprite arrow_sprite;
					arrow_sprite.setTexture(*p_texture_holder->getTexture(Textures::d_Arrow));
					projectile::Arrow arrow = projectile::Arrow(ptr_tiles, angle, arrow_sprite, GetDamage(Items::Bow), Items::Arrow, p_projectile_manager->m_particleSystem);
					arrow.setPosition(getPosition());
					p_projectile_manager->m_arrows.push_back(arrow);
					m_attackTimer.restart();
				}
				break;
			case Items::TestSpell:
				if (m_attackTimer.getElapsedTime().asSeconds() > GetSpeed(Items::TestSpell))
				{
					if (m_inventory.contains(Items::TestSpell))
					{
						m_inventory.RemoveItem(Items::TestSpell, 1);
					}else
					{
						if (!m_Gear.slots[3].Items.empty() && m_Gear.slots[3].Items[0].item == Items::TestSpell)
						{
							m_Gear.slots[3].Items.pop_back();
						}else
						{
							break;
						}
					}
					float angle = std::atan2f(sf::Mouse::getPosition(window).y - 720/2, sf::Mouse::getPosition(window).x - 1280/2);
					sf::Sprite spell_sprite;
					spell_sprite.setTexture(*p_texture_holder->getTexture(Textures::Armor_Chaos));
					p_projectile_manager->m_spells.push_back(projectile::Spell(getPosition(), angle, Items::TestSpell, spell_sprite, GetDamage(Items::TestSpell), ptr_tiles, Light(sf::Color(math::random(1, 255), math::random(1, 255), math::random(1, 255), 255), sf::Vector3f(getPosition().x, getPosition().y, 0.075f), sf::Vector3f(0.f, 5.f, 0.f), false)));
					m_attackTimer.restart();
				}
				break;
			default:
				break;
			}
		}
		break;
	case aRight:
		if (!m_Gear.slots[2].Items.empty())
		{
			switch (m_Gear.slots[2].Items[0].item)
			{
			case Items::Bow:
				if (m_inventory.contains(Items::Arrow) && m_attackTimer.getElapsedTime().asSeconds() > GetSpeed(Items::Bow))
				{
					m_inventory.RemoveItem(Items::Arrow, 1);
					float angle = std::atan2f(sf::Mouse::getPosition(window).y - 720/2, sf::Mouse::getPosition(window).x - 1280/2);
					sf::Sprite arrow_sprite;
					arrow_sprite.setTexture(*p_texture_holder->getTexture(Textures::d_Arrow));
					projectile::Arrow arrow = projectile::Arrow(ptr_tiles, angle, arrow_sprite, GetDamage(Items::Bow), Items::Arrow, p_projectile_manager->m_particleSystem);
					arrow.setPosition(getPosition());
					p_projectile_manager->m_arrows.push_back(arrow);
					m_attackTimer.restart();
				}
				break;
			case Items::TestSpell:
				if (m_attackTimer.getElapsedTime().asSeconds() > GetSpeed(Items::TestSpell))
				{
					if (m_inventory.contains(Items::TestSpell))
					{
						m_inventory.RemoveItem(Items::TestSpell, 1);
					}else
					{
						if (!m_Gear.slots[2].Items.empty() && m_Gear.slots[2].Items[0].item == Items::TestSpell)
						{
							m_Gear.slots[2].Items.pop_back();
						}else
						{
							break;
						}
					}
					float angle = std::atan2f(sf::Mouse::getPosition(window).y - 720/2, sf::Mouse::getPosition(window).x - 1280/2);
					sf::Sprite spell_sprite;
					spell_sprite.setTexture(*p_texture_holder->getTexture(Textures::Armor_Chaos));
					p_projectile_manager->m_spells.push_back(projectile::Spell(getPosition(), angle, Items::TestSpell, spell_sprite, GetDamage(Items::TestSpell), ptr_tiles, Light(sf::Color(math::random(1, 255), math::random(1, 255), math::random(1, 255), 255), sf::Vector3f(getPosition().x, getPosition().y, 0.075f), sf::Vector3f(0.f, 5.f, 0.f), false)));
					m_attackTimer.restart();
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}

void Player::handleEvents(sf::Event const& event)
{
	if (event.type == event.KeyPressed && event.key.code == sf::Keyboard::Tab)
	{
		tabClicked = true;
	}
	if (event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		leftMouseClicked  = true;
	}
	if (event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
	{
		rightMouseClicked  = true;
	}
	if (event.type == sf::Event::MouseWheelMoved)
	{
		mouseWheelDelta = event.mouseWheel.delta;
	}
}

void Player::resetInputs(){
	mouseWheelDelta = 0;
	rightMouseClicked = false;
	leftMouseClicked = false;
	tabClicked = false;
}

void D_Gear::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	states.transform=getTransform();
	target.draw(m_sprite, states);
}