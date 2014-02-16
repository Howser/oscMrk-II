#include "Map\Map.h"
#include <stdexcept>

gen::Map::Map(TextureHolder* textureHolder, FontHolder* fontHolder, MobManager* mobManager, LightManager* ptr_light_manager, sf::Mutex& mutex)
	:
	font(*fontHolder->getFont(Fonts::Main)),
	tileset(*textureHolder->getTexture(Textures::Cave_Sheet)),
	m_mini_tileset(*textureHolder->getTexture(Textures::Mini_Map_sheet)),
	mobManagerPtr(mobManager),
	ptr_texture_holder(textureHolder),
	ptr_light_manager(ptr_light_manager),
	loaded(false),
	generating(false),
	m_mutex(mutex),
	m_wall_tiles()
{
	m_mini_map_sprite.setTexture(m_mini_tileset);
	m_mini_map_sprite.setColor(sf::Color(255, 255, 255, 100));
	m_mini_map_player.setPosition(1280/16, 1280/16);
	m_mini_map_player.setSize(sf::Vector2f(4, 4));
	m_mini_map_player.setFillColor(sf::Color::Magenta);
	
}

gen::Map::~Map(void)
{
}

void gen::Map::Gen(){
	generating = true;
	tiles.clear();
	rooms.clear();
	tiles.resize(mapWidth, std::vector<Tile>(mapHeight, Tile()));


	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			tiles[x][y].x = x;
			tiles[x][y].y = y;
			tiles[x][y].type = 0;
		}
	}
	mobSpawners.clear();
	switch (type)
	{
	case gen::Prison:
		Prison();
		break;
	case gen::Cave:
		Cave();
		break;
	case gen::Hell:
		Hell();
		break;
	}

	ApplyID();
	loaded = true;
	generating = false;
}

bool gen::Map::surroundingIsFloor(Tile const& tile){
	if (tiles[tile.x + 1][tile.y].type == 1 || tiles[tile.x - 1][tile.y].type == 1 || tiles[tile.x][tile.y - 1].type == 1 || tiles[tile.x][tile.y + 1].type == 1)
	{
		return true;
	}
	return false;
}

void gen::Map::Prison(){
	int width = math::random(minRoomWidth*2, maxRoomWidth), height = math::random(minRoomWidth*2, maxRoomWidth);
	CreateRoom(10, 10, width, height, true, false);
	tiles[rooms[0].x + 1][rooms[0].y].type = 3;
	while (rooms.size() < 10)
	{
		int side = math::random(0, 1);
		if (side == 0)
		{
			int i = math::random(0, rooms.size() - 1);
			width = math::random(minRoomWidth*2, maxRoomWidth);
			height = math::random(minRoomWidth*2, maxRoomWidth);
			int dist = math::random(maxRoomWidth, maxRoomWidth*2);
			if (CanRoom(rooms[i].x + rooms[i].width + dist, rooms[i].y, width, height))
			{
				CreateRoom(rooms[i].x + rooms[i].width + dist, rooms[i].y, width, height, true, false);
				Path(&rooms[i], &rooms[rooms.size() - 1]);
				for (int x = rooms[i].x + rooms[i].width; x < rooms[i].x + rooms[i].width + dist; x++)
				{
					int j = math::random(1, 100);
					if (j > 50 && CanRoom(x, rooms[i].y - 2, 5, 4))
					{
						CreateRoom(x, rooms[i].y - 2, 4, 4, false, false);
						tiles[x + 2][rooms[i].y + 1].type = 1;
						tiles[x + 1][rooms[i].y + 1].type = 1;
						tiles[x + 2][rooms[i].y + 2].type = 1;
					}
					int l = math::random(1, 100);
					if (l > 50 && CanRoom(x, rooms[i].y + 6, 4, 4))
					{
						CreateRoom(x, rooms[i].y + 6, 4, 4, false, false);
						tiles[x + 2][rooms[i].y + 5].type = 1;
						tiles[x + 2][rooms[i].y + 6].type = 1;
						tiles[x + 1][rooms[i].y + 6].type = 1;
					}
				}
			}
		}else
		{
			int i = math::random(0, rooms.size() - 1);
			width = math::random(minRoomWidth*2, maxRoomWidth);
			height = math::random(minRoomWidth*2, maxRoomWidth);
			int dist = math::random(maxRoomWidth, maxRoomWidth*2);
			if (CanRoom(rooms[i].x, rooms[i].y + rooms[i].height + dist, width, height))
			{
				CreateRoom(rooms[i].x, rooms[i].y + rooms[i].height + dist, width, height, true, false);
				Path(&rooms[i], &rooms[rooms.size() - 1]);
				for (int y = rooms[i].y + height; y < rooms[i].y + rooms[i].height + dist; y++)
				{
					int j = math::random(1, 100);
					if (j > 50 && CanRoom(rooms[i].x - 3, y, 5, 4))
					{
						CreateRoom(rooms[i].x - 3, y, 5, 4, false, false);
						tiles[rooms[i].x + 1][y + 1].type = 1;
						tiles[rooms[i].x + 1][y + 2].type = 1;
						tiles[rooms[i].x + 2][y + 1].type = 1;
					}
					int l = math::random(1, 100);
					if (l > 50 && CanRoom(rooms[i].x + 6, y, 5, 4))
					{
						CreateRoom(rooms[i].x + 6, y, 5, 4, false, false);
						tiles[rooms[i].x + 6][y + 1].type = 1;
						tiles[rooms[i].x + 6][y + 2].type = 1;
						tiles[rooms[i].x + 5][y + 1].type = 1;
					}
				}
			}
		}
	}
}

void gen::Map::Path(Room* from, Room* to){
	if (std::sqrt(pow(from->x - to->x, 2)) > std::sqrt(pow(from->y - to->y, 2)))
	{
		for (int x = from->x + from->width - 2; (x < to->x + 1) ? x++: (x != to->x + 1) ? x--:x = -1;)
		{
			if (x == -1)
			{
				break;
			}
			if (tiles[x][to->y + 2].type != 1 && tiles[x][to->y + 3].type != 1 && tiles[x][to->y + 4].type != 1)
			{
				if (to->y + 5 < mapHeight && to->y > 0 && x < mapWidth && x > 0)
				{
					tiles[x][to->y + 2].type = 2;
					tiles[x][to->y + 3].type = 1;
					tiles[x][to->y + 4].type = 1;
					tiles[x][to->y + 5].type = 2;
				}
			}
		}
	}else
	{
		for (int y = from->y + from->height - 2; (y < to->y + 1) ? y++: (y != to->y + 1) ? y--:y = -1;)
		{
			if (y == -1)
			{
				break;
			}
			if (tiles[to->x + 2][y].type != 1 && tiles[to->x + 3][y].type != 1 && tiles[to->x + 4][y].type != 1)
			{
				tiles[to->x + 2][y].type = 2;
				tiles[to->x + 3][y].type = 1;
				tiles[to->x + 4][y].type = 1;
				tiles[to->x + 5][y].type = 2;
			}
		}
	}
}

void gen::Map::Cave(){
	std::vector<Cell> cells;
	Room room(mapWidth/2 - 6, mapHeight/2 - 6, 12, 12, true);
	ApplyRoom(&room, &room.cave, &room.cave);
	rooms.push_back(room);
	cells.push_back(Cell(mapWidth/2, mapHeight/2 - 7));
	cells.push_back(Cell(mapWidth/2, mapHeight/2 + 7));
	cells.push_back(Cell(mapWidth/2 - 7, mapHeight/2));
	cells.push_back(Cell(mapWidth/2 + 7, mapHeight/2));
	for (unsigned int a = 0; a < 75; a++)
	{
		for (unsigned int c = 0; c < cells.size(); c++)
		{
			if (cells[c].active)
			{
				if ((cells[c].x < mapWidth - 4 && cells[c].x > 4 && cells[c].y < mapHeight - 4 && cells[c].y > 4))
				{
					for (unsigned int i = 0; i < 2; i++)
					{
						if (tiles[cells[c].x - 1][cells[c].y].type != 1 || tiles[cells[c].x + 1][cells[c].y].type != 1 || tiles[cells[c].x][cells[c].y - 1].type != 1 || tiles[cells[c].x][cells[c].y + 1].type != 1)
						{
							sf::Vector2i p = cells[c].digpos();
							while (tiles[p.x][p.y].type != 0 || !surroundingIsFloor(tiles[p.x][p.y]))
							{
								p = cells[c].digpos();
							}
							tiles[p.x][p.y].type = 1;
							cells[c].x = p.x;
							cells[c].y = p.y;
							unsigned int s = math::random(1, 1000);
							if (s <= 130)
							{
								sf::Vector2f pos(cells[c].x, cells[c].y);
								if (mobSpawners.size() < 15 && s <= 25 && math::distance(pos, rooms[0].getCenter()) > 50)
								{
									MobSpawner spawner(p.x, p.y, math::random(3, 5), TYPE::spider, 0, TYPE::special);
									mobSpawners.push_back(spawner);
									/*Torch torch = Torch(Light(sf::Color(200, 100, 100, 255), sf::Vector3<float>(p.x*32-100, p.y*32-100, 0.075f), sf::Vector3<float>(0, 10.f, 0), false), ptr_light_manager, ptr_texture_holder);
									m_torches.push_back(torch); 
									m_torches.back().ptr_light = &ptr_light_manager->m_lights.back();*/
								}
							}
							if (s <= 65)
							{
								if (s <= 5)
								{
									HealingWell well(sf::Vector2f(p.x, p.y));
									wells.push_back(well);
								}
								cells.push_back(cells[c]);
							}
						}
						else
						{
							cells[c].active = false;
						}
					}
				}
			}
		}
	}
}

void gen::Map::Hell(){
	int width = math::random(minRoomWidth, maxRoomWidth), height = math::random(minRoomWidth, maxRoomWidth);
	CreateRoom(mapWidth/2 - width/2, mapHeight/2 - height/2, width, height, true, true);
	while (rooms.size() < ROOMS)
	{
		originRoomIndex = math::random(0, rooms.size() - 1);
		PickWallTile(&rooms[originRoomIndex]);
		int width = math::random(minRoomWidth, maxRoomWidth), height = math::random(minRoomWidth, maxRoomWidth);
		//up
		if (side == 0)
		{
			if (rooms.size() == ROOMS - 1)
			{
				CreateRoom(wallX - width/2, wallY - height, maxRoomWidth*2 + math::random(-5, -1), maxRoomWidth*2 + math::random(-5, -1), true, true);
			}else
			{
				CreateRoom(wallX - width/2, wallY - height, width, height, true, true);
			}
		}//right
		else if (side == 1)
		{
			if (rooms.size() == ROOMS - 1)
			{
				CreateRoom(wallX, wallY - height/2, maxRoomWidth*2 + math::random(-5, -1), maxRoomWidth*2 + math::random(-5, -1), true, true);
			}else
			{
				CreateRoom(wallX, wallY - height/2, width, height, true, true);
			}

		}//down
		else if (side == 2)
		{
			if (rooms.size() == ROOMS - 1)
			{
				CreateRoom(wallX - width/2, wallY, maxRoomWidth*2 + math::random(-5, -1), maxRoomWidth*2 + math::random(-5, -1), true, true);
			}
			else
			{
				CreateRoom(wallX - width/2, wallY, width, height, true, true);
			}
		}//left
		else if (side == 3)
		{
			if (rooms.size() == ROOMS - 1)
			{
				CreateRoom(wallX - width, wallY - height/2, maxRoomWidth*2 + math::random(-5, -1), maxRoomWidth*2 + math::random(-5, -1), true, true);
			}
			else
			{
				CreateRoom(wallX - width, wallY - height/2, width, height, true, true);
			}
		}
	}
}

void gen::Map::setView(sf::View const& view)
{
	mView = view;
}

void gen::Map::PickWallTile(Room* room){
	side = math::random(0, 3);
	//up
	if (side == 0)
	{
		wallX = math::random(room->x + 1, room->x + room->width - 2);
		wallY = room->y;
	}//right
	else if (side == 1)
	{
		wallX = room->x + room->width;
		wallY = math::random(room->y + 1, room->y + room->height - 2);
	}//down
	else if (side == 2)
	{   
		wallX = math::random(room->x + 1, room->x + room->width - 2);
		wallY = room->y + room->height;
	}//left
	else if (side == 3)
	{
		wallX = room->x;
		wallY = math::random(room->y + 1, room->y + room->height - 2);
	}
}

void gen::Map::CreateRoom(Room* room, bool add, bool cave){
	if (CanRoom(room))
	{
		ApplyRoom(room, &add, &cave);
		int i = math::random(1, 100);
		if (add)
		{
			rooms.push_back(*room);
			if (rooms.size() > 1 && i <= mobRoomChance + 10)
			{
				mobSpawners.push_back(MobSpawner(room->getCenter().x/WIDTH, room->getCenter().y/HEIGHT, room->radius/10, TYPE::skeleton, 0, TYPE::special));
			}
		}else
		{
			if (i <= mobRoomChance)
			{
				mobSpawners.push_back(MobSpawner(room->getCenter().x/WIDTH, room->getCenter().y/HEIGHT, 1, TYPE::skeleton, 0, TYPE::special));
			}
		}
	}else
	{
		originRoomIndex = math::random(0, rooms.size() - 1);
		PickWallTile(&rooms[originRoomIndex]);
	}
}

void gen::Map::CreateRoom(int x, int y, short width, short height, bool add, bool cave){
	Room room(x, y, width, height, cave);
	if (CanRoom(&room))
	{
		ApplyRoom(&room, &add, &cave);
		int i = math::random(1, 100);
		if (add)
		{
			rooms.push_back(room);
			if (rooms.size() > 1 && i <= mobRoomChance + 10)
			{
				mobSpawners.push_back(MobSpawner(room.getCenter().x/WIDTH, room.getCenter().y/HEIGHT, room.radius/10, TYPE::skeleton, 0, TYPE::special));
			}
		}else
		{
			if (i <= mobRoomChance)
			{
				mobSpawners.push_back(MobSpawner(room.getCenter().x/WIDTH, room.getCenter().y/HEIGHT, 1, TYPE::skeleton, 0, TYPE::special));
			}
		}
	}else if (type != Type::Prison)
	{
		originRoomIndex = math::random(0, rooms.size() - 1);
		PickWallTile(&rooms[originRoomIndex]);
	}
}

void gen::Map::ApplyRoom(Room* room, bool* add, bool* cave){
	int i = 0;
	for (unsigned int x = room->x, y = room->y; x < room->x + room->width; x++)
	{
		for (y = room->y; y < room->y + room->height; y++)
		{
			if (type == Type::Hell)
			{
				//random tiles
				/*if (rooms.size() < ROOMS - 1 && i < 5)
				{
					if (x == room->x + 1 || x == room->x + room->width - 2)
					{
						int chance = math::random(0, 20);
						if (chance >= 15)
						{
							tiles[x][y].type = 2;
							room->radius--;
							i++;
						}
					}
					if (y == room->y + 1 || y == room->y + room->height - 2)
					{
						int chance = math::random(0, 20);
						if (chance >= 15)
						{
							tiles[x][y].type = 2;
							room->radius--;
							i++;
						}
					}
				}*/
			}
			//walls and floors
			if (x == room->x || x == room->x + room->width - 1 || y == room->y || y == room->y + room->height - 1)
			{
				if (type != Type::Cave)
				{
					tiles[x][y].type = 2;
					room->radius--;
				}
			} if(tiles[x][y].type != 2 && tiles[x][y].type != 3)
				tiles[x][y].type = 1;
		}
	}

	//fix unpathable tiles
	for (unsigned int x = room->x, y = room->y; x < room->x + room->width; x++)
	{
		for (y = room->y; y < room->y + room->height; y++)
		{
			if (tiles[x][y].type == 1 && tiles[x + 1][y].type == 2 && tiles[x - 1][y].type == 2 && tiles[x][y + 1].type == 2 && tiles[x][y - 1].type == 2)
			{
				if (x < room->x + room->width/2)
				{
					if (y < room->y + room->height/2)
					{
						tiles[x + 1][y].type = 1;
					}else
					{
						tiles[x][y - 1].type = 1;
					}
				}else
				{
					if (y < room->y + room->height/2)
					{
						tiles[x - 1][y].type = 1;
					}else
					{
						tiles[x][y - 1].type = 1;
					}
				}
			}
		}
	}
	if (rooms.size() > 0 && rooms.size() < ROOMS - 1 && room->width*room->height >= 90)
	{
		MobSpawner spawner(room->getCenter().x, room->getCenter().y, room->width*room->height/10, TYPE::special, 0, TYPE::special);
		mobSpawners.push_back(spawner);
	}
	if (side == 0)
	{
		tiles[wallX][wallY + 2].type = 1;
		tiles[wallX][wallY + 1].type = 1;
		tiles[wallX][wallY].type = 1;
		tiles[wallX][wallY - 1].type = 1;
		tiles[wallX][wallY - 2].type = 1;
		tiles[wallX][wallY - 3].type = 1;
		if (rooms.size() == ROOMS - 1)
		{
			tiles[room->x + room->width/2][room->y + 2].type = 3;
		}
	}else if (side == 1)
	{
		tiles[wallX - 3][wallY].type = 1;
		tiles[wallX - 2][wallY].type = 1;
		tiles[wallX - 1][wallY].type = 1;
		tiles[wallX][wallY].type = 1;
		tiles[wallX + 1][wallY].type = 1;
		tiles[wallX + 2][wallY].type = 1;
		if (rooms.size() == ROOMS - 1)
		{
			tiles[room->x + room->width - 2][room->y + room->height/2].type = 3;
		}
	}else if (side == 2)
	{
		tiles[wallX][wallY + 2].type = 1;
		tiles[wallX][wallY + 1].type = 1;
		tiles[wallX][wallY].type = 1;
		tiles[wallX][wallY - 1].type = 1;
		tiles[wallX][wallY - 2].type = 1;
		tiles[wallX][wallY - 3].type = 1;
		if (rooms.size() == ROOMS - 1)
		{
			tiles[room->x + room->width/2][room->y + room->height - 2].type = 3;
		}
	}else if (side == 3)
	{
		tiles[wallX + 2][wallY].type = 1;
		tiles[wallX + 1][wallY].type = 1;
		tiles[wallX][wallY].type = 1;
		tiles[wallX - 1][wallY].type = 1;
		tiles[wallX - 2][wallY].type = 1;
		tiles[wallX - 3][wallY].type = 1;
		if (rooms.size() == ROOMS - 1)
		{
			tiles[room->x + room->width - 2][room->y + room->height/2].type = 3;
		}
	}
}

bool gen::Map::CanRoom(Room* room){
	if (room->x > 0 && room->x + room->width < mapWidth)
	{
		if (room->y > 0 && room->y + room->height < mapHeight)
		{
			for (unsigned int x = room->x, y = room->y; x < room->x + room->width; x++)
			{
				for (y = room->y; y < room->y + room->height; y++)
				{
					if (tiles[x][y].type != 0)
					{
						return false;
					}
				}
			}
			return true;
		}else
		{
			return false;
		}
	}else
	{
		return false;
	}
}

bool gen::Map::CanRoom(int x, int y, short width, short height){
	Room room(x, y, width, height, false);
	if (room.x > 0 && room.x + room.width < mapWidth)
	{
		if (room.y > 0 && room.y + room.height < mapHeight)
		{
			for (unsigned int x = room.x, y = room.y; x < room.x + room.width; x++)
			{
				for (y = room.y; y < room.y + room.height; y++)
				{
					if (tiles[x][y].type != 0)
					{
						return false;
					}
				}
			}
			return true;
		}else
		{
			return false;
		}
	}else
	{
		return false;
	}
}

int gen::Map::GetLowX(){
	int lowx = mapWidth;
	for (unsigned int x = 0, y = 0; x < lowx; x++)
	{
		for (y = 0; y < mapHeight; y++)
		{
			if (tiles[x][y].type != 0)
			{
				if (tiles[x][y].x < lowx)
				{
					lowx = x;
				}
			}
		}
	}
	return lowx;
}

int gen::Map::GetLowY(){
	int lowy = mapHeight;
	for (unsigned int x = 0, y = 0; x < mapWidth; x++)
	{
		for (y = 0; y < lowy; y++)
		{
			if (tiles[x][y].type != 0)
			{
				if (tiles[x][y].y < lowy)
				{
					lowy = y;
				}
			}
		}
	}
	return lowy;
}

int gen::Map::GetHighX(){
	int highx = 0;
	for (int x = mapWidth - 1, y = 0; x > highx; x--)
	{
		for (y = 0; y < mapHeight; y++)
		{
			if (tiles[x][y].type != 0)
			{
				if (tiles[x][y].x > highx)
				{
					highx = x;
				}
			}
		}
	}
	return highx + 1;
}

int gen::Map::GetHighY(){
	int highy = 0;
	for (int x = 0, y = 0; x < mapWidth; x++)
	{
		for (y = mapHeight - 1; y > highy; y--)
		{
			if (tiles[x][y].type != 0)
			{
				if (tiles[x][y].y > highy)
				{
					highy = y;
				}
			}
		}
	}
	return highy + 1;
}

void gen::Map::Resize(){
	lowX = GetLowX();
	highX = GetHighX();
	lowY = GetLowY();
	highY = GetHighY();
	size.x = highX - lowX;
	size.y = highY - lowY;
	std::vector<std::vector<Tile>> temp;

	try {
		temp.resize(size.x, std::vector<Tile>(size.y, Tile()));
	}
	catch (std::length_error e){
		std::cout << e.what() << std::endl;
	}

	for (unsigned int x = lowX, y = lowY; x < highX; x++)
	{
		for (y = lowY; y < highY; y++)
		{
			temp[x - lowX][y - lowY] = tiles[x][y];
			temp[x - lowX][y - lowY].x -= lowX;
			temp[x - lowX][y - lowY].y -= lowY;
			if (tiles[x][y].type == 2 || tiles[x][y].type == 3){
				m_wall_tiles.push_back(temp[x - lowX][y - lowY]);
			}
		}
	}

	if (rooms.size() > 0)
	{
		for (unsigned int i = 0; i < rooms.size(); i++)
		{
			rooms[i].x -= lowX;
			rooms[i].y -= lowY;
		}
	}
	if (mobSpawners.size() > 0)
	{
		for (unsigned int i = 0; i < mobSpawners.size(); i++)
		{ 
			mobSpawners[i].x -= lowX;
			mobSpawners[i].y -= lowY;
		}
	}
	if (wells.size() > 0)
	{
		for (unsigned int i = 0; i < wells.size(); i++)
		{
			wells[i].setPosition(wells[i].getPosition().x - lowX, wells[i].getPosition().y - lowY);
		}
	}
	tiles = temp;
}

void gen::Map::ApplyID(){
	if (type == Type::Cave)
	{
		for (unsigned int x = 1, y = 1; x < mapWidth - 1; x++)
		{
			for (y = 1; y < mapHeight - 1; y++)
			{
				if (tiles[x][y].type == 0)
				{
					int wall = math::random(1, 4);
					if (tiles[x - 1][y].type == 1)
					{
						for (int xx = x; xx < x + wall; xx++)
						{
							if (xx < 150)
							{
								if (tiles[xx][y].type != 1)
								{
									tiles[xx][y].type = 2;
								}else
								{
									break;
								}
							}else
							{
								break;
							}
						}
					}
					if (tiles[x + 1][y].type == 1)
					{
						for (int xx = x; xx > x - wall; xx--)
						{
							if (tiles[xx][y].type != 1)
							{
								tiles[xx][y].type = 2;
							}else
							{
								break;
							}
						}
					}
					if (tiles[x][y - 1].type == 1)
					{
						for (int yy = y; yy < wall + y; yy++)
						{
							if (yy < 150)
							{
								if (tiles[x][yy].type != 1)
								{
									tiles[x][yy].type = 2;
								}else
								{
									break;
								}
							}else
							{
								break;
							}
						}
					}
					if (tiles[x][y + 1].type == 1)
					{
						for (int yy = y; yy > y - wall; yy--)
						{
							if (tiles[x][yy].type != 1)
							{
								tiles[x][yy].type = 2;
							}else
							{
								break;
							}
						}
					}
					if (tiles[x - 1][y].type == 1 ||tiles[x + 1][y].type == 1 || tiles[x][y - 1].type == 1 || tiles[x][y + 1].type == 1 || tiles[x - 1][y - 1].type == 1 ||tiles[x + 1][y - 1].type == 1 || tiles[x - 1][y + 1].type == 1 || tiles[x + 1][y + 1].type == 1)
					{
						tiles[x][y].type = 2;
					}
				}
			}
		}
	}

	for (unsigned int x = 1, y = 1; x < mapWidth - 1; x++)
	{
		for (y = 1; y < mapHeight - 1; y++)
		{
			tiles[x][y].ID = 0;
			if (tiles[x][y].type == 2)
			{
				if (tiles[x][y-1].type == 2)
				{
					tiles[x][y].ID += 1;
				}
				if (tiles[x+1][y].type == 2)
				{
					tiles[x][y].ID += 2;
				}
				if (tiles[x][y+1].type == 2)
				{
					tiles[x][y].ID += 4;
				}
				if (tiles[x-1][y].type == 2)
				{
					tiles[x][y].ID += 8;
				}
			}else 
			{
				if(tiles[x][y].type == 1){
					tiles[x][y].ID = 16;
				}else if (tiles[x][y].type == 3){
					tiles[x][y].ID = 17;
				}
			}
		}
	}
	Resize();
}

void gen::Map::update(sf::Time dt){
	SetBounds();
	for (int x = bounds.left/WIDTH, y = bounds.top/HEIGHT; x < bounds.left/WIDTH + bounds.width/WIDTH + 1; x++)
	{
		for (y = bounds.top/HEIGHT; y < bounds.top/HEIGHT + bounds.height/HEIGHT + 2; y++)
		{
			if (x < size.x && x >= 0 && y < size.y && y >= 0)
			{
				tiles[x][y].m_explored = true;
			}
		}
	}
}

void gen::Map::SetBounds(){
	bounds = sf::Rect<int>((int)(mView.getCenter().x - mView.getSize().x / 2), (int)(mView.getCenter().y - mView.getSize().y / 2), (int)(mView.getCenter().x + mView.getSize().x / 2) - (int)(mView.getCenter().x - mView.getSize().x / 2), (int)(mView.getCenter().y + mView.getSize().y / 2) - (int)(mView.getCenter().y - mView.getSize().y / 2));
}

void gen::Map::draw(sf::RenderTarget& target, sf::RenderStates states)const{
	sf::Sprite sprite;
	if (states.texture == nullptr)
	{
		sprite.setTexture(tileset);
	}
	else
	{
		sprite.setTexture(*states.texture);
	}
	for (int x = bounds.left/WIDTH, y = bounds.top/HEIGHT; x < bounds.left/WIDTH + bounds.width/WIDTH + 1; x++)
	{
		for (y = bounds.top/HEIGHT; y < bounds.top/HEIGHT + bounds.height/HEIGHT + 2; y++)
		{
			if (x < size.x && x >= 0 && y < size.y && y >= 0)
			{
				if (tiles[x][y].type == 1)
				{
					sprite.setTextureRect(sf::Rect<int>(tiles[x][y].ID*WIDTH, 0*HEIGHT, WIDTH, HEIGHT));
					sprite.setPosition(x*WIDTH, y*HEIGHT);
					target.draw(sprite);
				}
			}
		}
	}
}

void gen::Map::draw_mini_map(sf::RenderWindow* ptr_window, int X, int Y){
	for (unsigned int x = (X/WIDTH - (1280/2)/WIDTH >= 0) ? X/WIDTH - (1280/2)/WIDTH:0, y = 0; x < X/WIDTH + (1280/2)/WIDTH; x++)
	{
		for (y = (Y/HEIGHT - 1280/HEIGHT >= 0) ? Y/HEIGHT - (1280/2)/HEIGHT:0; y < Y/HEIGHT + (1280/2)/HEIGHT; y++)
		{
			if (x > 0 && x < tiles.size() && y > 0 && y < tiles[0].size()) 
			{
				if (tiles[x][y].m_explored && tiles[x][y].type != 0)
				{
					m_mini_map_sprite.setTextureRect(sf::Rect<int>((tiles[x][y].type-1)*4, 0, 4, 4));
					m_mini_map_sprite.setPosition((x*4) - (X/WIDTH - (1280/2)/WIDTH)*4, (y*4) - (Y/WIDTH - (1280/2)/HEIGHT)*4);
					ptr_window->draw(m_mini_map_sprite);
				}
			}
		}
	}
	ptr_window->draw(m_mini_map_player);
}

void gen::Map::draw_walls(sf::RenderTarget* target, sf::RenderStates states)
{
	sf::Sprite sprite;
	if (states.texture == nullptr) {
		sprite.setTexture(tileset);
	} else {
		sprite.setTexture(*states.texture);
	}
	
	sf::Rect<int> window = sf::Rect<int>(target->getView().getCenter().x - 640, target->getView().getCenter().y - 360, 1280, 720);
	for (auto i = m_wall_tiles.begin(); i != m_wall_tiles.end(); ++i){
		if (sf::Rect<int>(i->x*WIDTH, i->y*HEIGHT, WIDTH, HEIGHT).intersects(window))
		{
			sprite.setPosition(i->x * WIDTH, i->y * HEIGHT);
			sprite.setTextureRect(sf::Rect<int>(i->ID * WIDTH, 0, WIDTH, HEIGHT));
			target->draw(sprite);
		}
	}
}

bool gen::Map::isPathable(int x, int y) const
{
	return (x > 0 && x < size.x && y > 0 && y < size.y && tiles[x][y].type == 1) ?  true:false;
}

void gen::Map::Clear(){
	mobSpawners.clear();
	m_wall_tiles.clear();
}