#ifndef ModelType_H
#define ModelType_H

#include "stdafx.h"
#include "LoadModel.h"

class ModelType
{

public:

	ModelType(string filename, ID3D10Device* device, ID3D10ShaderResourceView* diff, ID3D10ShaderResourceView* spec);

	D3DXMATRIX Rx, Ry, Rz, T, S, W;
	void Draw();
	void Update(float dt);
	string type;
	void setTexture(ID3D10ShaderResourceView* diff);
private:
	VertexPos4NorTex* mModel;
	int mModelSize;
	LoadModel* mModelData;

	ID3D10Device*			mDevice;
	ID3D10EffectTechnique*	mTechnique;
	ID3D10Buffer*			mVertexBuffer;

	ID3D10EffectMatrixVariable* mW;
	ID3D10EffectMatrixVariable* mV;
	ID3D10EffectMatrixVariable* mP;
	ID3D10EffectMatrixVariable* mfxModelLightWVPVar;
	
	ID3D10EffectShaderResourceVariable* mEffectDiffMapVar;
	ID3D10EffectShaderResourceVariable* mEffectSpecMapVar;

	ID3D10ShaderResourceView* mDiff;
	ID3D10ShaderResourceView* mSpec;
};
#endif