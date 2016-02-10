#ifndef Level_H
#define Level_H

#include "stdafx.h"
#include "ModelType.h"
#include "ModelInstance.h"
#include "Octree.h"
#include "Enemy.h"
#include "Sky.h"
#include "OtherModels.h"


class Level
{
private:
	ModelInstance* player;

	OtherModels* frontArrow;
	OtherModels* backArrow;
	OtherModels* leftArrow;
	OtherModels* rightArrow;
	OtherModels* botArrow;
	OtherModels* topArrow;

	vector<ModelInstance*> boxInstances;
	vector<ModelInstance*> sandInstances;
	vector<ModelInstance*> stoneInstances;

	vector<ModelInstance*> topBlockInstance;
	vector<ModelInstance*> botBlockInstance;
	vector<ModelInstance*> rightBlockInstance;
	vector<ModelInstance*> leftBlockInstance;
	vector<ModelInstance*> frontBlockInstance;
	vector<ModelInstance*> backBlockInstance;

	vector<ModelInstance*> invTopBlockInstance;
	vector<ModelInstance*> invBotBlockInstance;
	vector<ModelInstance*> invRightBlockInstance;
	vector<ModelInstance*> invLeftBlockInstance;
	vector<ModelInstance*> invFrontBlockInstance;
	vector<ModelInstance*> invBackBlockInstance;

	vector<ModelInstance*> puzzleBlockInstance;

	vector<ModelInstance*> coinBlockInstance;

	vector<ModelInstance*> stopBlockInstance;

	vector<Enemy*> enemies1;

	ID3D10Device *device;
	Octree *octree;

	vector<bool> puzzlePieces;
	vector<bool> coins;

	int levelX, levelY, levelZ;

	float offset;

	int currentNode;
	int time;
	int score;

public:
	Level(ID3D10Device *tDevice, float x, float y, float z, int time);
	void draw(vector<ModelType*> blockTypes, vector<ModelType*> enemyTypes);
	void drawTrans(vector<ModelType*> blockTypes, vector<ModelType*> enemyTypes);
	void drawTypes(ModelType* type, int i);
	void createInstance(int type, int y, int z, int x);
	void changeNode();

	void addEnemies(string file);

	void clearLevel();

	void drawPlayer(ModelType* playerType);
	void drawEnemies(ModelType* type, int i);
	
	bool checkCollision(float x, float y, float z);

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

	bool blockType1Collision(float x, float y, float z);
	bool blockType2Collision(float x, float y, float z);
	bool blockType3Collision(float x, float y, float z);

	bool invTopBlockCollision(float x, float y, float z);
	bool invBotBlockCollision(float x, float y, float z);
	bool invRightBlockCollision(float x, float y, float z);
	bool invLeftBlockCollision(float x, float y, float z);
	bool invFrontBlockCollision(float x, float y, float z);
	bool invBackBlockCollision(float x, float y, float z);

	void setPlayerMatrix(D3DXMATRIX W);
	D3DXVECTOR3 getPlayerPos();
	bool puzzlePiecesLeft();
	bool coinsLeft();

	int getTime();
	void setScore(int points);
	int getScore();
	int getPuzzlesLeft();
};

#endif