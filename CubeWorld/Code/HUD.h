#ifndef HUD_H
#define HUD_H

#include "stdafx.h"
#include "LoadModel.h"

class HUD
{
public:

	HUD(string filename, ID3D10Device* device, ID3D10ShaderResourceView* diff);

	D3DXMATRIX T, S, W;

	void Draw();
	void Update();
	void DrawHighScore();
	void DrawOptions();

private:
	VertexPos4NorTex* mModel;
	int mModelSize;
	LoadModel* mModelData;

	ID3D10Device*			mDevice;
	ID3D10Buffer*			mVertexBuffer;

	ID3D10EffectTechnique*	mTechnique;

	ID3D10EffectMatrixVariable* mW;
	ID3D10EffectMatrixVariable* mV;
	ID3D10EffectMatrixVariable* mP;
	ID3D10EffectMatrixVariable* mfxModelLightWVPVar;
	
	ID3D10EffectShaderResourceVariable* mEffectDiffMapVar;

	ID3D10ShaderResourceView* mDiff;
};
#endif