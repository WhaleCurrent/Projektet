#include "Effects.h"

ID3D10Effect* Effects::mModelFX	= NULL;
ID3D10Effect* Effects::mSkyFX	= NULL;
ID3D10Effect* Effects::mHudFX	= NULL;
ID3D10Effect* Effects::mFireFX	= NULL;
ID3D10Effect* Effects::mMoveFX	= NULL;

void Effects::InitAll(ID3D10Device* device)
{
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;

	D3DX10CreateEffectFromFile("../Bin/Shaders/vertex.fx", NULL, NULL, "fx_4_0", NULL, 0, device, NULL, NULL, &mModelFX , NULL, NULL );
	D3DX10CreateEffectFromFile("../Bin/Shaders/sky.fx", NULL, NULL, "fx_4_0", NULL, 0, device, NULL, NULL, &mSkyFX, NULL, NULL );
	D3DX10CreateEffectFromFile("../Bin/Shaders/hud.fx", NULL, NULL, "fx_4_0", NULL, 0, device, NULL, NULL, &mHudFX, NULL, NULL );
	D3DX10CreateEffectFromFile("../Bin/Shaders/fire.fx", NULL, NULL, "fx_4_0", NULL, 0, device, NULL, NULL, &mFireFX, NULL, NULL);
	D3DX10CreateEffectFromFile("../Bin/Shaders/move.fx", NULL, NULL, "fx_4_0", NULL, 0, device, NULL, NULL, &mMoveFX, NULL, NULL);
}