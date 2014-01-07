#include "Player\Player.h"

#include "Math\VectorMath.h"

#include <SFML\Graphics\RenderTarget.hpp>

#include <iostream>

Player::Player(TextureHolder* textures, std::vector<Mob*>* mobsPtr)
	:
	p_texture_holder(textures),
	p_mobs(mobsPtr),
	m_sprite(),
	m_velocity(),
	m_path(),
	m_animation(sf::Vector2i(32, 32), 1),
	m_inventory(10, 10, 0, *textures),
	m_lootInventory(10, 10, 10*SLOTWIDTH + 10*5 + 100, *textures),
	m_Gear(*textures, m_inventory.width*SLOTWIDTH + SLOTWIDTH),
	inventoryState(false),
	lootState(false),
	tabClicked(false),
	leftMouseClicked(false),
	m_stackManager(*textures),
	m_tooltip(*textures),
	m_deleteItem(*textures),
	m_invRect(sf::Rect<int>(0, 0, m_inventory.width*SLOTWIDTH + m_inventory.width*5, m_inventory.height*SLOTHEIGHT + m_inventory.height*5)),
	m_lootInvRect(sf::Rect<int>(m_lootInventory.slots[0][0].getPosition().x, m_lootInventory.slots[0][0].getPosition().y, m_lootInventory.width*SLOTWIDTH + m_lootInventory.width*5, m_lootInventory.height*SLOTHEIGHT + m_lootInventory.width*5))
{
	m_sprite.setTexture(*(textures->getTexture(Textures::Player)));
	setOrigin(16, 16);
	m_inventory.slots[0][0].Items.push_back(GearItem(Items::TestWeapon, *textures, -1));
	Player::m_BootsSlot = Items::NOITEM;
	Player::m_ChestpieceSlot = Items::NOITEM;
	Player::m_HelmetSlot = Items::NOITEM;
	Player::m_lHandSlot = Items::NOITEM;
	Player::m_rHandSlot = Items::NOITEM;
	m_Gear.slots[0].slot = eGearSlot::Helmet;
	m_Gear.slots[1].slot = eGearSlot::Chestpiece;
	m_Gear.slots[2].slot = eGearSlot::Leggings;
	m_Gear.slots[3].slot = eGearSlot::lHand;
	m_Gear.slots[4].slot = eGearSlot::rHand;

	for (int i = 0; i < 5; i++)
	{
		m_d_gear.push_back(D_Gear());
		m_d_gear.back().setPosition(getPosition());
	}
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

void Player::update(sf::Time dt, sf::RenderWindow const& window)
{
	if (tabClicked)
	{
		inventoryState = true;
	}
	if (!inventoryState)
	{
		if (!m_path.empty())
		{
			// Check the distance to the next node 
			float dist = vec::length(getPosition() - m_path.back());
			if (dist < 10.f)
			{
				// remove it if the player is close enough
				m_path.pop_back();
			}

			if (!m_path.empty())
			{
				// Move towards the next node
				sf::Vector2f target = m_path.back() - getPosition();
				target = vec::normalize(target);
				m_velocity.x = target.x * TERMINAL_VELOCITY * dt.asSeconds();
				m_velocity.y = target.y * TERMINAL_VELOCITY * dt.asSeconds();
			}
		}
		else
		{
			stop();
		}

		// If the player is moving his rotation might be changed
		if (m_velocity.x != 0 && m_velocity.y != 0)
		{
			// Get the angle of the movement
			float angle = vec::angle(m_velocity);

			// set the animation (offset angle by 23 to make it not start at 0) 
			int animation = (angle+23)/45;

			// Constrain the animation
			if (animation > 7)
				animation = 7;
			if (animation < 0)
				animation = 0;

			m_animation.loop(animation);
		}

		m_animation.update();
		m_sprite.setTextureRect(m_animation.getFrame());
		move(m_velocity);
		for (int i = 0; i < 5; i++)
		{
			m_d_gear[i].setPosition(getPosition());
		}
	}
	resetInputs();
}

void Player::updateInventory(sf::RenderWindow const& window, TextureHolder & textures){
	if (tabClicked)
	{
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
		if (sf::Rect<int>(m_mouseSlot.getPosition().x, m_mouseSlot.getPosition().y, 1, 1).intersects(m_invRect) || (lootState && sf::Rect<int>(m_mouseSlot.getPosition().x, m_mouseSlot.getPosition().y, 1, 1).intersects(m_lootInvRect)))
		{
			bool broken = false;
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
									m_d_gear[0].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(13 + m_Gear.slots[0].Items[0].item))));
									m_d_gear[0].m_sprite.setPosition(getPosition().x + m_sprite.getTextureRect().width/2 - m_d_gear[0].m_sprite.getTextureRect().width/2, getPosition().y);
									break;
								case Chestpiece:
									item.Equip(&m_Gear.slots[1], &m_inventory.slots[x][y], &m_inventory);
									m_d_gear[1].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(13 + m_Gear.slots[1].Items[0].item))));
									m_d_gear[1].m_sprite.setPosition(getPosition().x + m_sprite.getTextureRect().width/2 - m_d_gear[1].m_sprite.getTextureRect().width/2, getPosition().y + 6);
									break;
								case Leggings:
									item.Equip(&m_Gear.slots[2], &m_inventory.slots[x][y], &m_inventory);
									m_d_gear[2].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(13 + m_Gear.slots[2].Items[0].item))));
									m_d_gear[2].m_sprite.setPosition(getPosition().x + m_sprite.getTextureRect().width/2 - m_d_gear[2].m_sprite.getTextureRect().width/2, getPosition().y);
									break;
								case lHand:
									item.Equip(&m_Gear.slots[3], &m_inventory.slots[x][y], &m_inventory);
									m_d_gear[3].m_sprite.setTexture(*(p_texture_holder->getTexture(Textures::d_TestWeapon)));
									m_d_gear[3].setPosition(getPosition().x, getPosition().y);
									break;
								case rHand:
									item.Equip(&m_Gear.slots[4], &m_inventory.slots[x][y], &m_inventory);
									m_d_gear[4].m_sprite.setTexture(*(p_texture_holder->getTexture((Textures::ID)(13 + m_Gear.slots[4].Items[0].item))));
									m_d_gear[4].m_sprite.setPosition(getPosition().x + m_sprite.getTextureRect().width/2 - m_d_gear[4].m_sprite.getTextureRect().width/2, getPosition().y);
									break;
								}
							}
							else
							{
								m_inventory.Command(&m_inventory.slots[x][y], &m_mouseSlot, inv::Use, &m_stackManager);
							}
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
			m_d_gear[i].draw(target, states);
		}
	}
}

void Player::drawGUI(sf::RenderWindow* p_window){
	if (inventoryState)
	{
		p_window->draw(m_inventory);
		p_window->draw(m_Gear);
		if (lootState)
		{
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