#pragma once
#include <math.h>

struct Node{
public:
	Node();
	Node(int x, int y, int id);
	~Node();
	int x, y, id, G, H, F;
	Node* parent;
	bool closed, open;
private:
};