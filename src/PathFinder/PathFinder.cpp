#include "PathFinder\PathFinder.h"
#include <math.h>

PathFinder::PathFinder(){}

PathFinder::PathFinder(std::vector<std::vector<gen::Tile>>* tilePtr, sf::Vector2i const& mapSize)
	: currentNode(-1), mapSize(mapSize)
{
	GetMap(tilePtr);
}

PathFinder::~PathFinder(){

}

void PathFinder::GetMap(std::vector<std::vector<gen::Tile>>* map){
	nodes.clear();
	Node node;
	for (int y = 0; y < mapSize.y; y++)
	{
		for (int x = 0; x < mapSize.x; x++)
		{
			node.x = x;
			node.y = y;
			node.id = (*map)[x][y].type;
 			nodes.push_back(node);
		}
	}
}

void PathFinder::CalculateHGF(Node* node, Node* destination){
	node->F = 0;
	node->H = 0;
	if (node->parent != nullptr)
	{
		if (node->y - node->parent->y == 0 || node->x - node->parent->x == 0)
		{
			node->G = 10 + node->parent->G;
		}else
		{
			node->G = 14 + node->parent->G;
		}
	}else
	{
		if (node->y - destination->y == 0 || node->x - destination->x == 0)
		{
			node->G = 10;
		}else
		{
			node->G = 14;
		}
	}
	node->H = (int)std::sqrt(pow((destination->x - node->x), 2) + pow((destination->y - node->y), 2));
	node->F = node->G + node->H;
}

int PathFinder::GetLowestFOpen(){
	unsigned int low = open[0];
	for (unsigned int i = 0; i < open.size(); i++)
	{
		if (nodes[open[i]].F < nodes[low].F)
		{
			low = open[i];
		}
	}
	return low;
}

int PathFinder::GetIndexOf(Node* node){
	return node->x + (node->y*mapSize.x);
}

int PathFinder::GetIndexOf(sf::Vector2i* node){
	return node->x + (node->y * mapSize.x);
}

int PathFinder::GetIndexOf(const sf::Vector2i& node){
	return node.x + (node.y * mapSize.x);
}

int PathFinder::GetIndexOf(std::vector<int> & list, Node* node){
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (GetIndexOf(node) == list[i])
		{
			return i;
		}
	}
	return -1;
}

void PathFinder::Clear()
{
	nodes.clear();
	open.clear();
	closed.clear();
}

std::vector<sf::Vector2i> PathFinder::GetPath(sf::Vector2i start, sf::Vector2i destination, bool tiles){
	if (start.x > 1 && start.x < mapSize.x - 1 && start.y > 1 && start.y < mapSize.y - 1 && destination.x > 1 && destination.x < mapSize.x - 1 && destination.y > 1 && destination.y < mapSize.y - 1 && (nodes[destination.x + destination.y * mapSize.x].id == 1 || nodes[destination.x + destination.y * mapSize.x].id == 3) && nodes[start.x + start.y * mapSize.x].id == 1)
	{
		path.clear();
		for (unsigned int i = 0; i < open.size(); i++)
		{
			nodes[open[i]].open = false;
			nodes[open[i]].parent = nullptr;
		}
		for (unsigned int i = 0; i < closed.size(); i++)
		{
			nodes[closed[i]].closed = false;
			nodes[closed[i]].parent = nullptr;
		}
		open.clear();
		closed.clear();
		currentNode = start.x + (start.y*mapSize.x);	
		nodes[currentNode].open = true;
		nodes[currentNode].closed = false;
		open.push_back(currentNode);

		while (!nodes[GetIndexOf(&destination)].closed && !nodes[GetIndexOf(&destination)].open)
		{
			currentNode = GetLowestFOpen();
			nodes[currentNode].open = false;
			nodes[currentNode].closed = true;
			closed.push_back(currentNode);
			open.erase(open.begin() + GetIndexOf(open, &nodes[currentNode]));

			if (currentNode < 0 || currentNode > mapSize.x* mapSize.y)
				break;

			if (nodes[currentNode - 1].id == 1 && !nodes[currentNode - 1].closed)
			{
				if (nodes[currentNode - 1].open)
				{
					if (nodes[currentNode - 1].G - nodes[currentNode].G > nodes[currentNode].G)
					{
						nodes[currentNode - 1].parent = &nodes[currentNode];
					}
				}else
				{
					nodes[currentNode - 1].open = true;
					open.push_back(currentNode - 1);
					nodes[currentNode - 1].parent = &nodes[currentNode];
				}
				CalculateHGF(&nodes[currentNode - 1], &nodes[currentNode]);
			}
			if (nodes[currentNode + 1].id == 1 && !nodes[currentNode + 1].closed)
			{
				if (nodes[currentNode + 1].open)
				{
					if (nodes[currentNode + 1].G - nodes[currentNode].G > nodes[currentNode].G)
					{
						nodes[currentNode + 1].parent = &nodes[currentNode];
					}
				}else
				{
					nodes[currentNode + 1].open = true;
					open.push_back(currentNode + 1);
					nodes[currentNode + 1].parent = &nodes[currentNode];
				}
				CalculateHGF(&nodes[currentNode + 1], &nodes[currentNode]);
			}
			if (nodes[currentNode - mapSize.x].id == 1 && !nodes[currentNode - mapSize.x].closed)
			{
				if (nodes[currentNode - mapSize.x].open)
				{
					if (nodes[currentNode - mapSize.x].G - nodes[currentNode].G > nodes[currentNode].G)
					{
						nodes[currentNode - mapSize.x].parent = &nodes[currentNode];
					}
				}else
				{
					nodes[currentNode - mapSize.x].open = true;
					open.push_back(currentNode - mapSize.x);
					nodes[currentNode - mapSize.x].parent = &nodes[currentNode];
				}
				CalculateHGF(&nodes[currentNode - mapSize.x], &nodes[currentNode]);
			}
			if (nodes[currentNode + mapSize.x].id == 1 && !nodes[currentNode + mapSize.x].closed)
			{
				if (nodes[currentNode + mapSize.x].open)
				{
					if (nodes[currentNode + mapSize.x].G - nodes[currentNode].G > nodes[currentNode].G)
					{
						nodes[currentNode + mapSize.x].parent = &nodes[currentNode];
					}
				}else
				{
					nodes[currentNode + mapSize.x].open = true;
					open.push_back(currentNode + mapSize.x);
					nodes[currentNode + mapSize.x].parent = &nodes[currentNode];
				}
				CalculateHGF(&nodes[currentNode + mapSize.x], &nodes[currentNode]);
			}
			if (nodes[currentNode - 1 - mapSize.x].id == 1 && !nodes[currentNode - 1 - mapSize.x].closed && nodes[currentNode - mapSize.x].id == 1 && nodes[currentNode - 1].id == 1)
			{
				if (nodes[currentNode - 1 - mapSize.x].open)
				{
					if (nodes[currentNode - 1 - mapSize.x].G - nodes[currentNode].G > nodes[currentNode].G)
					{
						nodes[currentNode - 1 - mapSize.x].parent = &nodes[currentNode];
					}
				}else
				{
					nodes[currentNode - 1 - mapSize.x].open = true;
					open.push_back(currentNode - 1 - mapSize.x);
					nodes[currentNode - 1 - mapSize.x].parent = &nodes[currentNode];
				}
				CalculateHGF(&nodes[currentNode - 1 - mapSize.x], &nodes[currentNode]);
			}
			if (nodes[currentNode + 1 - mapSize.x].id == 1 && !nodes[currentNode + 1 - mapSize.x].closed && nodes[currentNode - mapSize.x].id == 1 && nodes[currentNode + 1].id == 1)
			{
				if (nodes[currentNode + 1 - mapSize.x].open)
				{
					if (nodes[currentNode + 1 - mapSize.x].G - nodes[currentNode].G > nodes[currentNode].G)
					{
						nodes[currentNode + 1 - mapSize.x].parent = &nodes[currentNode];
					}
				}else
				{
					nodes[currentNode + 1 - mapSize.x].open = true;
					open.push_back(currentNode + 1 - mapSize.x);
					nodes[currentNode + 1 - mapSize.x].parent = &nodes[currentNode];
				}
				CalculateHGF(&nodes[currentNode + 1 - mapSize.x], &nodes[currentNode]);
			}
			if (nodes[currentNode - 1 + mapSize.x].id == 1 && !nodes[currentNode - 1 + mapSize.x].closed && nodes[currentNode + mapSize.x].id == 1 && nodes[currentNode - 1].id == 1)
			{
				if (nodes[currentNode - 1 + mapSize.x].open)
				{
					if (nodes[currentNode - 1 + mapSize.x].G - nodes[currentNode].G > nodes[currentNode].G)
					{
						nodes[currentNode - 1 + mapSize.x].parent = &nodes[currentNode];
					}
				}else
				{
					nodes[currentNode - 1 + mapSize.x].open = true;
					open.push_back(currentNode - 1 + mapSize.x);
					nodes[currentNode - 1 + mapSize.x].parent = &nodes[currentNode];
				}
				CalculateHGF(&nodes[currentNode - 1 + mapSize.x], &nodes[currentNode]);
			}
			if (nodes[currentNode + 1 + mapSize.x].id == 1 && !nodes[currentNode + 1 + mapSize.x].closed&& nodes[currentNode + mapSize.x].id == 1 && nodes[currentNode + 1].id == 1)
			{
				if (nodes[currentNode + 1 + mapSize.x].open)
				{
					if (nodes[currentNode + 1 + mapSize.x].G - nodes[currentNode].G > nodes[currentNode].G)
					{
						nodes[currentNode + 1 + mapSize.x].parent = &nodes[currentNode];
					}
				}else
				{
					nodes[currentNode + 1 + mapSize.x].open = true;
					open.push_back(currentNode + 1 + mapSize.x);
					nodes[currentNode + 1 + mapSize.x].parent = &nodes[currentNode];
				}
				CalculateHGF(&nodes[currentNode + 1 + mapSize.x], &nodes[currentNode]);
			}
		}
		SetPath(destination, tiles);
	}
	return path;
}

void PathFinder::SetPath(const sf::Vector2i & destination, bool tiles){
	Node current = nodes[GetIndexOf(destination)];
	path.push_back(sf::Vector2i(current.x, current.y));
	while (current.parent != nullptr)
	{
		if (tiles)
		{
			path.push_back(sf::Vector2i(current.parent->x, current.parent->y));
		}else
		{
			path.push_back(sf::Vector2i(current.parent->x*WIDTH + WIDTH/2, current.parent->y*HEIGHT + HEIGHT/2));
		}
		current = *current.parent;
	}
}