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
#include "Properties\Room.h"
#include "Properties\MapManager.h"
#include "Properties\HealingWell.h"
#include "Properties\MobSpawner.h"
#include "Properties\Cell.h"
#include "Properties\Torch.h"
#include "Properties\Gate.h"

#include <SFML\System\Mutex.hpp>

namespace gen{
	class Map : public sf::Drawable
	{
	public:
		Map(TextureHolder* textureHolder, FontHolder* fontHolder, MobManager* mobManager, LightManager* ptr_light_manager, sf::Mutex& mutex);
		~Map(void);

		void Gen();

		void setView(sf::View const& view);

		std::vector<Torch> m_torches;

		std::vector<Room> rooms;

		std::vector<MobSpawner> mobSpawners;

		std::vector<HealingWell> wells;

		std::vector<std::vector<Tile>> tiles;

		bool isPathable(int x, int y) const;

		void SetBounds();
		void update(sf::Time dt);
		void draw_mini_map(sf::RenderWindow* ptr_window, int x, int y);
		void draw_walls(sf::RenderTarget* target, sf::RenderStates states);

		///<summary>Map size.</summary>
		sf::Vector2i size;

		bool loaded;    // If false the map will be regenerated
		bool generating;
		gen::Type type; // This needs to be set to the right map before Gen() get's called

	private:
		void Path(Room* from, Room* to);

		void Prison();
		void Hell();
		void Cave();

		bool surroundingIsFloor(Tile const& tile);

		MobManager* mobManagerPtr;
		LightManager* ptr_light_manager;
		TextureHolder* ptr_texture_holder;

		sf::Mutex& m_mutex;

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
		sf::Texture tileset;
		sf::Texture m_mini_tileset;
		sf::View mView;

		sf::Rect<int> bounds;
		void draw(sf::RenderTarget& target, sf::RenderStates states)const;

		sf::Sprite m_mini_map_sprite;
		sf::RectangleShape m_mini_map_player;

		///<summary> DIS IS ONLY FOR RENDER TO MAEK BEAUTIFUL </summary>
		std::vector<Tile> m_wall_tiles;
	};
}