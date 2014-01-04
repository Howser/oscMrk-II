#include "PathFinder\Node.h"

Node::Node()
	: F(0), G(0), H(0), x(0), y(0), closed(false), open(false){
		parent = nullptr;
}

Node::~Node(){

}