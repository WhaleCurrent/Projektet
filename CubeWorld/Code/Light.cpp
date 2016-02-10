#include "Light.h"

Light& GetLight()
{
	static Light light;
	return light;
}

Light::Light()
{	
	mPointLight.pos.x = 3;
	mPointLight.pos.y = 3;
	mPointLight.pos.z = 8;
	mPointLight.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	mPointLight.diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mPointLight.specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	mPointLight.dir = -mPointLight.pos;
	D3DXVec3Normalize(&mPointLight.dir, &mPointLight.dir);
	updater = 0.0f;

}

void Light::init()
{
	mEyePosVarModel = Effects::mModelFX ->GetVariableByName("gEyePosW");
	mLightVarModel = Effects::mModelFX ->GetVariableByName("gLight");

	mEyePosVarHud = Effects::mHudFX ->GetVariableByName("gEyePosW");
	mLightVarHud = Effects::mHudFX ->GetVariableByName("gLight");
}

void Light::update()
{
	mPointLight.dir = -mPointLight.pos;
	D3DXVec3Normalize(&mPointLight.dir, &mPointLight.dir);
	mLightVarModel->SetRawValue(&mPointLight, 0, sizeof(Light));
	mEyePosVarModel->SetRawValue(GetCamera().mPosition, 0, sizeof(D3DXVECTOR3));

	mLightVarHud->SetRawValue(&mPointLight, 0, sizeof(Light));
	mEyePosVarHud->SetRawValue(GetCamera().mPosition, 0, sizeof(D3DXVECTOR3));

}