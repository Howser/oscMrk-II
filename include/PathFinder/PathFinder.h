#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Node.h"
#include "Map\Tile.h"
#include "Map\MapManager.h"

class PathFinder{
public:
	PathFinder();
	PathFinder(std::vector<std::vector<gen::Tile>>* tilePtr, sf::Vector2i const& mapSize);
        ~PathFinder();

        std::vector<sf::Vector2i> GetPath(sf::Vector2i start, sf::Vector2i destination);

		void GetMap(std::vector<std::vector<gen::Tile>>* p_tile);

        std::vector<sf::Vector2i> path;

		sf::Vector2i mapSize;
private:
        std::vector<Node> nodes;

		std::vector<int> open, closed;

        void CalculateHGF(Node* node, Node* destination);

        void SetPath(sf::Vector2i* start, sf::Vector2i* destination);

        int currentNode;

        int GetLowestFOpen();
		int GetIndexOf(Node* node);
		int GetIndexOf(sf::Vector2i* node);
		int GetIndexOf(std::vector<int> & list, Node* node);
		void Clear(std::vector<int>* list);
};