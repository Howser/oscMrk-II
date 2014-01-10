#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
#include <iostream>
#include <math.h>


#include "ResourceHolders\TextureHolder.h"
#include "ResourceHolders\FontHolder.h"
#include "ResourceHolders\ResourceIdentifiers.h"
#include "Math\GeneralMath.h"
#include "Tile.h"
#include "Room.h"
#include "MapManager.h"
#include "HealingWell.h"
#include "MobSpawner.h"
#include "Cell.h"

namespace gen{
	class Map : public sf::Drawable
	{
	public:
		Map(TextureHolder* textureHolder, FontHolder* fontHolder, MobManager* mobManager);
		~Map(void);

		void Gen();

		void setView(sf::View const& view);

		std::vector<Room> rooms;

		std::vector<MobSpawner> mobSpawners;

		std::vector<HealingWell> wells;

		std::vector<std::vector<Tile>> tiles;

		bool isPathable(int x, int y) const;

		void SetBounds();
		void update(sf::Time dt);
		void draw_mini_map(sf::RenderWindow* ptr_window, int x, int y, const sf::Vector2f & playerPosition);

		///<summary>Map size.</summary>
		sf::Vector2i size;

	private:
		//temp for drawing items
		sf::RectangleShape rect;

		Type type;

		void Path(Room* from, Room* to);

		void Prison();
		void Hell();
		void Cave();

		bool surroundingIsFloor(Tile const& tile);

		MobManager* mobManagerPtr;

		void PickWallTile(Room* room);
		bool CanRoom(Room* room);
		bool CanRoom(int x, int y, short width, short height);
		void CreateRoom(Room* room, bool add, bool cave);
		void CreateRoom(int x, int y, short width, short height, bool add, bool cave);
		void ApplyRoom(Room* room, bool* add, bool* cave);

		/// selected wall tile
		int wallX, wallY, side, originRoomIndex;
		int GetLowX(), lowX;
		int GetLowY(), lowY;
		int GetHighX(), highX;
		int GetHighY(), highY;

		///<summary>Removes useless tiles.</summary>
		void Resize();
		void ApplyID();

		sf::Font font;
		sf::Texture		tileset;
		sf::Texture m_mini_tileset;
		sf::View		mView;

		sf::Rect<int> bounds;
		void draw(sf::RenderTarget& target, sf::RenderStates states)const;
	};
}