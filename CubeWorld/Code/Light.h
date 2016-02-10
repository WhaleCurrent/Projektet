#ifndef LIGHT_H
#define LIGHT_H

#include "stdafx.h"

class Light
{
public:
	struct PointLight
	{
		PointLight()
		{
			ZeroMemory(this, sizeof(Light));
		}
		D3DXVECTOR3 pos;
		float pad1; //NOT USED
		D3DXVECTOR3 dir;
		float pad2; // not used
		D3DXCOLOR ambient;
		D3DXCOLOR diffuse;
		D3DXCOLOR specular;
		D3DXVECTOR3 att;
		float spotPow;

	};
	PointLight mPointLight;

	float updater;

	Light();
	void init();
	void update();

private:
	ID3D10EffectVariable* mEyePosVarModel;
	ID3D10EffectVariable* mLightVarModel;

	ID3D10EffectVariable* mEyePosVarHud;
	ID3D10EffectVariable* mLightVarHud;

};
Light& GetLight();
#endif