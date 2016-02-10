#ifndef AABB_H
#define AABB_H
#include "stdafx.h"

class AABB
{
private:
	float maxX;
	float minX;
	float maxY;
	float minY;
	float maxZ;
	float minZ;
	int x;
	int y;
	int z;


public:
	AABB();
	~AABB();
	AABB(int x, int y, int z, float sizeX, float sizeY, float sizeZ);
	bool collision(float x, float y, float z);
	bool AABBvAABB(float x, float y, float z, float sizeX, float sizeY, float sizeZ);
	bool OBBvAABB(vector<D3DXVECTOR3> points);
	void update(float x, float y, float z, float sizeX, float sizeY, float sizeZ);
	void updateSize(float x, float y, float z);
};
#endif