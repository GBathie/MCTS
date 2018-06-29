#include "Node.h"


Node::Node(Node* prev, int m) : move(m), wins(0), visits(0)
{
	parent = prev;
	for (int i = 0; i < children_size; ++i)
	{
		children[i] = nullptr;
	}
}

Node::Node(int m) : move(m), wins(0), visits(0)
{
	parent = nullptr;
	for (int i = 0; i < children_size; ++i)
	{
		children[i] = nullptr;
	}
}

Node::~Node()
{
	for (int i = 0; i < children_size; ++i)
	{
		if (children[i])
		{
			delete children[i];
		}
	}
}
