#include "LevelHandler.h"

LevelHandler::LevelHandler(ID3D10Device *tDevice)
{
	device = tDevice;
	//BLOCKS -----
	blocks.push_back(new ModelType("../Bin/Models/playerCube.obj", device, TextureManager::mPlayer, TextureManager::mPlayer));
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mWorld1Sand, TextureManager::mWorld1Sand));
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mWorld1Stone, TextureManager::mWorld1Stone));
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mWorld1Metal, TextureManager::mWorld1Metal));
	//top 04
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mTop, TextureManager::mTop));
	//bot 05
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mBot, TextureManager::mBot));
	//right 06
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mRight, TextureManager::mRight));
	//left 07
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mLeft, TextureManager::mLeft));
	//front 08
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mFront, TextureManager::mFront));
	//back 09
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mBack, TextureManager::mBack));
	//puzzle
	blocks.push_back(new ModelType("../Bin/Models/puzzle.obj", device, TextureManager::mPuzzle, TextureManager::mPuzzle));
	//coin
	blocks.push_back(new ModelType("../Bin/Models/coin.obj", device, TextureManager::mCoin, TextureManager::mCoin));
	//temp ska bytas
	blocks.push_back(new ModelType("../Bin/Models/playerCube.obj", device, TextureManager::mPlayer, TextureManager::mPlayer));
	//stopblock
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mStop, TextureManager::mStop));

	//top_inv 14
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mTop_inv, TextureManager::mTop_inv));
	//bot_inv 15
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mBot_inv, TextureManager::mBot_inv));
	//right_inv 16
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mRight_inv, TextureManager::mRight_inv));
	//left_inv 17
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mLeft_inv, TextureManager::mLeft_inv));
	//front_inv 18
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mFront_inv, TextureManager::mFront_inv));
	//back_inv 19
	blocks.push_back(new ModelType("../Bin/Models/cube.obj", device, TextureManager::mBack_inv, TextureManager::mBack_inv));

	angle = 0;

	empty = true;

	started = false;
	positionSet = false;
	positionSetMove = false;

	mParticle.init(device, Effects::mFireFX, 100000, TextureManager::mPuzzle);
	moveParticle.init(device, Effects::mMoveFX, 100000, TextureManager::mPuzzle);
	mText = new Text(device);

	menu = new ModelInstance(0, 0, 0);

	lvlNames.push_back("../Bin/Levels/lvl01.txt");
	lvlNames.push_back("../Bin/Levels/lvl02.txt");
	lvlNames.push_back("../Bin/Levels/lvl03.txt");
	lvlNames.push_back("../Bin/Levels/lvl04.txt");
	lvlNames.push_back("../Bin/Levels/lvl05.txt");

	enemyNames.push_back("../Bin/Enemies/empty.txt");
	enemyNames.push_back("../Bin/Enemies/empty.txt");
	enemyNames.push_back("../Bin/Enemies/empty.txt");
	enemyNames.push_back("../Bin/Enemies/lvl04e.txt");
	enemyNames.push_back("../Bin/Enemies/empty.txt");

	//ENEMIES -----
	enemies.push_back(new ModelType("../Bin/Models/enemy.obj", device, TextureManager::mEnemy, TextureManager::mEnemy));

	mDeadCube = new DeadCube("../Bin/Models/playerCube.obj", device, TextureManager::mDeadPlayer, TextureManager::mDeadPlayer);

	//ska nog inte ligga här
	skybox = new Sky("../Bin/Models/sky.obj", device, TextureManager::mSky1);

		otherModels.push_back(new OtherModels(1, "../Bin/Models/Other/cactus.obj", device, TextureManager::mCactus, TextureManager::mDefaultSpec,
			D3DXVECTOR3(-30, -10, -30), D3DXVECTOR3(0, -30, 30), D3DXVECTOR3(5, 5, 5)));

		otherModels.push_back(new OtherModels(2, "../Bin/Models/Other/sun.obj", device, TextureManager::mSun, TextureManager::mDefaultSpec,
			D3DXVECTOR3(350, 100, 350), D3DXVECTOR3(0, -30, -30), D3DXVECTOR3(20, 20, 20)));

		otherModels.push_back(new OtherModels(2, "../Bin/Models/Other/sun.obj", device, TextureManager::mSun_trans, TextureManager::mDefaultSpec,
			D3DXVECTOR3(350, 100, 350), D3DXVECTOR3(0, -30, -30), D3DXVECTOR3(21, 21, 21)));

		otherModels.push_back(new OtherModels(1, "../Bin/Models/Other/palmtree.obj", device, TextureManager::mPalmtree, TextureManager::mDefaultSpec,
			D3DXVECTOR3(-10, 10, 70), D3DXVECTOR3(0, -210, -30), D3DXVECTOR3(5, 5, 5)));

		otherModels.push_back(new OtherModels(1, "../Bin/Models/Other/tombstone.obj", device, TextureManager::mTombstone, TextureManager::mDefaultSpec,
			D3DXVECTOR3(10, 10, 70), D3DXVECTOR3(0, -210, -30), D3DXVECTOR3(5, 5, 5)));

		otherModels.push_back(new OtherModels(3, "../Bin/Models/Other/ring.obj", device, TextureManager::mRing, TextureManager::mDefaultSpec,
			D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(15, 15, 15)));

		otherModels.push_back(new OtherModels(4, "../Bin/Models/Other/ring.obj", device, TextureManager::mRing, TextureManager::mDefaultSpec,
			D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(90, 0, 0), D3DXVECTOR3(13, 13, 13)));
}
void LevelHandler::createLevel(string filename)
{
	fstream in(filename);
	int c, time;
	float x, y, z;

	if(in)
		while(!in.eof())
		{
			in >> x >> y >> z >> time;

			Level test(device, x, y, z, time);
			levels.push_back(test);
			currentLevel++;

			for(int i = 0; i < y; i++)
			{
				for(int j = 0; j < z; j++)
				{
					for(int k = 0; k < x; k++)
					{
						in >> c;
						if(c != 0)
							levels[currentLevel].createInstance(c, i, j, k);
					}
				}
			}
		}
	in.close();
}
Level LevelHandler::createCleanLevel(string fileName)
{
	fstream in(fileName);
	int c, x, y, z, time;
	Level* lvl;
	if(in)
		while(!in.eof())
		{
			in >> x >> y >> z >> time;

			lvl = new Level(device, (float)x, (float)y, (float)z, time);

			for(int i = 0; i < y; i++)
			{
				for(int j = 0; j < z; j++)
				{
					for(int k = 0; k < x; k++)
					{
						in >> c;
						if(c != 0)
							lvl->createInstance(c, i, j, k);
					}
				}
			}
			
		}
	in.close();

	return *lvl;
}
void LevelHandler::addEnemies(string file)
{
	levels[currentLevel].addEnemies(file);
}
void LevelHandler::drawLevel()
{
	
	skybox->Draw();
	levels[currentLevel].draw(blocks, enemies);
	
	if(currentLevel == 0)
	{
		otherModels[0]->Draw();
		otherModels[1]->Draw();
		otherModels[2]->Draw();
	}
	else if(currentLevel == 1)
	{
		otherModels[3]->Draw();
	}
	else if(currentLevel == 2)
	{
		otherModels[5]->Draw();
		otherModels[6]->Draw();
	}
	else if(currentLevel == 3)
	{
		otherModels[4]->Draw();
	}

	levels[currentLevel].drawTrans(blocks, enemies);
	
	if(HUDdeath && !HUDNoTime)
	{
		mDeadCube->Draw();
	}
	
	mParticle.draw();
	moveParticle.draw();
	RECT rectemp;
	rectemp.bottom = 1;
	rectemp.right = 1;
	rectemp.left = 0;
	rectemp.top = 0;
	mText->DrawString("", rectemp, D3DXCOLOR(0,0,0,1), 0);
}

void LevelHandler::drawParticle()
{
	mParticle.draw();
}

void LevelHandler::update(float deltaTime)
{
	if(currentLevel == 0)
	{
		otherModels[0]->Update();
		otherModels[1]->Update();
		otherModels[2]->Update();
	}
	else if(currentLevel == 1)
	{
		otherModels[3]->Update();
	}
	else if(currentLevel == 2)
	{
		otherModels[5]->Update();
		otherModels[6]->Update();
	}
	else if(currentLevel == 3)
	{
		otherModels[4]->Update();
	}

	mDeadCube->Update();
	updateParticles();
	//D3DXVECTOR3 ppos = getPlayerPos();
}
void LevelHandler::updateParticles()
{
	if(updateParticle)
	{
		if(!positionSet)
		{
			positionSet = true;
		}
	}
	else
		positionSet = false;
	
	
	if(timePart < gameTime)
	{
		updateParticle = false;
	}
	else
	{
		updateParticle = true;
		mParticle.update(gDeltaTime, gameTime);
	}

	if(particleTimer)
	{
		particleTimer = false;
		timePart = gameTime + 0.2f;
	}

		
	
	if(updateParticleMove)
	{
		if(!positionSetMove)
		{
			positionSetMove = true;
		}
	}
	else
		positionSetMove = false;
	
	
	if(timePartMove < gameTime)
	{
		updateParticleMove = false;
	}
	else
	{
		updateParticleMove = true;
		moveParticle.update(gDeltaTime, gameTime);
	}

	if(particleTimerMove)
	{
		particleTimerMove = false;
		timePartMove = gameTime + 0.5f;
	}
}

void LevelHandler::changeLevel()
{
	if(currentLevel + 1 == (int)levels.size())
	{
		//if(currentLevel + 1 < (int)lvlNames.size())
		//{
			createLevel(lvlNames[currentLevel + 1]);
			addEnemies(enemyNames[currentLevel]);
			if(currentLevel == 0)
			{
				skybox->setTexture(TextureManager::mSky1);
				blocks[1]->setTexture(TextureManager::mWorld1Sand);
				blocks[2]->setTexture(TextureManager::mWorld1Stone);
				blocks[3]->setTexture(TextureManager::mWorld1Metal);
				SOUNDMANAGER->SetNewBackgroundSound("../Bin/Sounds/world1.wav");

			}
			else if(currentLevel == 1)
			{
				skybox->setTexture(TextureManager::mSky1);
				blocks[1]->setTexture(TextureManager::mWorld1Sand);
				blocks[2]->setTexture(TextureManager::mWorld1Stone);
				blocks[3]->setTexture(TextureManager::mWorld1Metal);
				SOUNDMANAGER->SetNewBackgroundSound("../Bin/Sounds/world1.wav");
			}
			else if(currentLevel == 2)
			{
				skybox->setTexture(TextureManager::mSky2);
				blocks[1]->setTexture(TextureManager::mWorld2Sand);
				blocks[2]->setTexture(TextureManager::mWorld2Stone);
				blocks[3]->setTexture(TextureManager::mWorld2Metal);
				SOUNDMANAGER->SetNewBackgroundSound("../Bin/Sounds/world2.wav");
			}
			else if(currentLevel == 3)
			{
				skybox->setTexture(TextureManager::mSky3);
				blocks[1]->setTexture(TextureManager::mWorld3Sand);
				blocks[2]->setTexture(TextureManager::mWorld3Stone);
				blocks[3]->setTexture(TextureManager::mWorld3Metal);
				SOUNDMANAGER->SetNewBackgroundSound("../Bin/Sounds/world3.wav");
			}
		//}
	}
	else
	{
		currentLevel++;
		resetCurrentLevel();
		if(currentLevel == 0)
		{
			skybox->setTexture(TextureManager::mSky1);
			blocks[1]->setTexture(TextureManager::mWorld1Sand);
			blocks[2]->setTexture(TextureManager::mWorld1Stone);
			blocks[3]->setTexture(TextureManager::mWorld1Metal);
			SOUNDMANAGER->SetNewBackgroundSound("../Bin/Sounds/world1.wav");

		}
		else if(currentLevel == 1)
		{
			skybox->setTexture(TextureManager::mSky1);
			blocks[1]->setTexture(TextureManager::mWorld1Sand);
			blocks[2]->setTexture(TextureManager::mWorld1Stone);
			blocks[3]->setTexture(TextureManager::mWorld1Metal);
			SOUNDMANAGER->SetNewBackgroundSound("../Bin/Sounds/world1.wav");
		}
		else if(currentLevel == 2)
		{
			skybox->setTexture(TextureManager::mSky2);
			blocks[1]->setTexture(TextureManager::mWorld2Sand);
			blocks[2]->setTexture(TextureManager::mWorld2Stone);
			blocks[3]->setTexture(TextureManager::mWorld2Metal);
			SOUNDMANAGER->SetNewBackgroundSound("../Bin/Sounds/world2.wav");
		}
		else if(currentLevel == 3)
		{
			skybox->setTexture(TextureManager::mSky3);
			blocks[1]->setTexture(TextureManager::mWorld3Sand);
			blocks[2]->setTexture(TextureManager::mWorld3Stone);
			blocks[3]->setTexture(TextureManager::mWorld3Metal);
			SOUNDMANAGER->SetNewBackgroundSound("../Bin/Sounds/world3.wav");
		}
	}
}
void LevelHandler::resetCurrentLevel()
{
	tempPoints = levels[currentLevel].getScore();

	levels[currentLevel].clearLevel();
	
	levels[currentLevel] = createCleanLevel(lvlNames[currentLevel]);

	levels[currentLevel].addEnemies(enemyNames[currentLevel]);

	levels[currentLevel].setScore(tempPoints);
}
void LevelHandler::changeNode()
{
	levels[currentLevel].changeNode();
}
bool LevelHandler::puzzleBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].puzzleBlockCollision(x, y, z);
}
bool LevelHandler::puzzlePiecesLeft()
{
	return levels[currentLevel].puzzlePiecesLeft();
}
bool LevelHandler::coinBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].coinBlockCollision(x, y, z);
}
bool LevelHandler::coinsLeft()
{
	return levels[currentLevel].coinsLeft();
}
void LevelHandler::setPlayerMatrix(D3DXMATRIX World)
{
	this->levels.at(currentLevel).setPlayerMatrix(World);
}

bool LevelHandler::collision(float x, float y, float z)
{
	return levels[currentLevel].checkCollision(x, y, z);
}

bool LevelHandler::topBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].topBlockCollision(x, y, z);
}

bool LevelHandler::botBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].botBlockCollision(x, y, z);
}

bool LevelHandler::rightBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].rightBlockCollision(x, y, z);
}

bool LevelHandler::leftBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].leftBlockCollision(x, y, z);
}

bool LevelHandler::frontBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].frontBlockCollision(x, y, z);
}

bool LevelHandler::backBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].backBlockCollision(x, y, z);
}

bool LevelHandler::stopBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].stopBlockCollision(x, y, z);
}

D3DXVECTOR3 LevelHandler::getPlayerPos()
{
	return levels[currentLevel].getPlayerPos();
}
bool LevelHandler::enemyCollision(vector<D3DXVECTOR3> points)
{
	return levels[currentLevel].enemyCollision(points);
}

void LevelHandler::startGame()
{
	if(!started)
	{
		SOUNDMANAGER->SetNewBackgroundSound("../Bin/Sounds/world1.wav");
		levelTime = 0;
		started = true;
		if(levels.size() == 0)
		{
			currentLevel = -1;
			createLevel(lvlNames[0]);
			addEnemies(enemyNames[0]);
		}
	}
}

int LevelHandler::getTime()
{
	return levels[currentLevel].getTime();
}

int LevelHandler::getScore()
{
	return levels[currentLevel].getScore();
}

int LevelHandler::getScore(int level)
{
	return levels[level].getScore();
}

void LevelHandler::setScore(int points)
{
	if(points > levels[currentLevel].getScore())
		levels[currentLevel].setScore(points);
}

void LevelHandler::saveScore()
{
	ofstream out("../Bin/Levels/highscore.txt");

	out << (int)levels.size() << endl;

	for(int i = 0; i < (int)levels.size(); i++)
	{
		out << i << " " << levels[i].getScore() << endl;
	}

	out.close();
}

void LevelHandler::loadScore()
{
	ifstream in("../Bin/Levels/highscore.txt");
	int lvls, lvlNo, points;

	in >> lvls;
	if(lvls == 0)
	{
		createLevel(lvlNames[0]);
		addEnemies(enemyNames[0]);
		levels[0].setScore(0);
	}
	else
	{
		for(int i = 0; i < lvls; i++)
		{
			createLevel(lvlNames[i]);
			addEnemies(enemyNames[i]);
			in >> lvlNo >> points;
			levels[lvlNo].setScore(points);
		}
	}

	in.close();

	currentLevel = 0;
}

int LevelHandler::chooseLevel(int level)
{
	if(level < (int)levels.size())
	{

		currentLevel = level;
		if(currentLevel == 0 || currentLevel == 1)
		{
			skybox->setTexture(TextureManager::mSky1);
			blocks[1]->setTexture(TextureManager::mWorld1Sand);
			blocks[2]->setTexture(TextureManager::mWorld1Stone);
			blocks[3]->setTexture(TextureManager::mWorld1Metal);
			SOUNDMANAGER->SetNewBackgroundSound("../Bin/Sounds/world1.wav");
		}
		else if(currentLevel == 2)
		{
			skybox->setTexture(TextureManager::mSky2);
			blocks[1]->setTexture(TextureManager::mWorld2Sand);
			blocks[2]->setTexture(TextureManager::mWorld2Stone);
			blocks[3]->setTexture(TextureManager::mWorld2Metal);
			SOUNDMANAGER->SetNewBackgroundSound("../Bin/Sounds/world2.wav");
		}
		else if(currentLevel == 3)
		{
			skybox->setTexture(TextureManager::mSky3);
			blocks[1]->setTexture(TextureManager::mWorld3Sand);
			blocks[2]->setTexture(TextureManager::mWorld3Stone);
			blocks[3]->setTexture(TextureManager::mWorld3Metal);
			SOUNDMANAGER->SetNewBackgroundSound("../Bin/Sounds/world3.wav");
		}
		resetCurrentLevel();
		return 1;
	}
	return -1;
}
bool LevelHandler::blockType1Collision(float x, float y, float z)
{
	return levels[currentLevel].blockType1Collision(x, y, z);
}
bool LevelHandler::blockType2Collision(float x, float y, float z)
{
	return levels[currentLevel].blockType2Collision(x, y, z);
}
bool LevelHandler::blockType3Collision(float x, float y, float z)
{
	return levels[currentLevel].blockType3Collision(x, y, z);
}

int LevelHandler::getSize()
{
	return levels.size();
}

int LevelHandler::getPuzzlesLeft()
{
	return levels[currentLevel].getPuzzlesLeft();
}

bool LevelHandler::invTopBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].invTopBlockCollision(x, y, z);
}
bool LevelHandler::invBotBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].invBotBlockCollision(x, y, z);
}
bool LevelHandler::invRightBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].invRightBlockCollision(x, y, z);
}
bool LevelHandler::invLeftBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].invLeftBlockCollision(x, y, z);
}
bool LevelHandler::invFrontBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].invFrontBlockCollision(x, y, z);
}
bool LevelHandler::invBackBlockCollision(float x, float y, float z)
{
	return levels[currentLevel].invBackBlockCollision(x, y, z);
}