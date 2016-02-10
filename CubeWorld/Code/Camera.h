#ifndef CAMERA_H
#define CAMERA_H

#include "stdafx.h"

class Camera
{
public:
	Camera();
	void setCameraLens(float fovY, float ratio, float zn, float zf);
	void rebuildView(float dt);

	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mRight;
	D3DXVECTOR3 mUp;
	D3DXVECTOR3 mLook;

	D3DXVECTOR3 target;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	float Y;
};

Camera& GetCamera();
#endif