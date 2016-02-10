#ifndef LevelHandler_H
#define LevelHandler_H

#include "stdafx.h"
#include "Level.h"
#include "OtherModels.h"
#include "DeadCube.h"
#include "Particle.h"
#include "Particle2.h"
#include "Text.h"
#include "SoundManager.h"

class LevelHandler
{
private:
	
	vector<Level> levels;
	vector<ModelType*> blocks;
	vector<ModelType*> enemies;
	ID3D10Device *device;
	ModelInstance* menu;
	vector<string> lvlNames;
	vector<string> enemyNames;
	int tempPoints;
	float angle;
	bool started;

public:
	bool empty;
	bool positionSet;
	bool positionSetMove;

	Text* mText;

	Sky* skybox;
	vector<OtherModels*> otherModels;

	DeadCube* mDeadCube;
	Particle mParticle;
	Particle2 moveParticle;

	LevelHandler(ID3D10Device *device);
	void createLevel(string filename);
	void drawLevel();
	void drawMenu(float dt);
	void drawParticle();
	void changeMenu(int type);
	void update(float deltaTime);
	void updateParticles();
	void changeLevel();
	void changeNode();
	void addEnemies(string file);
	void resetCurrentLevel();
	Level createCleanLevel(string fileName);

	void startGame();

	void setPlayerMatrix(D3DXMATRIX World);
	D3DXVECTOR3 getPlayerPos();
	bool collision(float x, float y, float z);

	bool topBlockCollision(float x, float y, float z);
	bool botBlockCollision(float x, float y, float z);
	bool rightBlockCollision(float x, float y, float z);
	bool leftBlockCollision(float x, float y, float z);
	bool frontBlockCollision(float x, float y, float z);
	bool backBlockCollision(float x, float y, float z);

	bool stopBlockCollision(float x, float y, float z);

	bool coinBlockCollision(float x, float y, float z);
	bool puzzleBlockCollision(float x, float y, float z);
	bool enemyCollision(vector<D3DXVECTOR3> points);

	bool invTopBlockCollision(float x, float y, float z);
	bool invBotBlockCollision(float x, float y, float z);
	bool invRightBlockCollision(float x, float y, float z);
	bool invLeftBlockCollision(float x, float y, float z);
	bool invFrontBlockCollision(float x, float y, float z);
	bool invBackBlockCollision(float x, float y, float z);

	bool blockType1Collision(float x, float y, float z);
	bool blockType2Collision(float x, float y, float z);
	bool blockType3Collision(float x, float y, float z);

	bool puzzlePiecesLeft();
	bool coinsLeft();
	int getTime();
	int getScore();
	int getScore(int level);
	void setScore(int points);
	int getPuzzlesLeft();

	void saveScore();
	void loadScore();
	int chooseLevel(int level);

	int getSize();
};

#endif