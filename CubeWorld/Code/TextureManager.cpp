#include "TextureManager.h"

//Model
ID3D10ShaderResourceView*	TextureManager::mWorld1Sand		= NULL;
ID3D10ShaderResourceView*	TextureManager::mWorld1Stone	= NULL;
ID3D10ShaderResourceView*	TextureManager::mWorld1Metal	= NULL;

ID3D10ShaderResourceView*	TextureManager::mWorld2Sand		= NULL;
ID3D10ShaderResourceView*	TextureManager::mWorld2Stone	= NULL;
ID3D10ShaderResourceView*	TextureManager::mWorld2Metal	= NULL;

ID3D10ShaderResourceView*	TextureManager::mWorld3Sand		= NULL;
ID3D10ShaderResourceView*	TextureManager::mWorld3Stone	= NULL;
ID3D10ShaderResourceView*	TextureManager::mWorld3Metal	= NULL;

//Special1
ID3D10ShaderResourceView*	TextureManager::mTop	= NULL;
ID3D10ShaderResourceView*	TextureManager::mBot	= NULL;
ID3D10ShaderResourceView*	TextureManager::mRight	= NULL;
ID3D10ShaderResourceView*	TextureManager::mLeft	= NULL;
ID3D10ShaderResourceView*	TextureManager::mFront	= NULL;
ID3D10ShaderResourceView*	TextureManager::mBack	= NULL;

//Special2
ID3D10ShaderResourceView*	TextureManager::mTop_inv	= NULL;
ID3D10ShaderResourceView*	TextureManager::mBot_inv	= NULL;
ID3D10ShaderResourceView*	TextureManager::mRight_inv	= NULL;
ID3D10ShaderResourceView*	TextureManager::mLeft_inv	= NULL;
ID3D10ShaderResourceView*	TextureManager::mFront_inv	= NULL;
ID3D10ShaderResourceView*	TextureManager::mBack_inv	= NULL;

//Stopblock
ID3D10ShaderResourceView*	TextureManager::mStop	= NULL;

//Player
ID3D10ShaderResourceView*	TextureManager::mPlayer	= NULL;
ID3D10ShaderResourceView*	TextureManager::mDeadPlayer	= NULL;
//Puzzle
ID3D10ShaderResourceView*	TextureManager::mPuzzle	= NULL;
//Sky
ID3D10ShaderResourceView*	TextureManager::mSky1	= NULL;
ID3D10ShaderResourceView*	TextureManager::mSky2	= NULL;
ID3D10ShaderResourceView*	TextureManager::mSky3	= NULL;
//HUD
ID3D10ShaderResourceView*	TextureManager::mHud	= NULL;
//Coin
ID3D10ShaderResourceView*	TextureManager::mCoin	= NULL;

//Other
ID3D10ShaderResourceView*	TextureManager::mCactus	= NULL;
ID3D10ShaderResourceView*	TextureManager::mSun	= NULL;
ID3D10ShaderResourceView*	TextureManager::mSun_trans	= NULL;
ID3D10ShaderResourceView*	TextureManager::mPalmtree	= NULL;
ID3D10ShaderResourceView*	TextureManager::mTombstone	= NULL;
ID3D10ShaderResourceView*	TextureManager::mRing	= NULL;

//Enemy
ID3D10ShaderResourceView*	TextureManager::mEnemy	= NULL;

//Menu
ID3D10ShaderResourceView*	TextureManager::mMenu	= NULL;
ID3D10ShaderResourceView*	TextureManager::mNoLevel	= NULL;

//Arrows
ID3D10ShaderResourceView*	TextureManager::mArrowFront	= NULL;
ID3D10ShaderResourceView*	TextureManager::mArrowBack	= NULL;
ID3D10ShaderResourceView*	TextureManager::mArrowLeft	= NULL;
ID3D10ShaderResourceView*	TextureManager::mArrowRight	= NULL;
ID3D10ShaderResourceView*	TextureManager::mArrowBot	= NULL;
ID3D10ShaderResourceView*	TextureManager::mArrowTop	= NULL;

ID3D10ShaderResourceView*	TextureManager::mArrowMulti = NULL;

//Levels
ID3D10ShaderResourceView* TextureManager::mLevel1 = NULL;
ID3D10ShaderResourceView* TextureManager::mLevel2 = NULL;
ID3D10ShaderResourceView* TextureManager::mLevel3 = NULL;
ID3D10ShaderResourceView* TextureManager::mLevel4 = NULL;
ID3D10ShaderResourceView* TextureManager::mLevel5 = NULL;
ID3D10ShaderResourceView* TextureManager::mLevel6 = NULL;
ID3D10ShaderResourceView* TextureManager::mLevel7 = NULL;
//ID3D10ShaderResourceView* TextureManager::mLevel8 = NULL;
//ID3D10ShaderResourceView* TextureManager::mLevel9 = NULL;
//ID3D10ShaderResourceView* TextureManager::mLevel10 = NULL;

//Spec
ID3D10ShaderResourceView*	TextureManager::mDefaultSpec = NULL;


void TextureManager::InitAll(ID3D10Device* device)
{
	//Model
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/World1/metal.png", 0,0,&mWorld1Metal,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/World1/sand.png", 0,0,&mWorld1Sand,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/World1/stone.png", 0,0,&mWorld1Stone,0);

	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/World2/metal.png", 0,0,&mWorld2Metal,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/World2/sand.png", 0,0,&mWorld2Sand,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/World2/stone.png", 0,0,&mWorld2Stone,0);

	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/World3/metal.png", 0,0,&mWorld3Metal,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/World3/sand.png", 0,0,&mWorld3Sand,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/World3/stone.png", 0,0,&mWorld3Stone,0);

	//Special1
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/transBlock/top.png", 0,0,&mTop,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/transBlock/bot.png", 0,0,&mBot,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/transBlock/right.png", 0,0,&mRight,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/transBlock/left.png", 0,0,&mLeft,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/transBlock/front.png", 0,0,&mFront,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/transBlock/back.png", 0,0,&mBack,0);

	//Special2
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/transBlock/top_inv.png", 0,0,&mTop_inv,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/transBlock/bot_inv.png", 0,0,&mBot_inv,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/transBlock/right_inv.png", 0,0,&mRight_inv,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/transBlock/left_inv.png", 0,0,&mLeft_inv,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/transBlock/front_inv.png", 0,0,&mFront_inv,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/transBlock/back_inv.png", 0,0,&mBack_inv,0);

	//Player
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/cube.png", 0,0,&mPlayer,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/deadCube.png", 0,0,&mDeadPlayer,0);

	//Menu
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/menu.png", 0,0,&mMenu,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/noLevel.png", 0,0,&mNoLevel,0);

	//Puzzle
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/puzzle.png", 0,0,&mPuzzle,0);

	//Sky
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Skyboxes/sky01.png", 0,0,&mSky1,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Skyboxes/sky05.png", 0,0,&mSky2,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Skyboxes/sky04b.png", 0,0,&mSky3,0);

	//Stop
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/stop.png", 0,0,&mStop,0);

	//HUD
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/hud.png", 0,0,&mHud,0);
	
	//Enemy
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/enemy.png", 0,0,&mEnemy,0);

	//Coin
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/coin.png", 0,0,&mCoin,0);

	//Other
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/OtherModels/cactus.png", 0,0,&mCactus,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/OtherModels/sun.png", 0,0,&mSun,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/OtherModels/sun_trans.png", 0,0,&mSun_trans,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/OtherModels/palmtree.png", 0,0,&mPalmtree,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/OtherModels/tombstone.png", 0,0,&mTombstone,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/OtherModels/ring.png", 0,0,&mRing,0);

	//Arrows
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Arrows/arrowFront.png", 0,0,&mArrowFront,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Arrows/arrowBack.png", 0,0,&mArrowBack,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Arrows/arrowLeft.png", 0,0,&mArrowLeft,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Arrows/arrowRight.png", 0,0,&mArrowRight,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Arrows/arrowBot.png", 0,0,&mArrowBot,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Arrows/arrowTop.png", 0,0,&mArrowTop,0);

	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Arrows/multiArrow.png", 0,0,&mArrowMulti,0);

	//Levels
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Levels/1.png", 0,0,&mLevel1,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Levels/2.png", 0,0,&mLevel2,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Levels/3.png", 0,0,&mLevel3,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Levels/4.png", 0,0,&mLevel4,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Levels/5.png", 0,0,&mLevel5,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Levels/5.png", 0,0,&mLevel6,0);
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Levels/5.png", 0,0,&mLevel7,0);
	//D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Levels/8.png", 0,0,&mLevel8,0);
	//D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Levels/9.png", 0,0,&mLevel9,0);
	//D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/Levels/10.png", 0,0,&mLevel10,0);

	//Default spec
	D3DX10CreateShaderResourceViewFromFile(device, "../Bin/Textures/defaultSpec.png", 0,0,&mDefaultSpec,0);
}
