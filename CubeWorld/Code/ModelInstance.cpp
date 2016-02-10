#include "ModelInstance.h"

ModelInstance::ModelInstance(float x, float y, float z)
{
	D3DXMatrixIdentity(&W);
	W._41 = x;
	W._42 = y;
	W._43 = z;

	posY = y;

	ry = RandF(0, 2 * PI);
	t = 0.0f;

	aabb = new AABB((int)x, (int)y, (int)z, 2.0f, 2.0f, 2.0f);

}

void ModelInstance::Update(int offset)
{
	D3DXMATRIX Ry, T;
	ry += gDeltaTime;
	t += gDeltaTime * 1.5f;
	D3DXMatrixRotationY(&Ry, ry);
	D3DXMatrixTranslation(&T, W._41, sinf(t) * 0.3f + posY, W._43);
	W = Ry * T;
}

bool ModelInstance::collision(float x, float y, float z)
{
	return this->aabb->collision(x, y, z);
}