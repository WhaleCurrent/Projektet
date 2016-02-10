#ifndef SKY_H
#define SKY_H

#include "stdafx.h"
#include "LoadModel.h"

class Sky
{
public:

	Sky(string filename, ID3D10Device* device, ID3D10ShaderResourceView* diff);

	D3DXMATRIX Rx, Ry, Rz, T, S, W;

	void Draw();
	void Update();
	void setTexture(ID3D10ShaderResourceView* diff);
	ID3D10ShaderResourceView*	mShaderResourceDiff;
private:
	VertexPosNorTex* mModel;
	int mModelSize;
	LoadModel* mModelData;

	ID3D10Device*			mDevice;
	ID3D10Buffer*			mVertexBuffer;

	ID3D10EffectTechnique*	mTechnique;

	ID3D10EffectMatrixVariable* mW;
	ID3D10EffectMatrixVariable* mVP;

	
	
	ID3D10EffectShaderResourceVariable* mEffectDiffMapVar;
};
#endif