#ifndef CUBE_H
#define CUBE_H
#include "stdafx.h"
#include "LevelHandler.h"
#include "Side.h"

class Cube
{
private:
public:
	D3DXMATRIX cubeMatrix;
private:
	D3DXMATRIX totalTrans;
	D3DXVECTOR3 pivot;
	D3DXMATRIX totalRot;

	vector<D3DXVECTOR3> xRoll;
	vector<D3DXVECTOR3> yRoll;
	vector<D3DXVECTOR3> zRoll;
	vector<D3DXVECTOR3> xRollInv;
	vector<D3DXVECTOR3> yRollInv;
	vector<D3DXVECTOR3> zRollInv;

	D3DXVECTOR3 frontTop;
	D3DXVECTOR3 frontBot;
	D3DXVECTOR3 frontRight;
	D3DXVECTOR3 frontLeft;

	D3DXVECTOR3 particleVector;

	D3DXVECTOR3 backTop;
	D3DXVECTOR3 backBot;
	D3DXVECTOR3 backRight;
	D3DXVECTOR3 backLeft;

	D3DXVECTOR3 leftTop;
	D3DXVECTOR3 leftBot;
	D3DXVECTOR3 rightTop;
	D3DXVECTOR3 rightBot;

	vector<D3DXVECTOR3> loopStart;
	D3DXVECTOR3 particleVec;
	D3DXVECTOR3 particlePos;

	vector<Side *>sides;
	vector<Side *>xSideRot;
	vector<Side *>ySideRot;
	vector<Side *>zSideRot;

	char forChar;
	char backChar;
	char rightChar;
	char leftChar;
	char tempFor;
	char tempBack;
	char tempRight;
	char tempLeft;

	bool xZ;
	bool yZ;
	bool xY;
	bool updControles;
	bool updRight;
	bool stuckInLoop;

	bool forward;
	bool backwards;
	bool right;
	bool left;

	bool doneColliding;
	int colliding;
public:
	float rotationX;
	float rotationY;
	float rotationZ;
	float rotationXz;
	float rotationXy;
	float rotationXy2;
private:
	bool yPivot;
	bool zPivot;
	bool invRot;

	long xSide;
	long ySide;
	long zSide;
	long rotNr;

	int rotFactor;
	int rotDir;
	LevelHandler *levelHandler;

	D3DXVECTOR3 landingSpot;

	int timeLeft;

public:
	Cube(LevelHandler * levelhandler);
	D3DXMATRIX move(float deltaTime);
	void setLevelHandler(LevelHandler *lh);
	float getXPos();
	float getYPos();
	float getZPos();

	D3DXVECTOR3 position;

	void rotateControls(bool right);
	void updateControles();

	void checkForError();

	vector<D3DXVECTOR3> getOBBPoints();

	int moveForwardCollision();
	int moveBackwardsCollision();
	int moveRightCollision();
	int moveLeftCollision();

	bool frontBotCollision();
	bool frontMidCollision();
	bool frontTopCollision();
	bool backBotCollision();
	bool backMidCollision();
	bool backTopCollision();
	bool rightBotCollision();
	bool rightMidCollision();
	bool rightTopCollision();
	bool leftBotCollision();
	bool leftMidCollision();
	bool leftTopCollision();
	bool frontFrontCollision();
	bool backBackCollision();
	bool rightRightCollision();
	bool leftLeftCollision();
	bool botCollision();
	bool topCollision();
	bool frontRightCollision();
	bool frontLeftCollision();
	bool backRightCollision();
	bool backLeftCollision();
	bool frontFrontBotCollision();
	bool frontFrontTopCollision();
	bool backBackBotCollision();
	bool backBackTopCollision();
	bool rightRightBotCollision();
	bool rightRightTopCollision();
	bool leftLeftBotCollision();
	bool leftLeftTopCollision();
	bool rightRightFrontCollision();
	bool rightRightBackCollision();
	bool leftLeftFrontCollision();
	bool leftLeftBackCollision();
	bool frontFrontLeftCollision();
	bool frontFrontRightCollision();
	bool backBackLeftCollision();
	bool backBackRightCollision();
	bool rightBotBotCollision();
	bool rightTopTopCollision();
	bool leftBotBotCollision();
	bool leftTopTopCollision();
	bool botBotCollision();
	bool topTopCollision();
	bool frontBotBotCollision();
	bool backBotBotCollision();
	bool frontTopTopCollision();
	bool backTopTopCollision();

	void switchSidesXPos();
	void switchSidesXNeg();
	void switchSidesYPos();
	void switchSidesYNeg();
	void switchSidesZPos();
	void switchSidesZNeg();

	int topSideCollision();
	int botSideCollision();
	int rightSideCollision();
	int leftSideCollision();
	int frontSideCollision();
	int backSideCollision();

	int checkSideCollisions();
	bool loopStuck();

	bool stopBlockCollision(int x, int y, int z);

	bool coinCollision();
	bool puzzlePieceCollision();
	bool jumpPossible(D3DXVECTOR3 position);

	void playAudio();

	void setPosition(D3DXVECTOR3 pos);

	float orgSpeed;
	float speed;
	float speed2;

	bool puzzlePiecesLeft();
	bool moving;
	void reset();
	void sendTime(int timeLeft);
	void checkWin();
	int points;
};
#endif