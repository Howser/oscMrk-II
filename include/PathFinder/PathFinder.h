#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Node.h"
#include "Map\Tile.h"
#include "Map\Properties\MapManager.h"

class PathFinder{
public:
	PathFinder();
	PathFinder(std::vector<std::vector<gen::Tile>>* tilePtr, sf::Vector2i const& mapSize);
        ~PathFinder();

        std::vector<sf::Vector2i> GetPath(const sf::Vector2i & start, const sf::Vector2i & destination, bool tiles);

		void GetMap(std::vector<std::vector<gen::Tile>>* p_tile);

		void Clear();

        std::vector<sf::Vector2i> path;

		sf::Vector2i mapSize;
private:
        std::vector<Node> nodes;

		std::vector<int> open, closed;

        void CalculateHGF(Node* node, Node* destination);

        void SetPath(const sf::Vector2i& destination, bool tiles);

        int currentNode;

        int GetLowestFOpen();
		int GetIndexOf(Node* node);
		int GetIndexOf(sf::Vector2i* node);
		int GetIndexOf(const sf::Vector2i& node);
		int GetIndexOf(std::vector<int> & list, Node* node);
		void Clear(std::vector<int>* list);

		sf::Vector2<int> destination;
};