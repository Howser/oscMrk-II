#include "StateStack\InventoryState.h"
#include <SFML\Graphics\RenderWindow.hpp>

InventoryState::InventoryState(StateStack& stateStack, Context context, States::ID id)
	:
	State(stateStack, context, id),
	mSprite(),
	inventory(),//player inventory
	GearState(*context.textures, inventory->width*inventory->slotSprite.getLocalBounds().width + inventory->width*5)
{
	mSprite.setTexture(*getContext().textures->getTexture(Textures::Inventory));
	setPersistent(true);
	/*inventory.slots[0][0].Items.push_back(ArmorItem(Items::TestArmor, *getContext().textures));
	inventory.slots[8][0].Items.push_back(ArmorItem(Items::TestArmorC, *getContext().textures));
	inventory.slots[1][0].Items.push_back(ArmorItem(Items::TestArmorH, *getContext().textures));*/
	GearState.slots[0].slot = eGearSlot::Helmet;
	GearState.slots[1].slot = eGearSlot::Chestpiece;
	GearState.slots[2].slot = eGearSlot::Leggings;
	GearState.slots[3].slot = eGearSlot::lHand;
	GearState.slots[4].slot = eGearSlot::rHand;
}

InventoryState::~InventoryState()
{
}

bool InventoryState::update(sf::Time dt)
{
	/*mouseSlot.x = sf::Mouse::getPosition(*getContext().window).x;
	mouseSlot.y = sf::Mouse::getPosition(*getContext().window).y;
	if (leftMousePressed)
	{
		for (unsigned int x = 0, y = 0; x < inventory->width; x++)
		{
			for (y = 0; y < inventory->height; y++)
			{
				if (sf::Rect<int>(mouseSlot.x, mouseSlot.y, 1, 1).intersects(sf::Rect<int>(inventory->slots[x][y].getPosition().x, inventory->slots[x][y].getPosition().y, SLOTWIDTH, SLOTHEIGHT)))
				{
					inv::Command command = inv::Command::Move;
					inventory->Command(&inventory->slots[x][y], &mouseSlot, command);
				}
			}
		}
	}
	if (rightMousePressed)
	{
		for (unsigned int x = 0, y = 0; x < inventory->width; x++)
		{
			for (y = 0; y < inventory->height; y++)
			{
				if (sf::Rect<int>(mouseSlot.x, mouseSlot.y, 1, 1).intersects(sf::Rect<int>(inventory->slots[x][y].getPosition().x, inventory->slots[x][y].getPosition().y, SLOTWIDTH, SLOTHEIGHT)))
				{
					inv::Command command = inv::Command::Use;
					if (inventory->slots[x][y].Items.size() > 0)
					{
						if (GetType(inventory->slots[x][y].Items[0].item) == itemType::Armor)
						{
							GearItem item = GearItem(inventory->slots[x][y].Items[0].item, *getContext().textures);
							switch (item.eGearSlot)
							{
							case Helmet:
								item.use(&GearState.slots[0], &inventory->slots[x][y], inventory);
								break;
							case Chestpiece:
								item.use(&GearState.slots[1], &inventory->slots[x][y], inventory);
								break;
							case Leggings:
								item.use(&GearState.slots[2], &inventory->slots[x][y], inventory);
								break;
								case lHand:
								item.use(&GearState.slots[3], &inventory->slots[x][y], inventory);
								break;
								case rHand:
								item.use(&GearState.slots[4], &inventory->slots[x][y], inventory);
								break;
							}
						}
						else
						{
							inventory->Command(&inventory->slots[x][y], &mouseSlot, command);
						}
					}
				}
			}
		}
		for (unsigned int i = 0; i < GearState.slots.size(); i++)
		{
			if (sf::Rect<int>(mouseSlot.x, mouseSlot.y, 1, 1).intersects(sf::Rect<int>(GearState.slots[i].getPosition().x, GearState.slots[i].getPosition().y, SLOTWIDTH, SLOTHEIGHT)))
			{
				inv::Command command = inv::Command::Use;
				GearState.Command(&GearState.slots[i], inventory);
			}
		}
	}
	rightMousePressed = false;
	leftMousePressed = false;*/
	return false;
}

void InventoryState::draw()
{
	sf::RenderWindow* window = getContext().window;
	window->setView(window->getDefaultView());
	window->draw(*inventory);
	window->draw(GearState);
	if (mouseSlot.Items.size() > 0)
	{
		sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(26, 26));
		rect.setPosition(mouseSlot.getPosition().x - 13, mouseSlot.getPosition().y - 13);
		rect.setFillColor(sf::Color::Magenta);
		window->draw(rect);
	}
}

bool InventoryState::handleEvents(sf::Event const& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Tab)
		{
			requestStackPop();
		}
	}if (event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		leftMousePressed = true;
	}
	if (event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
	{
		rightMousePressed = true;
	}
	return false;
}

States::ID InventoryState::getID() const
{
	return States::Inventory;
}