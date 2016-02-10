#include "Octree.h"

Octree::Octree(int levels, float x, float y, float z)
{
	this->levels = levels;
	sizeX = x;
	sizeY = y;
	sizeZ = z;

	for(int i = 0; i < levels; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			vector<ModelInstance*> node;
			nodes.push_back(node);
		}
	}
}

void Octree::addToNode(ModelInstance* instance, int node)
{
	nodes[node].push_back(instance);
}

int Octree::findNode(float x, float y, float z)
{
	return 0;
}