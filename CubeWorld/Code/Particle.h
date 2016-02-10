#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "stdafx.h"

class Particle
{
public:
	struct ParticleVertex
	{
		D3DXVECTOR3 initialPos;
		D3DXVECTOR3 initialVel;
		D3DXVECTOR2 size;
		float age;
		unsigned int type;
		D3DXVECTOR3 color;
	};

	Particle();
	~Particle();

	void init(ID3D10Device* device, ID3D10Effect* effects, UINT maxParticles, ID3D10ShaderResourceView* diff);

	void update(float deltaTime, float gameTime);
	void draw();
	
private:
	void buildVB();
	void buildRandomTex();
 
private:
 
	UINT mMaxParticles;
	bool mFirstRun;

	float mGameTime;
	float mTimeStep;
public:
	bool mUpdate;
	D3DXVECTOR4 mEyePosW;
	D3DXVECTOR4 mEmitPosW;
private:

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mInitVB;	
	ID3D10Buffer* mDrawVB;
	ID3D10Buffer* mStreamOutVB;
 
	
	ID3D10EffectTechnique*				mStreamOutTech;
	ID3D10EffectTechnique*				mDrawTech;
	ID3D10EffectMatrixVariable*			mViewProjVar;
	ID3D10EffectScalarVariable*			mGameTimeVar;
	ID3D10EffectScalarVariable*			mTimeStepVar;
	ID3D10EffectVectorVariable*			mEyePosVar;
	ID3D10EffectVectorVariable*			mEmitPosVar;
	ID3D10EffectVectorVariable*			mEmitDirVar;
	ID3D10EffectShaderResourceVariable* mRandomTexVar;
	ID3D10ShaderResourceView*			mRandomTex;
	ID3D10EffectShaderResourceVariable* mTextureVar;
	ID3D10ShaderResourceView*			mTexture;

	ID3D10EffectScalarVariable*			mUpdateVar;
	ID3D10EffectVectorVariable*			mParticleColorVar;
	ID3D10EffectVectorVariable*			mSizeVar;

	bool isFire;
};
#endif