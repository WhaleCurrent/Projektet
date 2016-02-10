#ifndef VOLUMEOBJECT_H
#define VOLUMEOBJECT_H

#include "stdafx.h"
#include "LoadModel.h"

class VolumeObject
{
public:
	VolumeObject(string filename, ID3D10Device* device, ID3D10ShaderResourceView* diff, int id = 0, ID3D10ShaderResourceView* spec = TextureManager::mDefaultSpec, D3DXVECTOR3 position = D3DXVECTOR3(0,0,0), D3DXVECTOR3 rotation = D3DXVECTOR3(0,0,0), D3DXVECTOR3 scale = D3DXVECTOR3(1,1,1));

	D3DXMATRIX Rx, Ry, Rz, T, S, W;
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mRotation;
	D3DXVECTOR3 mScale;
	int mId;
	float t, rot;
	D3DXVECTOR3 mStartPos;
	bool mRotated;

	void Draw();
	void UpdateMinus();
	void UpdatePlus();

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
	ID3D10EffectShaderResourceVariable* mEffectSpecMapVar;

	ID3D10ShaderResourceView* mDiff;
	ID3D10ShaderResourceView* mSpec;
};
#endif