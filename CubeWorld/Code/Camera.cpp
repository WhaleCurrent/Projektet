#include "Camera.h"

Camera& GetCamera()
{
	static Camera camera;
	return camera;
}

Camera::Camera()
{
	mPosition = D3DXVECTOR3(0.0f, 40.0f, -20.0f);
	mRight    = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	mUp       = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	mLook     = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);

	D3DXMatrixLookAtLH(&mView, &mPosition, &mLook, &mUp);
}

void Camera::setCameraLens(float fovY, float ratio, float zn, float zf)
{
	D3DXMatrixPerspectiveFovLH(&mProj, fovY, ratio, zn, zf);
}

void Camera::rebuildView(float dt)
{
	D3DXMatrixLookAtLH(&mView, &mPosition, &mLook, &mUp);
}