#ifndef MENU_H
#define MENU_H

#include "stdafx.h"
#include "LoadModel.h"
#include "MenuObjects.h"
#include "MenuText.h"
#include "VolumeObject.h"
#include "Highscore.h"
#include "Sky.h"
#include "Text.h"
#include "SoundManager.h"

class Menu
{
public:

	Menu(string filename, ID3D10Device* device, ID3D10ShaderResourceView* diff, ID3D10ShaderResourceView* spec);

	D3DXMATRIX T, S, Rx, Ry, Rz, W;
	float mRotX, mRotY, mRotZ;

	D3DXVECTOR3 mPosition;

	void changeMenu(int type);
	void Draw();
	void Update();
	void RotateY();

	int mRotatingX, mRotatingY, mRotatingZ;
	int mSide;
	bool mIsMoving;

	Sky* skybox;

	Text* mText;

	vector<Highscore> mHighscores;
	vector<MenuObjects> mMiniLevel;
	MenuText* mMenuText;
	MenuText* mMenuTextVolume;
	MenuText* mMenuTextMinus;
	MenuText* mMenuTextPlus;

	VolumeObject* mVolumeObject;

	void setTexture();
	void loadTextures(int lvl);
	void setSkyTexture(ID3D10ShaderResourceView* diff);
	int mCompletedLevels;

private:
	VertexPos4NorTex* mModel;
	int mModelSize;
	LoadModel* mModelData;

	bool left, right, up, down, mainMenu;

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