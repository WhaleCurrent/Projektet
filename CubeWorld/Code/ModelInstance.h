#ifndef ModelInstance_H
#define ModelInstance_H

#include "stdafx.h"
#include "LoadModel.h"
#include "AABB.h"

class ModelInstance
{

public:

	ModelInstance(float x, float y, float z);
	D3DXMATRIX W;
	AABB *aabb;
	void Update(int offset);
	float ry, t, posY;
	bool collision(float x, float y, float z);

};
#endif