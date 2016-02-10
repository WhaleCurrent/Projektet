#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "stdafx.h"

namespace TextureManager
{
	//Model
	extern ID3D10ShaderResourceView*	mWorld1Sand;
	extern ID3D10ShaderResourceView*	mWorld1Stone;
	extern ID3D10ShaderResourceView*	mWorld1Metal;

	extern ID3D10ShaderResourceView*	mWorld2Sand;
	extern ID3D10ShaderResourceView*	mWorld2Stone;
	extern ID3D10ShaderResourceView*	mWorld2Metal;

	extern ID3D10ShaderResourceView*	mWorld3Sand;
	extern ID3D10ShaderResourceView*	mWorld3Stone;
	extern ID3D10ShaderResourceView*	mWorld3Metal;

	//Special1
	extern ID3D10ShaderResourceView*	mTop;
	extern ID3D10ShaderResourceView*	mBot;
	extern ID3D10ShaderResourceView*	mRight;
	extern ID3D10ShaderResourceView*	mLeft;
	extern ID3D10ShaderResourceView*	mFront;
	extern ID3D10ShaderResourceView*	mBack;

	//Special2
	extern ID3D10ShaderResourceView*	mTop_inv;
	extern ID3D10ShaderResourceView*	mBot_inv;
	extern ID3D10ShaderResourceView*	mRight_inv;
	extern ID3D10ShaderResourceView*	mLeft_inv;
	extern ID3D10ShaderResourceView*	mFront_inv;
	extern ID3D10ShaderResourceView*	mBack_inv;

	//Stopblock
	extern ID3D10ShaderResourceView*	mStop;
	//Player
	extern ID3D10ShaderResourceView*	mPlayer;
	extern ID3D10ShaderResourceView*	mDeadPlayer;
	//Puzzle
	extern ID3D10ShaderResourceView*	mPuzzle;
	//Sky
	extern ID3D10ShaderResourceView*	mSky1;
	extern ID3D10ShaderResourceView*	mSky2;
	extern ID3D10ShaderResourceView*	mSky3;
	//HUD
	extern ID3D10ShaderResourceView*	mHud;
	//Coin
	extern ID3D10ShaderResourceView*	mCoin;

	//Enemy
	extern ID3D10ShaderResourceView*	mEnemy;

	//Menu
	extern ID3D10ShaderResourceView*	mMenu;
	extern ID3D10ShaderResourceView*	mNoLevel;

	//Other
	extern ID3D10ShaderResourceView*	mCactus;
	extern ID3D10ShaderResourceView*	mSun;
	extern ID3D10ShaderResourceView*	mSun_trans;
	extern ID3D10ShaderResourceView*	mPalmtree;
	extern ID3D10ShaderResourceView*	mTombstone;
	extern ID3D10ShaderResourceView*	mRing;

	//Arrows
	extern ID3D10ShaderResourceView*	mArrowFront;
	extern ID3D10ShaderResourceView*	mArrowBack;
	extern ID3D10ShaderResourceView*	mArrowLeft;
	extern ID3D10ShaderResourceView*	mArrowRight;
	extern ID3D10ShaderResourceView*	mArrowBot;
	extern ID3D10ShaderResourceView*	mArrowTop;

	extern ID3D10ShaderResourceView*	mArrowMulti;

	//Default spec
	extern ID3D10ShaderResourceView*	mDefaultSpec;

	//Levels
	extern ID3D10ShaderResourceView* mLevel1;
	extern ID3D10ShaderResourceView* mLevel2;
	extern ID3D10ShaderResourceView* mLevel3;
	extern ID3D10ShaderResourceView* mLevel4;
	extern ID3D10ShaderResourceView* mLevel5;
	extern ID3D10ShaderResourceView* mLevel6;
	extern ID3D10ShaderResourceView* mLevel7;
	//extern ID3D10ShaderResourceView* mLevel8;
	//extern ID3D10ShaderResourceView* mLevel9;
	//extern ID3D10ShaderResourceView* mLevel10;

	void InitAll(ID3D10Device* device);
};
#endif