#include "AABB.h"

AABB::AABB(int x, int y, int z, float sizeX, float sizeY, float sizeZ)
{
	maxX = x + sizeX;
	minX = x - sizeX;
	maxY = y + sizeY;
	minY = y - sizeY;
	maxZ = z + sizeZ;
	minZ = z - sizeZ;

	this->x = x;
	this->y = y;
	this->z = z;

}
AABB::~AABB()
{
}
bool AABB::collision(float x, float y, float z)
{
	if(x < maxX && x > minX && y < maxY && y > minY && z < maxZ && z > minZ)
	{
		return true;
	}

	return false;
}
void AABB::update(float x, float y, float z, float sizeX, float sizeY, float sizeZ)
{
	maxX = x + sizeX;
	minX = x - sizeX;
	maxY = y + sizeY;
	minY = y - sizeY;
	maxZ = z + sizeZ;
	minZ = z - sizeZ;
}
bool AABB::AABBvAABB(float x, float y, float z, float sizeX, float sizeY, float sizeZ)
{
	float cMaxX = x + sizeX;
	float cMinX = x - sizeX;
	float cMaxY = y + sizeY;
	float cMinY = y - sizeY;
	float cMaxZ = z + sizeZ;
	float cMinZ = z - sizeZ;

	if(collision(cMaxX, cMaxY, cMaxZ))
		return true;
	if(collision(cMaxX, cMaxY, cMinZ))
		return true;
	if(collision(cMaxX, cMinY, cMinZ))
		return true;
	if(collision(cMaxX, cMinY, cMaxZ))
		return true;

	if(collision(cMinX, cMaxY, cMaxZ))
		return true;
	if(collision(cMinX, cMaxY, cMinZ))
		return true;
	if(collision(cMinX, cMinY, cMinZ))
		return true;
	if(collision(cMinX, cMinY, cMaxZ))
		return true;


	return false;
}
bool AABB::OBBvAABB(vector<D3DXVECTOR3> points)
{
	for(int i = 0; i < (int)points.size(); i++)
	{
		if(collision(points[i].x, points[i].y, points[i].z))
			return true;
	}

	return false;
}
void AABB::updateSize(float sizeX, float sizeY, float sizeZ)
{
	maxX = x + sizeX;
	minX = x - sizeX;
	maxY = y + sizeY;
	minY = y - sizeY;
	maxZ = z + sizeZ;
	minZ = z - sizeZ;
}