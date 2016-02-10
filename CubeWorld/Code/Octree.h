#ifndef Octree_h
#define Octree_h

#include "stdafx.h"
#include "ModelInstance.h"

class Octree
{
public:
	Octree(int levels, float x, float y, float z);
	void addToNode(ModelInstance* instance, int node);
	int findNode(float x, float y, float z);
	vector<vector<ModelInstance*>> nodes;
private:
	
	int levels;
	float sizeX, sizeY, sizeZ;
};

#endif