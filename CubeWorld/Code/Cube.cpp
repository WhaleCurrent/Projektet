#include "Cube.h"
#include <algorithm>
#include "SoundManager.h"
//JONAS KUB
Cube::Cube(LevelHandler* levelhandler)
{
	this->levelHandler = levelhandler;
	D3DXMatrixIdentity(&cubeMatrix);
	D3DXMatrixIdentity(&totalRot);
	D3DXMatrixIdentity(&totalTrans);
	totalTrans._42 = 12;
	totalTrans._41 = 4;
	//totalTrans._43 = 20;
	float pos = 2.0f;
	float neg = -2.0f;

	forChar = 'W';
	backChar = 'S';
	rightChar = 'D';
	leftChar = 'A';

	tempFor = forChar;
	tempBack = backChar;
	tempRight = rightChar;
	tempLeft = leftChar;

	moving = false;
	updControles = false;
	updRight = false;
	colliding = false;
	stuckInLoop = false;

	xZ = true;
	xY = false;
	yZ = false;

	forward = false;
	backwards = false;
	right = false;
	left = false;

	rotationX = 0;
	rotationY = 0;
	rotationZ = 0;
	rotationXz = 0;
	rotationXy = 0;
	rotationXy2 = 0;

	yPivot = false;
	zPivot = false;
	invRot = false;

	sides.push_back(new Side("bot")); //0
	sides.push_back(new Side("top")); //1
	sides.push_back(new Side("front")); //2
	sides.push_back(new Side("back")); //3
	sides.push_back(new Side("left")); //4
	sides.push_back(new Side("right")); //5

	//rulla fram -  negativ
	xSideRot.push_back(sides.at(2)); //front
	xSideRot.push_back(sides.at(0)); //bot
	xSideRot.push_back(sides.at(3)); //back
	xSideRot.push_back(sides.at(1)); //top

	//höger - negativ
	ySideRot.push_back(sides.at(5)); //right
	ySideRot.push_back(sides.at(2)); //front
	ySideRot.push_back(sides.at(4)); //left
	ySideRot.push_back(sides.at(3)); //back

	//höger -  positiv
	zSideRot.push_back(sides.at(0)); //bot
	zSideRot.push_back(sides.at(4)); //left
	zSideRot.push_back(sides.at(1)); //top
	zSideRot.push_back(sides.at(5)); //right

	orgSpeed = 3.0f;
	speed = orgSpeed;
	speed2 = 5.0f;
	rotDir = 1;

	rotFactor = 1;

	frontTop = D3DXVECTOR3(0.0f, neg, neg);
	frontBot = D3DXVECTOR3(0.0f, pos, neg);
	frontRight = D3DXVECTOR3(neg, 0.0f, neg);
	frontLeft = D3DXVECTOR3(pos, 0.0f, neg);

	backTop = D3DXVECTOR3(0.0f, neg, pos);
	backBot = D3DXVECTOR3(0.0f, pos, pos);
	backRight = D3DXVECTOR3(neg, 0.0f, pos);
	backLeft = D3DXVECTOR3(pos, 0.0f, pos);

	leftTop = D3DXVECTOR3(pos, neg, 0.0f);
	leftBot = D3DXVECTOR3(pos, pos, 0.0f);
	rightTop = D3DXVECTOR3(neg, neg, 0.0f);
	rightBot = D3DXVECTOR3(neg, pos, 0.0f);

}
//Cube::~Cube()
//{
//	delete this->levelHandler;
//	for(int i = 0; i < 4; i++)
//	{
//		delete xSideRot[i];
//		delete ySideRot[i];
//		delete zSideRot[i];
//	}
//	for(int i = 0; i < 6; i++)
//	{
//		delete sides[i];
//	}
//}
void Cube::setLevelHandler(LevelHandler *lh)
{
	this->levelHandler = lh;
}
float Cube::getXPos()
{
	return totalTrans._41;
}
float Cube::getYPos()
{
	return totalTrans._42;
}
float Cube::getZPos()
{
	return totalTrans._43;
}
D3DXMATRIX Cube::move(float deltaTime)
{
	
	static float colDelay = 0.0f;

	D3DXMATRIX rotMatrix;
	D3DXMATRIX trans1;
	D3DXMATRIX trans2;
	D3DXMATRIX totRotMatrix;
	D3DXMatrixIdentity(&trans1);
	D3DXMatrixIdentity(&trans2);
	D3DXMatrixIdentity(&rotMatrix);
	

	if(GetAsyncKeyState(forChar) && !moving && !colliding)
	{
		moving = true;
		forward = true;
		int collision = this->moveForwardCollision();
		if(collision == 3 && !stopBlockCollision(0, 4, 4))
		{
			pivot = this->frontTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 19 && !stopBlockCollision(0, 4, 4))
		{
			pivot = this->frontTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 1 && !stopBlockCollision(0, -4, 4))
		{
			pivot = this->frontBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 2 && !stopBlockCollision(0, 0, 4))
		{
			rotFactor = 2;
			pivot = this->frontTop;
			landingSpot = D3DXVECTOR3(0, 0, 4);
		}
		else if(collision == 4 && !stopBlockCollision(0, 0, 4))
		{
			rotFactor = 2;
			pivot = this->frontBot;
			speed = speed2;
			landingSpot = D3DXVECTOR3(0, -4, 0);
		}
		else if(collision == 24 && !stopBlockCollision(0, -4, 0))
		{
			rotFactor = 2;
			pivot = this->frontBot;
			speed = speed2;
			landingSpot = D3DXVECTOR3(0, -4, 0);
		}
		else if(collision == 5 && !stopBlockCollision(0, 4, -4))
		{
			pivot = this->backTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 6 && !stopBlockCollision(0, -4, -4))
		{
			pivot = this->backBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 20 && !stopBlockCollision(0, -4, -4))
		{
			pivot = this->backBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 7 && !stopBlockCollision(0, 4, -4))
		{
			pivot = this->backTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 8 && !stopBlockCollision(0, -4, -4))
		{
			pivot = this->backBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 9 && !stopBlockCollision(0, 0, -4))
		{
			rotFactor = 2;
			pivot = this->backBot;
			landingSpot = D3DXVECTOR3(0, 0, -4);
		}
		else if(collision == 10 && !stopBlockCollision(0, 4, 0))
		{
			rotFactor = 2;
			pivot = this->backTop;
			landingSpot = D3DXVECTOR3(0, 4, 0);
		}
		else if(collision == 11 && !stopBlockCollision(0, 4, 0))
		{
			rotFactor = 2;
			pivot = this->backTop;
			landingSpot = D3DXVECTOR3(0, 4, 0);
		}
		else if(collision == 14 && !stopBlockCollision(0, 0, -4))
		{
			pivot = this->backBot;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(0, 0, -4);
		}
		else if(collision == 12 && !stopBlockCollision(-4, 4, 0))
		{
			pivot = this->leftTop;
			zPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 13 && !stopBlockCollision(-4, 0, 0))
		{
			pivot = this->leftTop;
			zPivot = true;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(-4, 0, 0);
		}
		else if(collision == 15 && !stopBlockCollision(4, -4, 0))
		{
			pivot = this->rightBot;
			zPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 16 && !stopBlockCollision(4, 0, 0))
		{
			pivot = this->rightBot;
			zPivot = true;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(4, 0, 0);
		}
		else if(collision == 17 && !stopBlockCollision(4, -4, 0))
		{
			pivot = this->rightBot;
			zPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 18 && !stopBlockCollision(-4, 4, 0))
		{
			pivot = this->leftTop;
			zPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 22 && !stopBlockCollision(0, -4, -4))
		{
			pivot = this->backBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 23 && !stopBlockCollision(-4, 0, -4))
		{
			pivot = this->backLeft;
			yPivot = true;
			invRot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 25 && !stopBlockCollision(-4, 0, 4))
		{
			pivot = this->frontLeft;
			yPivot = true;
			invRot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 26 && !stopBlockCollision(-4, 0, -4))
		{
			pivot = this->backLeft;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else
		{
			moving = false;
			forward = false;
		}
	}
	else if(GetAsyncKeyState(backChar) && !moving && !colliding)
	{
		moving = true;
		backwards = true;

		int collision = this->moveBackwardsCollision();
		if(collision == 3 && !stopBlockCollision(0, 4, -4))
		{
			pivot = this->backTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 19 && !stopBlockCollision(0, 4, -4))
		{
			pivot = this->backTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 1 && !stopBlockCollision(0, -4, -4))
		{
			pivot = this->backBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 2 && !stopBlockCollision(0, 0, -4))
		{
			rotFactor = 2;
			pivot = this->backTop;
			landingSpot = D3DXVECTOR3(0, 0, -4);
		}
		else if(collision == 4 && !stopBlockCollision(0, -4, 0))
		{
			rotFactor = 2;
			pivot = this->backBot;
			speed = speed2;
			landingSpot = D3DXVECTOR3(0, -4, 0);
		}
		else if(collision == 5 && !stopBlockCollision(0, 4, 4))
		{
			pivot = this->frontTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 6 && !stopBlockCollision(0, -4, 4))
		{
			pivot = this->frontBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 20 && !stopBlockCollision(0, -4, 4))
		{
			pivot = this->frontBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 7 && !stopBlockCollision(0, 4, 4))
		{
			pivot = this->frontTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 8 && !stopBlockCollision(0, -4, 4))
		{
			pivot = this->frontBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 9 && !stopBlockCollision(0, 0, 4))
		{
			rotFactor = 2;
			pivot = this->frontBot;
			landingSpot = D3DXVECTOR3(0, 0, 4);
		}
		else if(collision == 10 && !stopBlockCollision(0, 4, 0))
		{
			rotFactor = 2;
			pivot = this->frontTop;
			landingSpot = D3DXVECTOR3(0, 4, 0);
		}
		else if(collision == 11 && !stopBlockCollision(0, 4, 0))
		{
			rotFactor = 2;
			pivot = this->frontTop;
			landingSpot = D3DXVECTOR3(0, 4, 0);
		}
		else if(collision == 12 && !stopBlockCollision(-4, -4, 0))
		{
			zPivot = true;
			pivot = this->leftBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 13 && !stopBlockCollision(-4, 0, 0))
		{
			zPivot = true;
			pivot = this->leftBot;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(-4, 0, 0);
		}
		else if(collision == 14 && !stopBlockCollision(0, 0, 4))
		{
			pivot = this->frontBot;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(0, 0, 4);
		}
		else if(collision == 15 && !stopBlockCollision(4, 4, 0))
		{
			pivot = this->rightTop;
			zPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 16 && !stopBlockCollision(4, 0, 0))
		{
			pivot = this->rightTop;
			zPivot = true;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(4, 0, 0);
		}
		else if(collision == 17 && !stopBlockCollision(-4, -4, 0))
		{
			pivot = this->leftBot;
			zPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 18 && !stopBlockCollision(4, 4, 0))
		{
			pivot = this->rightTop;
			zPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 21 && !stopBlockCollision(4, 0, -4))
		{
			pivot = this->backRight;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 22 && !stopBlockCollision(0, -4, 4))
		{
			pivot = this->frontBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 23 && !stopBlockCollision(4, 0, 4))
		{
			pivot = this->frontRight;
			yPivot = true;
			invRot = true;
			landingSpot = pivot * -2;
		}
		else
		{
			moving = false;
			backwards = false;
		}
	}
	else if(GetAsyncKeyState(leftChar) && !moving && !colliding)
	{
		moving = true;
		left = true;

		int collision = this->moveLeftCollision();
		if(collision == 3 && !stopBlockCollision(-4, 4, 0))
		{
			pivot = this->leftTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 1 && !stopBlockCollision(-4, -4, 0))
		{
			pivot = this->leftBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 4 && !stopBlockCollision(0, -4, 0))
		{
			rotFactor = 2;
			pivot = this->leftBot;
			speed = speed2;
			landingSpot = D3DXVECTOR3(0, -4, 0);
		}
		else if(collision == 5 && !stopBlockCollision(4, 4, 0))
		{
			pivot = this->rightTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 6 && !stopBlockCollision(0, -4, 0))
		{
			pivot = this->rightBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 7 && !stopBlockCollision(4, 4, 0))
		{
			pivot = this->rightTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 8 && !stopBlockCollision(0, -4, 0))
		{
			pivot = this->rightBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 9 && !stopBlockCollision(4, 0, 0))
		{
			rotFactor = 2;
			pivot = this->rightBot;
			landingSpot = D3DXVECTOR3(4, 0, 0);
		}
		else if(collision == 10  && !stopBlockCollision(0, 4, 0))
		{
			rotFactor = 2;
			pivot = this->rightTop;
			landingSpot = D3DXVECTOR3(0, 4, 0);
		}
		else if(collision == 11 && !stopBlockCollision(0, 4, 0))
		{
			rotFactor = 2;
			pivot = this->rightTop;
			landingSpot = D3DXVECTOR3(0, 4, 0);
		}
		else if(collision == 12 && !stopBlockCollision(-4, 0, 4))
		{
			pivot = this->frontLeft;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 29 && !stopBlockCollision(-4, 0, 4))
		{
			pivot = this->frontLeft;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 13 && !stopBlockCollision(0, 0, 4))
		{
			pivot = this->frontLeft;
			yPivot = true;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(0, 0, 4);
		}
		else if(collision == 14 && !stopBlockCollision(4, 0, 4))
		{
			pivot = this->frontRight;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 32 && !stopBlockCollision(4, 0, 4))
		{
			pivot = this->frontRight;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 15 && !stopBlockCollision(-4, 0, -4))
		{
			pivot = this->backLeft;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 31 && !stopBlockCollision(-4, 0, -4))
		{
			pivot = this->backLeft;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 16 && !stopBlockCollision(-4, 0, 0))
		{
			pivot = this->backLeft;
			yPivot = true;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(-4, 0, 0);
		}
		else if(collision == 17 && !stopBlockCollision(0, 0, -4))
		{
			pivot = this->backRight;
			rotFactor = 2;
			yPivot = true;
			landingSpot = D3DXVECTOR3(0, 0, -4);
		}
		else if(collision == 18 && !stopBlockCollision(4, 0, 0))
		{
			pivot = this->frontRight;
			rotFactor = 2;
			yPivot = true;
			landingSpot = D3DXVECTOR3(4, 0, 0);
		}
		else if(collision == 19 && !stopBlockCollision(4, 0, -4))
		{
			pivot = this->backRight;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 30 && !stopBlockCollision(4, 0, -4))
		{
			pivot = this->backRight;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 20 && !stopBlockCollision(-4, 0, 0))
		{
			pivot = this->backLeft;
			yPivot = true;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(-4, 0, 0);
		}
		else if(collision == 21 && !stopBlockCollision(4, 0, 0))
		{
			pivot = this->rightBot;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(4, 0, 0);
		}
		else if(collision == 22 && !stopBlockCollision(-4, 0, 0))
		{
			pivot = this->leftTop;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(-4, 0, 0);
		}
		else if(collision == 23 && !stopBlockCollision(-4, 4, 0))
		{
			pivot = this->leftTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 24 && !stopBlockCollision(4, 0, 0))
		{
			pivot = this->frontRight;
			yPivot = true;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(4, 0, 0);
		}
		else if(collision == 25 && !stopBlockCollision(-4, 4, 0))
		{
			pivot = this->leftTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 27 && !stopBlockCollision(0, -4, 0))
		{
			pivot = this->leftBot;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(0, -4, 0);
		}
		else if(collision == 28 && !stopBlockCollision(4, -4, 0))
		{
			pivot = this->rightBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 33 && !stopBlockCollision(-4, 0, 4))
		{
			pivot = this->frontLeft;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 35 && !stopBlockCollision(4, 0, -4))
		{
			pivot = this->backRight;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else
		{
			moving = false;
			left = false;
		}
	}
	else if(GetAsyncKeyState(rightChar) && !moving && !colliding)
	{
		moving = true;
		right = true;

		int collision = this->moveRightCollision();
		if(collision == 3 && !stopBlockCollision(4, 4, 0))
		{
			pivot = this->rightTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 1 && !stopBlockCollision(4, -4, 0))
		{
			pivot = this->rightBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 4 && !stopBlockCollision(0, -4, 0))
		{
			rotFactor = 2;
			pivot = this->rightBot;
			speed = speed2;
			landingSpot = D3DXVECTOR3(0, -4, 0);
		}
		else if(collision == 5 && !stopBlockCollision(-4, 4, 0))
		{
			pivot = this->leftTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 6 && !stopBlockCollision(-4, -4, 0))
		{
			pivot = this->leftBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 7 && !stopBlockCollision(-4, 4, 0))
		{
			pivot = this->leftTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 8 && !stopBlockCollision(-4, -4, 0))
		{
			pivot = this->leftBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 9 && !stopBlockCollision(-4, 0, 0))
		{
			rotFactor = 2;
			pivot = this->leftBot;
			landingSpot = D3DXVECTOR3(-4, 0, 0);
		}
		else if(collision == 11 && !stopBlockCollision(0, 4, 0))
		{
			rotFactor = 2;
			pivot = this->leftTop;
			landingSpot = D3DXVECTOR3(0, 4, 0);
		}
		else if(collision == 32 && !stopBlockCollision(4, 0, 4))
		{
			pivot = this->frontRight;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 12 && !stopBlockCollision(4, 0, 4))
		{
			pivot = this->frontRight;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 29 && !stopBlockCollision(4, 0, 4))
		{
			pivot = this->frontRight;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 13 && !stopBlockCollision(0, 0, 4))
		{
			pivot = this->frontRight;
			yPivot = true;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(0, 0, 4);
		}
		else if(collision == 14 && !stopBlockCollision(-4, 0, 4))
		{
			pivot = this->frontLeft;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 15 && !stopBlockCollision(-4, 0, 0))
		{
			pivot = this->frontLeft;
			yPivot = true;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(-4, 0, 0);
		}
		else if(collision == 16 && !stopBlockCollision(0, 0, -4))
		{
			pivot = this->backLeft;
			yPivot = true;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(0, 0, -4);
		}
		else if(collision == 17 && !stopBlockCollision(4, 0, 0))
		{
			pivot = this->backRight;
			yPivot = true;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(4, 0, 0);
		}
		else if(collision == 18 && !stopBlockCollision(4, 0, -4))
		{
			pivot = this->backRight;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 31 && !stopBlockCollision(4, 0, -4))
		{
			pivot = this->backRight;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 35 && !stopBlockCollision(-4, 0, -4))
		{
			pivot = this->backLeft;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 19 && !stopBlockCollision(-4, 0, -4))
		{
			pivot = this->backLeft;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 30 && !stopBlockCollision(-4, 0, -4))
		{
			pivot = this->backLeft;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else if(collision == 20 && !stopBlockCollision(4, 0, 0))
		{
			pivot = this->backRight;
			yPivot = true;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(4, 0, 0);
		}
		else if(collision == 21 && !stopBlockCollision(-4, 0, 0))
		{
			pivot = this->leftBot;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(-4, 0, 0);
		}
		else if(collision == 22 && !stopBlockCollision(4, 0, 0))
		{
			pivot = this->rightTop;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(4, 0, 0);
		}
		else if(collision == 23 && !stopBlockCollision(4, 4, 0))
		{
			pivot = this->rightTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 24 && !stopBlockCollision(4, 4, 0))
		{
			pivot = this->rightTop;
			landingSpot = pivot * -2;
		}
		else if(collision == 25 && !stopBlockCollision(-4, -4, 0))
		{
			pivot = this->leftBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 27 && !stopBlockCollision(0, -4, 0))
		{
			pivot = this->rightBot;
			rotFactor = 2;
			landingSpot = D3DXVECTOR3(0, -4, 0);
		}
		else if(collision == 28 && !stopBlockCollision(-4, -4, 0))
		{
			pivot = this->leftBot;
			landingSpot = pivot * -2;
		}
		else if(collision == 33 && !stopBlockCollision(4, 0, -4))
		{
			pivot = this->backRight;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		//else if(collision == 30)
		//{
		//	pivot = this->backLeft;
		//	yPivot = true;
		//}
		else if(collision == 36 && !stopBlockCollision(-4, 0, 4))
		{
			pivot = this->frontLeft;
			yPivot = true;
			landingSpot = pivot * -2;
		}
		else
		{
			moving = false;
			right = false;
		}
	}

	if(forward && moving)
	{
		if(zPivot)
		{
			if(rotationXz > PI * 0.2)
				rotationXz += speed * 3 * deltaTime;
			else
				rotationXz += speed * deltaTime;

			D3DXMatrixRotationZ(&rotMatrix, rotationXz);
		}
		else if(yPivot && !invRot)
		{
			if(rotationXy < -(PI * 0.2))
				rotationXy -= speed * 3 * deltaTime;
			else
				rotationXy -= speed * deltaTime;

			D3DXMatrixRotationY(&rotMatrix, rotationXy);
		}
		else if(yPivot)
		{
			if(rotationXy > PI * 0.2)
				rotationXy += speed * 3 * deltaTime;
			else
				rotationXy += speed * deltaTime;

			D3DXMatrixRotationY(&rotMatrix, rotationXy);
		}
		else
		{
			if(rotationX > PI * 0.2)
				rotationX += speed * 3 * deltaTime;
			else
				rotationX += speed * deltaTime;

			D3DXMatrixRotationX(&rotMatrix, rotationX);
		}
	}
	else if(backwards && moving)
	{	
		if(zPivot)
		{
			if(rotationXz < -(PI * 0.2))
				rotationXz -= speed * 3 * deltaTime;
			else
				rotationXz -= speed * deltaTime;

			D3DXMatrixRotationZ(&rotMatrix, rotationXz);
		}
		else if(yPivot && invRot)
		{
			if(rotationXy2 < -(PI * 0.2))
				rotationXy2 -= speed * 3 * deltaTime;
			else
				rotationXy2 -= speed * deltaTime;

			D3DXMatrixRotationY(&rotMatrix, rotationXy2);
		}
		else if(yPivot)
		{
			if(rotationXy2 > PI * 0.2)
				rotationXy2 += speed * 3 * deltaTime;
			else
				rotationXy2 += speed * deltaTime;

			D3DXMatrixRotationY(&rotMatrix, rotationXy2);
		}
		else
		{
			if(rotationX < -(PI * 0.2))
				rotationX -= speed * 3 * deltaTime;
			else
				rotationX -= speed * deltaTime;

			D3DXMatrixRotationX(&rotMatrix, rotationX);
		}
	}
	else if(right && moving)
	{
		if(yPivot)
		{
			if(rotationY < -(PI * 0.2))
				rotationY -= speed * 3 * deltaTime;
			else
				rotationY -= speed * deltaTime;

			D3DXMatrixRotationY(&rotMatrix, rotationY);
		}
		else
		{
			if(rotationZ < -(PI * 0.2))
				rotationZ -= speed * 3 * deltaTime;
			else
				rotationZ -= speed * deltaTime;

			D3DXMatrixRotationZ(&rotMatrix, rotationZ);
		}
	}
	else if(left && moving)
	{
		if(yPivot)
		{
			if(rotationY > PI * 0.2)
				rotationY += speed * 3 * deltaTime;
			else
				rotationY += speed * deltaTime;

			D3DXMatrixRotationY(&rotMatrix, rotationY);
		}
		else
		{
			if(rotationZ > PI * 0.2)
				rotationZ += speed * 3 * deltaTime;
			else
				rotationZ += speed * deltaTime;

			D3DXMatrixRotationZ(&rotMatrix, rotationZ);
		}
	}

	D3DXMatrixTranslation(&trans1, pivot.x, pivot.y, pivot.z);
	D3DXMatrixTranslation(&trans2, pivot.x*-1, pivot.y*-1, pivot.z*-1);

	cubeMatrix = totalRot * trans1 * rotMatrix * trans2 * totalTrans;

	colDelay += gDeltaTime;

	if(colliding)
	{
		particlePos += gDeltaTime * particleVec * 2.0f;

		particleTimerMove = true;
		
		gParticleSizeMove = D3DXVECTOR2(0.2f, 0.2f);
		gParticleEmitPosMove = particlePos;
	}

	stuckInLoop = loopStuck();
	if(colliding && colDelay >= 0.5f && !stuckInLoop)
	{
		loopStart.push_back(D3DXVECTOR3(totalTrans._41, totalTrans._42, totalTrans._43));
		colliding = checkSideCollisions();
		particleVec = D3DXVECTOR3(totalTrans._41, totalTrans._42, totalTrans._43) - loopStart[loopStart.size() - 1];
		particlePos = loopStart[loopStart.size() - 1];

		//1 = mot egen bot, 2 = mot egen top, 3 = mot egen fram, 4 = mot egen bak, 5 = mot egen vänster, 6 = mot egen höger

		if(colliding == 1)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		}
		else if(colliding == 2)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 1.0f, 1.0f);
		}
		else if(colliding == 3)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		}
		else if(colliding == 4)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		}
		else if(colliding == 5)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		}
		else if(colliding == 6)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		colDelay = 0.0f;
		if(this->coinCollision())
		{
			
		}
		else if(this->puzzlePieceCollision())
		{
		
		}


	}
	if(!colliding && colDelay == 0.0f || stuckInLoop)
	{
		colliding = false;
		loopStart.clear();
		updateControles();
	}

	if(rotationX > PI * 0.5 * rotFactor || rotationXz > PI * 0.5 * rotFactor || (rotationXy < -(PI * 0.5f * rotFactor) && !invRot) || (rotationXy > PI * 0.5f * rotFactor && invRot))
	{
		playAudio();

		if(zPivot)
		{
			D3DXMatrixRotationZ(&rotMatrix, PI * 0.5f * rotFactor);
		}
		else if(yPivot && !invRot)
		{
			D3DXMatrixRotationY(&rotMatrix, -(PI * 0.5f * rotFactor));
		}
		else if(yPivot)
		{
			D3DXMatrixRotationY(&rotMatrix, PI * 0.5f * rotFactor);
		}
		else
		{
			D3DXMatrixRotationX(&rotMatrix, PI * 0.5f * rotFactor);
		}

		totalRot *= rotMatrix;
		rotationX = 0;
		rotationXz = 0;
		rotationXy = 0;
		moving = false;
		totalTrans._41 = floorf(cubeMatrix._41 + 0.5f);
		totalTrans._42 = floorf(cubeMatrix._42 + 0.5f);
		totalTrans._43 = floorf(cubeMatrix._43 + 0.5f);
		forward = false;
		speed = orgSpeed;
		rotDir = 1;
		for(int i = 0; i < rotFactor; i++)
		{
			if(zPivot)
			{
				switchSidesZNeg();
			}
			else if(yPivot && !invRot)
			{
				switchSidesYNeg();
			}
			else if(yPivot)
			{
				switchSidesYPos();
			}
			else
			{
				switchSidesXPos(); // front
			}
		}
		rotFactor = 1;
		zPivot = false;
		yPivot = false;
		invRot = false;

		loopStart.push_back(D3DXVECTOR3(totalTrans._41, totalTrans._42, totalTrans._43));

		checkForError();
		colliding = checkSideCollisions();
		particleVec = D3DXVECTOR3(totalTrans._41, totalTrans._42, totalTrans._43) - loopStart[loopStart.size() - 1];
		particlePos = loopStart[loopStart.size() - 1];
		colDelay = 0.0f;

		if(colliding == 1)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		}
		else if(colliding == 2)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 1.0f, 1.0f);
		}
		else if(colliding == 3)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		}
		else if(colliding == 4)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		}
		else if(colliding == 5)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		}
		else if(colliding == 6)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		if(this->coinCollision())
		{
			
		}
		else if(this->puzzlePieceCollision())
		{

		}

		updateControles();
	}
	else if(rotationX < -(PI * 0.5 * rotFactor) || rotationXz < -(PI * 0.5 * rotFactor) || rotationXy2 > PI * 0.5 * rotFactor || (rotationXy2 < -(PI * 0.5 * rotFactor) && invRot) || (rotationXy2 > PI * 0.5 * rotFactor && !invRot))
	{
		playAudio();

		if(zPivot)
		{
			D3DXMatrixRotationZ(&rotMatrix, -(PI * 0.5f * rotFactor));
		}
		else if(yPivot && !invRot)
		{
			D3DXMatrixRotationY(&rotMatrix, PI * 0.5f * rotFactor);
		}
		else if(yPivot)
		{
			D3DXMatrixRotationY(&rotMatrix, -(PI * 0.5f * rotFactor));
		}
		else
		{
			D3DXMatrixRotationX(&rotMatrix, -(PI * 0.5f * rotFactor));
		}
		totalRot *= rotMatrix;
		rotationX = 0;
		rotationXz = 0;
		rotationXy2 = 0;
		moving = false;
		totalTrans._41 = floorf(cubeMatrix._41 + 0.5f);
		totalTrans._42 = floorf(cubeMatrix._42 + 0.5f);
		totalTrans._43 = floorf(cubeMatrix._43 + 0.5f);
		backwards = false;
		speed = orgSpeed;
		rotDir = 1;
		for(int i = 0; i < rotFactor; i++)
		{
			if(zPivot)
			{
				switchSidesZPos();
			}
			else if(yPivot && !invRot)
			{
				switchSidesYPos();
			}
			else if(yPivot)
			{
				switchSidesYNeg();
			}
			else
			{
				switchSidesXNeg();
			}
		}
		rotFactor = 1;
		zPivot = false;
		yPivot = false;
		invRot = false;

		loopStart.push_back(D3DXVECTOR3(totalTrans._41, totalTrans._42, totalTrans._43));

		checkForError();
		colliding = checkSideCollisions();
		particleVec = D3DXVECTOR3(totalTrans._41, totalTrans._42, totalTrans._43) - loopStart[loopStart.size() - 1];
		particlePos = loopStart[loopStart.size() - 1];
		colDelay = 0.0f;

		if(colliding == 1)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		}
		else if(colliding == 2)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 1.0f, 1.0f);
		}
		else if(colliding == 3)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		}
		else if(colliding == 4)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		}
		else if(colliding == 5)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		}
		else if(colliding == 6)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
		if(this->coinCollision())
		{
			
		}
		else if(this->puzzlePieceCollision())
		{

		}

		updateControles();
		
	}
	else if(rotationZ > PI * 0.5 * rotFactor || rotationY > PI * 0.5 * rotFactor)
	{
		playAudio();

		if(yPivot)
		{
			D3DXMatrixRotationY(&rotMatrix, PI * 0.5f * rotFactor);
		}
		else
		{
			D3DXMatrixRotationZ(&rotMatrix, PI * 0.5f * rotFactor);
		}
		totalRot *= rotMatrix;
		rotationZ = 0;
		rotationY = 0;
		moving = false;
		totalTrans._41 = floorf(cubeMatrix._41 + 0.5f);
		totalTrans._42 = floorf(cubeMatrix._42 + 0.5f);
		totalTrans._43 = floorf(cubeMatrix._43 + 0.5f);
		left = false;
		speed = orgSpeed;
		rotDir = 1;
		for(int i = 0; i < rotFactor; i++)
		{
			if(yPivot)
			{
				switchSidesYPos();
			}
			else
			{
				switchSidesZNeg();
			}
		}
		rotFactor = 1;
		yPivot = false;

		loopStart.push_back(D3DXVECTOR3(totalTrans._41, totalTrans._42, totalTrans._43));

		checkForError();
		colliding = checkSideCollisions();
		particleVec = D3DXVECTOR3(totalTrans._41, totalTrans._42, totalTrans._43) - loopStart[loopStart.size() - 1];
		particlePos = loopStart[loopStart.size() - 1];
		colDelay = 0.0f;

		if(colliding == 1)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		}
		else if(colliding == 2)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 1.0f, 1.0f);
		}
		else if(colliding == 3)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		}
		else if(colliding == 4)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		}
		else if(colliding == 5)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		}
		else if(colliding == 6)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
		if(this->coinCollision())
		{
			
		}
		else if(this->puzzlePieceCollision())
		{

		}

		updateControles();

	}
	else if(rotationZ < -(PI * 0.5f * rotFactor) || rotationY < -(PI * 0.5f * rotFactor))
	{
		playAudio();

		if(yPivot)
		{
			D3DXMatrixRotationY(&rotMatrix, -(PI * 0.5f * rotFactor));
		}
		else
		{
			D3DXMatrixRotationZ(&rotMatrix, -(PI * 0.5f * rotFactor));
		}
		totalRot *= rotMatrix;
		rotationZ = 0;
		rotationY = 0;
		moving = false;
		totalTrans._41 = floorf(cubeMatrix._41 + 0.5f);
		totalTrans._42 = floorf(cubeMatrix._42 + 0.5f);
		totalTrans._43 = floorf(cubeMatrix._43 + 0.5f);
		right = false;
		speed = orgSpeed;
		rotDir = 1;
		for(int i = 0; i < rotFactor; i++)
		{
			if(yPivot)
			{
				switchSidesYNeg();
			}
			else
			{
				switchSidesZPos();
			}
		}
		rotFactor = 1;
		yPivot = false;

		loopStart.push_back(D3DXVECTOR3(totalTrans._41, totalTrans._42, totalTrans._43));

		checkForError();
		colliding = checkSideCollisions();
		particleVec = D3DXVECTOR3(totalTrans._41, totalTrans._42, totalTrans._43) - loopStart[loopStart.size() - 1];
		particlePos = loopStart[loopStart.size() - 1];
		colDelay = 0.0f;

		if(colliding == 1)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		}
		else if(colliding == 2)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 1.0f, 1.0f);
		}
		else if(colliding == 3)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		}
		else if(colliding == 4)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		}
		else if(colliding == 5)
		{
			gParticleColorMove = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		}
		else if(colliding == 6)
		{
			gParticleColorMove = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
		if(this->coinCollision())
		{
			
		}
		else if(this->puzzlePieceCollision())
		{

		}

		updateControles();
	}

	return cubeMatrix;
}

void Cube::checkForError()
{
	if((int)totalTrans._41 % 4 == 1)
		totalTrans._41--;
	else if((int)totalTrans._41 % 4 == 3)
		totalTrans._41++;

	if((int)totalTrans._42 % 4 == 1)
		totalTrans._42--;
	else if((int)totalTrans._42 % 4 == 3)
		totalTrans._42++;

	if((int)totalTrans._43 % 4 == 1)
		totalTrans._43--;
	else if((int)totalTrans._43 % 4 == 3)
		totalTrans._43++;
}

int Cube::moveForwardCollision()
{
	bool frontFront = this->frontFrontCollision();
	bool bot = this->botCollision();
	bool top = this->topCollision();
	bool frontMid = this->frontMidCollision();
	bool frontTop = this->frontTopCollision();
	bool frontBot = this->frontBotCollision();
	bool rightMid = this->rightMidCollision();
	bool rightTop = this->rightTopCollision();
	bool rightBot = this->rightBotCollision();
	bool leftMid = this->leftMidCollision();
	bool leftTop = this->leftTopCollision();
	bool leftBot = this->leftBotCollision();
	bool backBack = this->backBackCollision();
	bool backMid = this->backMidCollision();
	bool backTop = this->backTopCollision();
	bool backBot = this->backBotCollision();
	bool backRight = this->backRightCollision();
	bool backLeft = this->backLeftCollision();
	bool frontRight = this->frontRightCollision();
	bool frontLeft = this->frontLeftCollision();
	bool frontFrontBot = this->frontFrontBotCollision();
	bool frontFrontTop = this->frontFrontTopCollision();
	bool backBackBot = this->backBackBotCollision();
	bool backBackTop = this->backBackTopCollision();
	bool rightRightBot = this->rightRightBotCollision();
	bool rightRightTop = this->rightRightTopCollision();
	bool leftLeftBot = this->leftLeftBotCollision();
	bool leftLeftTop = this->leftLeftTopCollision();
	bool botBot = this->botBotCollision();
	bool topTop = this->topTopCollision();
	bool backBotBot = this->backBotBotCollision();
	bool rightBotBot = this->rightBotBotCollision();
	bool frontBotBot = this->frontBotBotCollision();
	bool leftTopTop = this->leftTopTopCollision();
	bool frontTopTop = this->frontTopTopCollision();

	if(top)
	{
		if(backBot && !backRight && !backLeft && backMid) // från tak till vägg
		{
			return 8;
		}
		else if(!backTop && !backBackTop && !rightTop && !leftTop && !backBot && !backBack && !bot)
		{
			return 11;
		}
		else if(frontMid && backTop) // från vägg till upp tak
		{
			return 5;
		}
		else if(backMid && !backBot && !backLeft && !backRight)
		{
			return 14;
		}
		else if(backTop && !backMid && !backBot) // gå vidare i tak
		{
			return 7;
		}
		//else if(!backTop && !backMid && !leftTop && !rightTop && !backBack && !backBackTop)
		//{
		//	return 10;
		//}
		else if(!bot && backBot && !frontBot && backMid && !backRight && !backLeft) //gå på vägg ner
		{
			return 20;
		}
	}

	if(frontMid && !frontTop && !frontLeft && !frontRight && !top && !topTop && !backMid && !backTop && !frontTopTop) //gå från vägg till platå
	{
		return 2;
	}
	else if(frontMid && !top && frontTop && !backMid && !backTop && !frontLeft && !frontRight && bot) //fram vägg från golv
	{
		return 3;
	}
	else if(frontMid && !top && frontTop && !backMid && !backTop && !bot) //vidare vägg
	{
		return 19;
	}
	else if(!bot && backBot && !frontBot && backMid && botBot) //gå på vägg ner till mark
	{
		return 22;
	}
	else if(!bot && backBot && !frontBot && backMid && !top) //gå på vägg ner
	{
		return 6;
	}
	else if(backMid && !backRight && !backLeft && !bot && !backBot && !botBot && !backBotBot && !frontMid) //frän vägg ner till tak
	{
		return 9;
	}
	//else if(!frontBot && !frontMid && !leftBot && !rightBot && bot && !frontFrontBot && frontBotBot && !top) // gå ner från sak till mark
	//{
	//	return 24;
	//}
	else if(!frontBot && !frontMid && !leftBot && !rightBot && bot && !frontFrontBot && !frontTop && !frontFront && !top) // gå ner från sak
	{
		return 4;
	}
	else if(frontBot && !frontMid && !frontTop && bot && !top) //från marken gå framåt
	{
		return 1;
	}
	else if(!top && topTop && leftTop && leftMid)
	{
		return 18;
	}
	else if(leftMid && leftTop && !top && !bot && !rightMid && !rightTop)
	{
		return 12;
	}
	else if(leftMid && !leftTop && !backLeft && !frontLeft && !bot && !topTop && !leftTopTop && !rightMid)
	{
		return 13;
	}
	else if(rightMid && !bot && botBot && rightBot)
	{
		return 17;
	}
	else if(!bot && rightBot && rightMid && !leftMid && !leftBot)
	{
		return 15;
	}
	else if(rightMid && !frontRight && !backRight && !bot && !rightBot && !botBot && !rightBot && !rightBotBot && !leftMid) //frän vägg ner till tak
	{
		return 16;
	}
	//else if(leftMid && frontMid && !bot && !top && !backMid && !rightMid && !backRight && backLeft)
	//{
	//	return 23;
	//}
	else if(frontMid && rightMid && !leftMid && frontLeft && !backLeft && !backMid)
	{
		return 25;
	}
	else if(rightMid && backMid && !leftMid && !frontMid && !frontLeft && backLeft)
	{
		return 26;
	}

	return 0;
}
int Cube::moveBackwardsCollision()
{
	bool frontFront = this->frontFrontCollision();
	bool bot = this->botCollision();
	bool top = this->topCollision();
	bool frontMid = this->frontMidCollision();
	bool frontTop = this->frontTopCollision();
	bool frontBot = this->frontBotCollision();
	bool rightMid = this->rightMidCollision();
	bool rightTop = this->rightTopCollision();
	bool rightBot = this->rightBotCollision();
	bool leftMid = this->leftMidCollision();
	bool leftTop = this->leftTopCollision();
	bool leftBot = this->leftBotCollision();
	bool backBack = this->backBackCollision();
	bool backMid = this->backMidCollision();
	bool backTop = this->backTopCollision();
	bool backBot = this->backBotCollision();
	bool backRight = this->backRightCollision();
	bool backLeft = this->backLeftCollision();
	bool frontRight = this->frontRightCollision();
	bool frontLeft = this->frontLeftCollision();
	bool frontFrontBot = this->frontFrontBotCollision();
	bool frontFrontTop = this->frontFrontTopCollision();
	bool backBackBot = this->backBackBotCollision();
	bool backBackTop = this->backBackTopCollision();
	bool rightRightBot = this->rightRightBotCollision();
	bool rightRightTop = this->rightRightTopCollision();
	bool leftLeftBot = this->leftLeftBotCollision();
	bool leftLeftTop = this->leftLeftTopCollision();
	bool botBot = this->botBotCollision();
	bool topTop = this->topTopCollision();
	bool frontBotBot = this->frontBotBotCollision();
	bool leftBotBot = this->leftBotBotCollision();
	bool rightTopTop = this->rightTopTopCollision();
	bool backTopTop = this->backTopTopCollision();


	if(top)
	{
		if(frontBot && !frontLeft && !frontRight && frontMid) // från tak till vägg
		{
			return 8;
		}
		else if(!frontTop && !frontFrontTop && !rightTop && !leftTop && !frontBot && !frontFront && !bot)
		{
			return 11;
		}
		else if(backMid && frontTop) // från vägg till upp tak
		{
			return 5;
		}
		else if(frontMid && !frontBot && !frontLeft && !frontRight)
		{
			return 14;
		}
		else if(frontTop && !frontMid && !frontBot) // gå vidare i tak
		{
			return 7;
		}
		//else if(!frontTop && !frontMid && !rightTop && !leftTop && !frontFront && !frontFrontTop)
		//{
		//	return 10;
		//}
		else if(!bot && frontBot && !backBot && frontMid && !frontRight && !frontLeft) //gå på vägg ner
		{
			return 20;
		}
	}

	if(backMid && !backTop && !backLeft && !backRight && !top && !topTop && !frontMid && !frontTop && !backTopTop) //gå från vägg till platå
	{
		return 2;
	}
	else if(backMid && !top && backTop && !frontMid && !frontTop && !backLeft && !backRight && bot) //fram vägg från golv
	{
		return 3;
	}
	else if(backMid && !top && backTop && !frontMid && !frontTop && !bot) //fram vägg från golv
	{
		return 19;
	}
	else if(!bot && frontBot && !backBot && frontMid && botBot) //gå på vägg ner till mark
	{
		return 22;
	}
	else if(!bot && frontBot && !backBot && frontMid && !top) //gå på vägg ner
	{
		return 6;
	}
	else if(frontMid && !frontRight && !frontLeft && !bot && !frontBot && !botBot && !frontBotBot && !backMid)  //frän vägg ner till tak
	{
		return 9;
	}
	else if(!backBot && !backMid && !leftBot && !rightBot && bot && !backBackBot && !backTop && !backBack && !top) // gå ner från sak
	{
		return 4;
	}
	else if(backBot && !backMid && !backTop && bot && !top) //från marken gå framåt
	{
		return 1;
	}
	else if(!bot && leftBot && !botBot && !rightMid && !rightBot) //gå på vägg ner 
	{
		return 12;
	}
	else if(leftMid && !leftBot && !backLeft && !frontLeft && !botBot && !leftBotBot && !rightMid)
	{
		return 13;
	}
	else if(leftMid && !bot && botBot && leftBot)
	{
		return 17;
	}
	else if(!top && topTop && rightTop && rightMid)
	{
		return 18;
	}
	else if(rightMid && rightTop && !top && !bot && !leftTop)
	{
		return 15;
	}
	else if(rightMid && !bot && !rightTop && !frontRight && !backRight && !topTop && !rightTopTop && !leftMid)
	{
		return 16;
	}
	else if(leftMid && backMid && !bot && !top && !frontMid && !rightMid && !frontRight && backRight)
	{
		return 21;
	}
	else if(leftMid && frontMid && !bot && !top && !backMid && !rightMid && !backRight && frontRight)
	{
		return 23;
	}

	return 0;
}
int Cube::moveRightCollision()
{
	bool frontFront = this->frontFrontCollision();
	bool bot = this->botCollision();
	bool top = this->topCollision();
	bool frontMid = this->frontMidCollision();
	bool frontTop = this->frontTopCollision();
	bool frontBot = this->frontBotCollision();
	bool rightMid = this->rightMidCollision();
	bool rightTop = this->rightTopCollision();
	bool rightBot = this->rightBotCollision();
	bool leftMid = this->leftMidCollision();
	bool leftTop = this->leftTopCollision();
	bool leftBot = this->leftBotCollision();
	bool backBack = this->backBackCollision();
	bool backMid = this->backMidCollision();
	bool backTop = this->backTopCollision();
	bool backBot = this->backBotCollision();
	bool backRight = this->backRightCollision();
	bool backLeft = this->backLeftCollision();
	bool frontRight = this->frontRightCollision();
	bool frontLeft = this->frontLeftCollision();
	bool frontFrontBot = this->frontFrontBotCollision();
	bool frontFrontTop = this->frontFrontTopCollision();
	bool backBackBot = this->backBackBotCollision();
	bool backBackTop = this->backBackTopCollision();
	bool rightRightBot = this->rightRightBotCollision();
	bool rightRightTop = this->rightRightTopCollision();
	bool leftLeftBot = this->leftLeftBotCollision();
	bool leftLeftTop = this->leftLeftTopCollision();
	bool rightRightFront = this->rightRightFrontCollision();
	bool rightRightBack = this->rightRightBackCollision();
	bool leftLeftFront = this->leftLeftFrontCollision();
	bool leftLeftBack = this->leftLeftBackCollision();
	bool backBackLeft = this->backBackLeftCollision();
	bool backBackRight = this->backBackRightCollision();
	bool frontFrontLeft = this->frontFrontLeftCollision();
	bool frontFrontRight = this->frontFrontRightCollision();
	bool topTop = this->topTopCollision();
	bool botBot = this->botBotCollision();
	bool rightRight = this->rightRightCollision();
	bool rightBotBot = this->rightBotBotCollision();
	bool leftLeft = this->leftLeftCollision();
	bool rightTopTop = this->rightTopTopCollision();


	if(rightMid && bot && topTop && !top && rightTop && !frontRight && !backRight)
	{
		return 24;
	}
	if(top)
	{
		if(leftMid && leftBot && !botBot && !frontLeft && !backLeft)
		{
			return 28;
		}
		else if(leftBot && !botBot && !frontLeft && !backLeft && !leftBot) // från tak till vägg
		{
			return 8;
		}
		else if(leftBot && !frontLeft && !backLeft && !leftBot)
		{
			return 25;
		}
		else if(!leftTop && !leftLeftTop && !frontTop && !backTop && !leftMid && !leftBot && !leftLeft && !bot)
		{
			return 11;
		}
		else if(rightMid && leftTop) // från vägg till upp tak
		{
			return 5;
		}
		else if(leftMid && !backLeft && !frontLeft)
		{
			return 21;
		}
		else if(leftTop && !leftMid && !leftBot) // gå vidare i tak
		{
			return 7;
		}
	}

	if(leftMid && frontLeft && !frontMid && !bot && !frontRight && !backMid && !top)
	{
		return 14;
	}
	else if(bot && !rightBot && !backBot && !frontBot && !rightMid && !rightRight && rightBotBot && !rightRightBot)
	{
		return 27;
	}
	else if(!rightBot && !rightMid && !frontBot && !backBot && bot && !rightRightBot && !rightTop && !top) // gå ner från sak
	{
		return 4;
	}
	else if(rightBot && !rightMid && !rightTop && bot) //från marken gå framåt
	{
		return 1;
	}
	else if(frontMid && !frontTop && !frontBot && !rightMid && !frontRight && !rightRightFront && !backRight && !rightRight && !backMid)
	{
		return 13;
	}
	else if(frontMid && !rightMid && frontRight && !leftMid && !bot && rightRight)
	{
		return 32;
	}
	else if(frontMid && !rightMid && frontRight && !leftMid && !backMid)
	{
		return 12;
	}
	else if(leftMid && !frontLeft && !leftTop && !leftBot && !frontFrontLeft && !frontFront && !backRight && !rightMid)
	{
		return 15;
	}
	else if(backMid && !leftMid && !backTop && !backBot && !backLeft && !leftLeftBack && !frontLeft && !leftLeft && !frontMid)
	{
		return 16;
	}
	else if(rightMid && !backRight && !bot && !rightTop && !rightBot && !backBackRight && !leftMid && !backMid && !backBack && !backLeft)
	{
		return 17;
	}
	//else if(rightMid && !bot && backRight && !backMid && !backLeft && frontMid && !rightTop && !rightBot)
	//{
	//	return 31;
	//}
	else if(rightMid && !bot && backRight && !backMid && !backLeft && !frontMid)
	{
		return 18;
	}
	else if(backMid && !leftMid && backLeft && !rightMid && !bot && leftLeft)
	{
		return 35;
	}
	else if(backMid && !leftMid && backLeft && !bot && !frontMid)
	{
		return 19;
	}
	//else if(rightMid && !rightTop && !rightBot && !backMid && !backRight && !backBackRight && !leftMid && !backBack)
	//{
	//	return 20;
	//}
	else if(rightMid && !frontRight && !backRight && bot && !rightTop && !leftMid && !leftTop && !rightTopTop)
	{
		return 22;
	}
	else if(rightMid && !frontRight && !backRight && bot && rightTop && !leftMid && !leftTop)
	{
		return 23;
	}
	else if(frontMid && rightMid && !leftMid && backRight && !backLeft && !backMid)
	{
		return 33;
	}
	else if(leftMid && backMid && !bot && !top && !frontMid && !rightMid && !frontRight && frontLeft)
	{
		return 36;
	}


	return 0;
}
int Cube::moveLeftCollision()
{
	bool frontFront = this->frontFrontCollision();
	bool bot = this->botCollision();
	bool top = this->topCollision();
	bool frontMid = this->frontMidCollision();
	bool frontTop = this->frontTopCollision();
	bool frontBot = this->frontBotCollision();
	bool rightMid = this->rightMidCollision();
	bool rightTop = this->rightTopCollision();
	bool rightBot = this->rightBotCollision();
	bool leftMid = this->leftMidCollision();
	bool leftTop = this->leftTopCollision();
	bool leftBot = this->leftBotCollision();
	bool backBack = this->backBackCollision();
	bool backMid = this->backMidCollision();
	bool backTop = this->backTopCollision();
	bool backBot = this->backBotCollision();
	bool backRight = this->backRightCollision();
	bool backLeft = this->backLeftCollision();
	bool frontRight = this->frontRightCollision();
	bool frontLeft = this->frontLeftCollision();
	bool frontFrontBot = this->frontFrontBotCollision();
	bool frontFrontTop = this->frontFrontTopCollision();
	bool backBackBot = this->backBackBotCollision();
	bool backBackTop = this->backBackTopCollision();
	bool rightRightBot = this->rightRightBotCollision();
	bool rightRightTop = this->rightRightTopCollision();
	bool leftLeftBot = this->leftLeftBotCollision();
	bool leftLeftTop = this->leftLeftTopCollision();
	bool rightRightFront = this->rightRightFrontCollision();
	bool rightRightBack = this->rightRightBackCollision();
	bool leftLeftFront = this->leftLeftFrontCollision();
	bool leftLeftBack = this->leftLeftBackCollision();
	bool backBackLeft = this->backBackLeftCollision();
	bool backBackRight = this->backBackRightCollision();
	bool frontFrontLeft = this->frontFrontLeftCollision();
	bool frontFrontRight = this->frontFrontRightCollision();
	bool topTop = this->topTopCollision();
	bool leftLeft = this->leftLeftCollision();
	bool leftBotBot = this->leftBotBotCollision();
	bool rightRight = this->rightRightCollision();
	bool botBot = this->botBotCollision();
	bool leftTopTop = this->leftTopTopCollision();

	if(leftMid && bot && topTop && !top && leftTop && !backLeft && !frontLeft)
	{
		return 25;
	}
	if(top)
	{
		if(rightMid && rightBot && !botBot && !frontRight && !backRight)
		{
			return 28;
		}
		else if(rightBot && !frontRight && !backRight && rightMid) // från tak till vägg
		{
			return 8;
		}
		else if(!rightTop && !rightRightTop && !frontTop && !backTop && !rightMid && !leftBot && !rightRight && !bot)
		{
			return 11;
		}
		else if(leftMid && rightTop) // från vägg till upp tak
		{
			return 5;
		}
		else if(rightMid && !backRight && !frontRight)
		{
			return 21;
		}
		else if(rightTop && !rightMid && !rightBot) // gå vidare i tak
		{
			return 7;
		}

	}

	if(bot && !leftBot && !backBot && !frontBot && !leftMid && !leftLeft && leftBotBot && !leftLeftBot)
	{
		return 27;
	}
	else if(!leftBot && !leftMid && !frontBot && !backBot && bot && !leftLeftBot && !leftTop && !top) // gå ner från sak
	{
		return 4;
	}
	else if(leftBot && !leftMid && !leftTop && bot) //från marken gå framåt
	{
		return 1;
	}
	else if(frontMid && !frontTop && !frontBot && !leftMid && !frontLeft && !leftLeftFront && !backLeft && !leftLeft && !backMid)
	{
		return 13;
	}
	else if(frontMid && !leftMid && frontLeft && !rightMid && !bot && leftLeft)
	{
		return 33;
	}
	else if(frontMid && !leftMid && frontLeft && !rightMid && !backMid)
	{
		return 12;//
	}
	else if(rightMid && frontRight && !frontMid && !frontLeft && !top)
	{
		return 14;
	}
	else if(leftMid && !backMid && !bot && backLeft && !backRight && frontMid && !leftTop && !leftBot)
	{
		return 31;
	}
	else if(leftMid && !backMid && !bot && backLeft && !backRight && !frontMid)
	{
		return 15;
	}
	else if(leftMid && !backLeft && !bot && !leftTop && !leftBot && !backBackLeft && !rightMid && !backRight && !backBack)
	{
		return 16;
	}
	else if(backMid && !bot && !backRight && !backTop && !backBot && !rightRightBack && !frontRight && !rightRight && !rightMid && !frontMid)
	{
		return 17;
	}
	else if(rightMid && !rightTop && !rightBot && !frontRight && !frontMid && !frontFront && !frontLeft && !leftMid)
	{
		return 18;
	}
	else if(backMid && !rightMid && backRight && !leftMid && rightRight)
	{
		return 35;
	}
	else if(backMid && !rightMid && backRight && !leftMid && !frontMid)
	{
		return 19;
	}
	//else if(leftMid && !backMid && !leftTop && !leftBot && !backBackLeft && !rightMid)
	//{
	//	return 20;
	//}
	else if(leftMid && !frontLeft && !backLeft && bot && !leftTop && !rightMid && !rightTop && !leftTopTop)
	{
		return 22;
	}
	else if(leftMid && !frontLeft && !backLeft && bot && leftTop && !rightMid && !rightTop)
	{
		return 23;
	}
	//else if(rightMid && !frontRight && !frontFrontRight && !frontMid && !rightTop && !rightBot)
	//{
	//	return 24;
	//}
	return 0;
}

int Cube::topSideCollision()
{	
	//läs ej ->
	if(this->sides.at(0)->getString() != "top") //topsidan är på botten
	{
		if(levelHandler->invTopBlockCollision(totalTrans._41, totalTrans._42 - 4, totalTrans._43))
			return 1;
	}
	if(this->sides.at(1)->getString() != "top") //topsidan är på toppen
	{
		if(levelHandler->invTopBlockCollision(totalTrans._41, totalTrans._42 + 4, totalTrans._43))
			return 2;
	}
	if(this->sides.at(2)->getString() != "top") //topsidan är fram
	{
		if(levelHandler->invTopBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 + 4))
			return 3;
	}
	if(this->sides.at(3)->getString() != "top") //topsidan är bak
	{
		if(levelHandler->invTopBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 - 4))
			return 4;
	}
	if(this->sides.at(4)->getString() != "top") //topsidan är till vänster
	{
		if(levelHandler->invTopBlockCollision(totalTrans._41 - 4, totalTrans._42, totalTrans._43))
			return 5;
	}
	if(this->sides.at(5)->getString() != "top") //topsidan är till höger
	{
		if(levelHandler->invTopBlockCollision(totalTrans._41 + 4, totalTrans._42, totalTrans._43))
			return 6;
	}


	if(this->sides.at(0)->getString() == "top") //topsidan är på botten
	{
		if(levelHandler->topBlockCollision(totalTrans._41, totalTrans._42 - 4, totalTrans._43))
			return 1;
	}
	else if(this->sides.at(1)->getString() == "top") //topsidan är på toppen
	{
		if(levelHandler->topBlockCollision(totalTrans._41, totalTrans._42 + 4, totalTrans._43))
			return 2;
	}
	else if(this->sides.at(2)->getString() == "top") //topsidan är fram
	{
		if(levelHandler->topBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 + 4))
			return 3;
	}
	else if(this->sides.at(3)->getString() == "top") //topsidan är bak
	{
		if(levelHandler->topBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 - 4))
			return 4;
	}
	else if(this->sides.at(4)->getString() == "top") //topsidan är till vänster
	{
		if(levelHandler->topBlockCollision(totalTrans._41 - 4, totalTrans._42, totalTrans._43))
			return 5;
	}
	else if(this->sides.at(5)->getString() == "top") //topsidan är till höger
	{
		if(levelHandler->topBlockCollision(totalTrans._41 + 4, totalTrans._42, totalTrans._43))
			return 6;
	}
	return false;
}

int Cube::botSideCollision()
{
	//läs ej->
	if(this->sides.at(0)->getString() != "bot") //botsidan är på botten
	{
		if(levelHandler->invBotBlockCollision(totalTrans._41, totalTrans._42 - 4, totalTrans._43))
			return 1;
	}
	if(this->sides.at(1)->getString() != "bot") //botsidan är på toppen
	{
		if(levelHandler->invBotBlockCollision(totalTrans._41, totalTrans._42 + 4, totalTrans._43))
			return 2;
	}
	if(this->sides.at(2)->getString() != "bot") //botsidan är fram
	{
		if(levelHandler->invBotBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 + 4))
			return 3;
	}
	if(this->sides.at(3)->getString() != "bot") //botsidan är bak
	{
		if(levelHandler->invBotBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 - 4))
			return 4;
	}
	if(this->sides.at(4)->getString() != "bot") //botsidan är till vänster
	{
		if(levelHandler->invBotBlockCollision(totalTrans._41 - 4, totalTrans._42, totalTrans._43))
			return 5;
	}
	if(this->sides.at(5)->getString() != "bot") //botsidan är till höger
	{
		if(levelHandler->invBotBlockCollision(totalTrans._41 + 4, totalTrans._42, totalTrans._43))
			return 6;
	}


	if(this->sides.at(0)->getString() == "bot") //botsidan är på botten
	{
		if(levelHandler->botBlockCollision(totalTrans._41, totalTrans._42 - 4, totalTrans._43))
			return 1;
	}
	else if(this->sides.at(1)->getString() == "bot") //botsidan är på toppen
	{
		if(levelHandler->botBlockCollision(totalTrans._41, totalTrans._42 + 4, totalTrans._43))
			return 2;
	}
	else if(this->sides.at(2)->getString() == "bot") //botsidan är fram
	{
		if(levelHandler->botBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 + 4))
			return 3;
	}
	else if(this->sides.at(3)->getString() == "bot") //botsidan är bak
	{
		if(levelHandler->botBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 - 4))
			return 4;
	}
	else if(this->sides.at(4)->getString() == "bot") //botsidan är till vänster
	{
		if(levelHandler->botBlockCollision(totalTrans._41 - 4, totalTrans._42, totalTrans._43))
			return 5;
	}
	else if(this->sides.at(5)->getString() == "bot") //botsidan är till höger
	{
		if(levelHandler->botBlockCollision(totalTrans._41 + 4, totalTrans._42, totalTrans._43))
			return 6;
	}
	return false;
}
int Cube::rightSideCollision()
{
	//läs ej->
	if(this->sides.at(0)->getString() != "right") //rightsidan är på botten
	{
		if(levelHandler->invRightBlockCollision(totalTrans._41, totalTrans._42 - 4, totalTrans._43))
			return 1;
	}
	if(this->sides.at(1)->getString() != "right") //rightsidan är på toppen
	{
		if(levelHandler->invRightBlockCollision(totalTrans._41, totalTrans._42 + 4, totalTrans._43))
			return 2;
	}
	if(this->sides.at(2)->getString() != "right") //rightsidan är fram
	{
		if(levelHandler->invRightBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 + 4))
			return 3;
	}
	if(this->sides.at(3)->getString() != "right") //rightsidan är bak
	{
		if(levelHandler->invRightBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 - 4))
			return 4;
	}
	if(this->sides.at(4)->getString() != "right") //rightsidan är till vänster
	{
		if(levelHandler->invRightBlockCollision(totalTrans._41 - 4, totalTrans._42, totalTrans._43))
			return 5;
	}
	if(this->sides.at(5)->getString() != "right") //rightsidan är till höger
	{
		if(levelHandler->invRightBlockCollision(totalTrans._41 + 4, totalTrans._42, totalTrans._43))
			return 6;
	}


	if(this->sides.at(0)->getString() == "right") //rightsidan är på botten
	{
		if(levelHandler->rightBlockCollision(totalTrans._41, totalTrans._42 - 4, totalTrans._43))
			return 1;
	}
	else if(this->sides.at(1)->getString() == "right") //rightsidan är på toppen
	{
		if(levelHandler->rightBlockCollision(totalTrans._41, totalTrans._42 + 4, totalTrans._43))
			return 2;
	}
	else if(this->sides.at(2)->getString() == "right") //rightsidan är fram
	{
		if(levelHandler->rightBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 + 4))
			return 3;
	}
	else if(this->sides.at(3)->getString() == "right") //rightsidan är bak
	{
		if(levelHandler->rightBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 - 4))
			return 4;
	}
	else if(this->sides.at(4)->getString() == "right") //rightsidan är till vänster
	{
		if(levelHandler->rightBlockCollision(totalTrans._41 - 4, totalTrans._42, totalTrans._43))
			return 5;
	}
	else if(this->sides.at(5)->getString() == "right") //rightsidan är till höger
	{
		if(levelHandler->rightBlockCollision(totalTrans._41 + 4, totalTrans._42, totalTrans._43))
			return 6;
	}
	return false;
}
int Cube::leftSideCollision()
{
	//läs ej->
	if(this->sides.at(0)->getString() != "left") //leftsidan är på botten
	{
		if(levelHandler->invLeftBlockCollision(totalTrans._41, totalTrans._42 - 4, totalTrans._43))
			return 1;
	}
	if(this->sides.at(1)->getString() != "left") //leftsidan är på toppen
	{
		if(levelHandler->invLeftBlockCollision(totalTrans._41, totalTrans._42 + 4, totalTrans._43))
			return 2;
	}
	if(this->sides.at(2)->getString() != "left") //leftsidan är fram
	{
		if(levelHandler->invLeftBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 + 4))
			return 3;
	}
	if(this->sides.at(3)->getString() != "left") //leftsidan är bak
	{
		if(levelHandler->invLeftBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 - 4))
			return 4;
	}
	if(this->sides.at(4)->getString() != "left") //leftsidan är till vänster
	{
		if(levelHandler->invLeftBlockCollision(totalTrans._41 - 4, totalTrans._42, totalTrans._43))
			return 5;
	}
	if(this->sides.at(5)->getString() != "left") //leftsidan är till höger
	{
		if(levelHandler->invLeftBlockCollision(totalTrans._41 + 4, totalTrans._42, totalTrans._43))
			return 6;
	}


	if(this->sides.at(0)->getString() == "left") //leftsidan är på botten
	{
		if(levelHandler->leftBlockCollision(totalTrans._41, totalTrans._42 - 4, totalTrans._43))
			return 1;
	}
	else if(this->sides.at(1)->getString() == "left") //leftsidan är på toppen
	{
		if(levelHandler->leftBlockCollision(totalTrans._41, totalTrans._42 + 4, totalTrans._43))
			return 2;
	}
	else if(this->sides.at(2)->getString() == "left") //leftsidan är fram
	{
		if(levelHandler->leftBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 + 4))
			return 3;
	}
	else if(this->sides.at(3)->getString() == "left") //leftsidan är bak
	{
		if(levelHandler->leftBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 - 4))
			return 4;
	}
	else if(this->sides.at(4)->getString() == "left") //leftsidan är till vänster
	{
		if(levelHandler->leftBlockCollision(totalTrans._41 - 4, totalTrans._42, totalTrans._43))
			return 5;
	}
	else if(this->sides.at(5)->getString() == "left") //leftsidan är till höger
	{
		if(levelHandler->leftBlockCollision(totalTrans._41 + 4, totalTrans._42, totalTrans._43))
			return 6;
	}
	return false;
}
int Cube::frontSideCollision()
{
	//läs ej->
	if(this->sides.at(0)->getString() != "front") //frontsidan är på botten
	{
		if(levelHandler->invFrontBlockCollision(totalTrans._41, totalTrans._42 - 4, totalTrans._43))
			return 1;
	}
	if(this->sides.at(1)->getString() != "front") //frontsidan är på toppen
	{
		if(levelHandler->invFrontBlockCollision(totalTrans._41, totalTrans._42 + 4, totalTrans._43))
			return 2;
	}
	if(this->sides.at(2)->getString() != "front") //frontsidan är fram
	{
		if(levelHandler->invFrontBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 + 4))
			return 3;
	}
	if(this->sides.at(3)->getString() != "front") //frontsidan är bak
	{
		if(levelHandler->invFrontBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 - 4))
			return 4;
	}
	if(this->sides.at(4)->getString() != "front") //frontsidan är till vänster
	{
		if(levelHandler->invFrontBlockCollision(totalTrans._41 - 4, totalTrans._42, totalTrans._43))
			return 5;
	}
	if(this->sides.at(5)->getString() != "front") //frontsidan är till höger
	{
		if(levelHandler->invFrontBlockCollision(totalTrans._41 + 4, totalTrans._42, totalTrans._43))
			return 6;
	}

	if(this->sides.at(0)->getString() == "front") //frontsidan är på botten
	{
		if(levelHandler->frontBlockCollision(totalTrans._41, totalTrans._42 - 4, totalTrans._43))
			return 1;
	}
	else if(this->sides.at(1)->getString() == "front") //frontsidan är på toppen
	{
		if(levelHandler->frontBlockCollision(totalTrans._41, totalTrans._42 + 4, totalTrans._43))
			return 2;
	}
	else if(this->sides.at(2)->getString() == "front") //frontsidan är fram
	{
		if(levelHandler->frontBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 + 4))
			return 3;
	}
	else if(this->sides.at(3)->getString() == "front") //frontsidan är bak
	{
		if(levelHandler->frontBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 - 4))
			return 4;
	}
	else if(this->sides.at(4)->getString() == "front") //frontsidan är till vänster
	{
		if(levelHandler->frontBlockCollision(totalTrans._41 - 4, totalTrans._42, totalTrans._43))
			return 5;
	}
	else if(this->sides.at(5)->getString() == "front") //frontsidan är till höger
	{
		if(levelHandler->frontBlockCollision(totalTrans._41 + 4, totalTrans._42, totalTrans._43))
			return 6;
	}
	return false;
}
int Cube::backSideCollision()
{
	//läs ej->
	if(this->sides.at(0)->getString() != "back") //backsidan är på botten
	{
		if(levelHandler->invBackBlockCollision(totalTrans._41, totalTrans._42 - 4, totalTrans._43))
			return 1;
	}
	if(this->sides.at(1)->getString() != "back") //backsidan är på toppen
	{
		if(levelHandler->invBackBlockCollision(totalTrans._41, totalTrans._42 + 4, totalTrans._43))
			return 2;
	}
	if(this->sides.at(2)->getString() != "back") //backsidan är fram
	{
		if(levelHandler->invBackBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 + 4))
			return 3;
	}
	if(this->sides.at(3)->getString() != "back") //backsidan är bak
	{
		if(levelHandler->invBackBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 - 4))
			return 4;
	}
	if(this->sides.at(4)->getString() != "back") //backsidan är till vänster
	{
		if(levelHandler->invBackBlockCollision(totalTrans._41 - 4, totalTrans._42, totalTrans._43))
			return 5;
	}
	if(this->sides.at(5)->getString() != "back") //backsidan är till höger
	{
		if(levelHandler->invBackBlockCollision(totalTrans._41 + 4, totalTrans._42, totalTrans._43))
			return 6;
	}


	if(this->sides.at(0)->getString() == "back") //backsidan är på botten
	{
		if(levelHandler->backBlockCollision(totalTrans._41, totalTrans._42 - 4, totalTrans._43))
			return 1;
	}
	else if(this->sides.at(1)->getString() == "back") //backsidan är på toppen
	{
		if(levelHandler->backBlockCollision(totalTrans._41, totalTrans._42 + 4, totalTrans._43))
			return 2;
	}
	else if(this->sides.at(2)->getString() == "back") //backsidan är fram
	{
		if(levelHandler->backBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 + 4))
			return 3;
	}
	else if(this->sides.at(3)->getString() == "back") //backsidan är bak
	{
		if(levelHandler->backBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43 - 4))
			return 4;
	}
	else if(this->sides.at(4)->getString() == "back") //backsidan är till vänster
	{
		if(levelHandler->backBlockCollision(totalTrans._41 - 4, totalTrans._42, totalTrans._43))
			return 5;
	}
	else if(this->sides.at(5)->getString() == "back") //backsidan är till höger
	{
		if(levelHandler->backBlockCollision(totalTrans._41 + 4, totalTrans._42, totalTrans._43))
			return 6;
	}
	return false;
}

int Cube::checkSideCollisions()
{
	//1 = mot egen bot, 2 = mot egen top, 3 = mot egen fram, 4 = mot egen bak, 5 = mot egen vänster, 6 = mot egen höger
	int frontSide = this->frontSideCollision();
	int backSide = this->backSideCollision();
	int leftSide = this->leftSideCollision();
	int rightSide = this->rightSideCollision();
	int topSide = this->topSideCollision();
	int botSide = this->botSideCollision();

	int color = 0;

	if(frontSide)
	{
		color = 1;
	}
	else if(backSide)
	{
		color = 2;
	}
	else if(leftSide)
	{
		color = 3;
	}
	else if(rightSide)
	{
		color = 4;
	}
	else if(topSide)
	{
		color = 5;
	}
	else if(botSide)
	{
		color = 6;
	}

	//bool collision = false;
	D3DXVECTOR3 tempTrans(0, 0, 0);
	int maxRange = 4;

	if(frontSide == 1)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, 0, i * 4)))
			{
				tempTrans.z += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(frontSide == 2)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, 0, i * 4)))
			{
				tempTrans.z += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(frontSide == 3)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 1 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, 0, i * 4)))
			{
				this->totalTrans._43 += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(frontSide == 4)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, 0, i * 4)))
			{
				this->totalTrans._43 += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(frontSide == 5)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, 0, i * 4)))
			{
				this->totalTrans._43 += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(frontSide == 6)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, 0, i * 4)))
			{
				this->totalTrans._43 += i * 4;
				stop = true;
			}
			i--;
		}
	}

	//1 = mot egen bot, 2 = mot egen top, 3 = mot egen fram, 4 = mot egen bak, 5 = mot egen vänster, 6 = mot egen höger
	else if(backSide == 1)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, 0, i * -4)))
			{
				tempTrans.z += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(backSide == 2)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, 0, i * -4)))
			{
				tempTrans.z += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(backSide == 3)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, 0, i * -4)))
			{
				tempTrans.z += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(backSide == 4)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 1 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, 0, i * -4)))
			{
				tempTrans.z += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(backSide == 5)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, 0, i * -4)))
			{
				tempTrans.z += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(backSide == 6)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, 0, i * -4)))
			{
				tempTrans.z += i * -4;
				stop = true;
			}
			i--;
		}
	}

	//1 = mot egen bot, 2 = mot egen top, 3 = mot egen fram, 4 = mot egen bak, 5 = mot egen vänster, 6 = mot egen höger
	else if(rightSide == 1)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(i * 4, 0, 0)))
			{
				tempTrans.x += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(rightSide == 2)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(i * 4, 0, 0)))
			{
				tempTrans.x += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(rightSide == 3)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(i * 4, 0, 0)))
			{
				tempTrans.x += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(rightSide == 4)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(i * 4, 0, 0)))
			{
				tempTrans.x += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(rightSide == 5)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(i * 4, 0, 0)))
			{
				tempTrans.x += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(rightSide == 6)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 1 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(i * 4, 0, 0)))
			{
				tempTrans.x += i * 4;
				stop = true;
			}
			i--;
		}
	}

	//1 = mot egen bot, 2 = mot egen top, 3 = mot egen fram, 4 = mot egen bak, 5 = mot egen vänster, 6 = mot egen höger
	else if(leftSide == 1)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(i * -4, 0, 0)))
			{
				tempTrans.x += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(leftSide == 2)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(i * -4, 0, 0)))
			{
				tempTrans.x += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(leftSide == 3)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(i * -4, 0, 0)))
			{
				tempTrans.x += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(leftSide == 4)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(i * -4, 0, 0)))
			{
				tempTrans.x += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(leftSide == 5)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 1 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(i * -4, 0, 0)))
			{
				tempTrans.x += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(leftSide == 6)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(i * -4, 0, 0)))
			{
				tempTrans.x += i * -4;
				stop = true;
			}
			i--;
		}
	}

	//1 = mot egen bot, 2 = mot egen top, 3 = mot egen fram, 4 = mot egen bak, 5 = mot egen vänster, 6 = mot egen höger
	else if(topSide == 1)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, i * 4, 0)))
			{
				tempTrans.y += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(topSide == 2)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 1 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, i * 4, 0)))
			{
				tempTrans.y += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(topSide == 3)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, i * 4, 0)))
			{
				tempTrans.y += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(topSide == 4)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, i * 4, 0)))
			{
				tempTrans.y += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(topSide == 5)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, i * 4, 0)))
			{
				tempTrans.y += i * 4;
				stop = true;
			}
			i--;
		}
	}
	else if(topSide == 6)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, i * 4, 0)))
			{
				tempTrans.y += i * 4;
				stop = true;
			}
			i--;
		}
	}

	//1 = mot egen bot, 2 = mot egen top, 3 = mot egen fram, 4 = mot egen bak, 5 = mot egen vänster, 6 = mot egen höger
	else if(botSide == 1)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 1 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, i * -4, 0)))
			{
				tempTrans.y += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(botSide == 2)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, i * -4, 0)))
			{
				tempTrans.y += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(botSide == 3)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, i * -4, 0)))
			{
				tempTrans.y += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(botSide == 4)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, i * -4, 0)))
			{
				tempTrans.y += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(botSide == 5)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, i * -4, 0)))
			{
				tempTrans.y += i * -4;
				stop = true;
			}
			i--;
		}
	}
	else if(botSide == 6)
	{
		int i = maxRange;
		bool stop = false;
		while(i > 0 && !stop)
		{
			if(this->jumpPossible(D3DXVECTOR3(0, i * -4, 0)))
			{
				tempTrans.y += i * -4;
				stop = true;
			}
			i--;
		}
	}

	this->totalTrans._41 += tempTrans.x;
	this->totalTrans._42 += tempTrans.y;
	this->totalTrans._43 += tempTrans.z;

	//if(tempTrans.x != 0 || tempTrans.y != 0 || tempTrans.z != 0)
		//collision = true;


	return color;
}

bool Cube::coinCollision()
{
	if(levelHandler->coinBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43))
	{
		SOUND* sfx = SOUNDMANAGER->PlaySound("../Bin/Sounds/coin.wav");
		particleTimer = true;
		gParticleColor = D3DXVECTOR3(1.0f, 0.6f, 0.0f);
		gParticleSize = D3DXVECTOR2(0.4f, 0.4f);
		gParticleEmitPos = D3DXVECTOR3(totalTrans._41, totalTrans._42, totalTrans._43);
		points += 1000;
		sfx->SetVolume(0.5f);
		return true;
	}

	return false;
}
bool Cube::puzzlePieceCollision()
{
	if(levelHandler->puzzleBlockCollision(totalTrans._41, totalTrans._42, totalTrans._43))
	{
		SOUND* sfx = SOUNDMANAGER->PlaySound("../Bin/Sounds/puzzle.wav");
		particleTimer = true;
		gParticleColor = D3DXVECTOR3(0.0f, 1.0f, 1.0f);
		gParticleSize = D3DXVECTOR2(0.7f, 0.7f);
		gParticleEmitPos = D3DXVECTOR3(totalTrans._41, totalTrans._42, totalTrans._43);
		sfx->SetVolume(4.0f);

		return true;
	}

	return false;
}

void Cube::switchSidesXPos()
{
	vector<string> temps;
	for(int i = 0; i < (int)xSideRot.size(); i++)
	{
		temps.push_back(xSideRot.at(i)->getString());
	}
	for(int i = (int)xSideRot.size() - 1; i >= 0; i--)
	{
		int tempInt = (i - 1) % 4;
		if(tempInt == -1)
			tempInt = xSideRot.size() - 1;

		xSideRot.at(i)->setString(temps.at(tempInt));
	}
}
void Cube::switchSidesXNeg()
{
	vector<string> temps;
	for(int i = 0; i < (int)xSideRot.size(); i++)
	{
		temps.push_back(xSideRot.at(i)->getString());
	}
	for(int i = 0; i < (int)xSideRot.size(); i++)
	{
		xSideRot.at(i)->setString(temps.at((i + 1) % 4));
	}
}
void Cube::switchSidesYNeg()
{
	vector<string> temps;
	for(int i = 0; i < (int)ySideRot.size(); i++)
	{
		temps.push_back(ySideRot.at(i)->getString());
	}
	for(int i = ySideRot.size() - 1; i >= 0; i--)
	{
		int tempInt = (i - 1) % 4;
		if(tempInt == -1)
			tempInt = ySideRot.size() - 1;

		ySideRot.at(i)->setString(temps.at(tempInt));
	}
}
void Cube::switchSidesYPos()
{
	vector<string> temps;
	for(int i = 0; i < (int)ySideRot.size(); i++)
	{
		temps.push_back(ySideRot.at(i)->getString());
	}
	for(int i = 0; i < (int)ySideRot.size(); i++)
	{
		ySideRot.at(i)->setString(temps.at((i + 1) % 4));
	}
}
void Cube::switchSidesZPos()
{
	vector<string> temps;
	for(int i = 0; i < (int)zSideRot.size(); i++)
	{
		temps.push_back(zSideRot.at(i)->getString());
	}
	for(int i = (int)zSideRot.size() - 1; i >= 0; i--)
	{
		int tempInt = (i - 1) % 4;
		if(tempInt == -1)
			tempInt = zSideRot.size() - 1;

		zSideRot.at(i)->setString(temps.at(tempInt));
	}
}
void Cube::switchSidesZNeg()
{
	vector<string> temps;
	for(int i = 0; i < (int)zSideRot.size(); i++)
	{
		temps.push_back(zSideRot.at(i)->getString());
	}
	for(int i = 0; i < (int)zSideRot.size(); i++)
	{
		zSideRot.at(i)->setString(temps.at((i + 1) % 4));
	}
}

bool Cube::frontBotCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 - 4, totalTrans._43 + 4);
}
bool Cube::frontMidCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42, totalTrans._43 + 4);
}
bool Cube::frontTopCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 + 4, totalTrans._43 + 4);
}
bool Cube::backBotCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 - 4, totalTrans._43 - 4);
}
bool Cube::backMidCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42, totalTrans._43 - 4);
}
bool Cube::backTopCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 + 4, totalTrans._43 - 4);
}
bool Cube::rightBotCollision()
{
	return this->levelHandler->collision(totalTrans._41 + 4, totalTrans._42 - 4, totalTrans._43);
}
bool Cube::rightMidCollision()
{
	return this->levelHandler->collision(totalTrans._41 + 4, totalTrans._42, totalTrans._43);
}
bool Cube::rightTopCollision()
{
	return this->levelHandler->collision(totalTrans._41 + 4, totalTrans._42 + 4, totalTrans._43);
}
bool Cube::leftBotCollision()
{
	return this->levelHandler->collision(totalTrans._41 - 4, totalTrans._42 - 4, totalTrans._43);
}
bool Cube::leftMidCollision()
{
	return this->levelHandler->collision(totalTrans._41 - 4, totalTrans._42, totalTrans._43);
}
bool Cube::leftTopCollision()
{
	return this->levelHandler->collision(totalTrans._41 - 4, totalTrans._42 + 4, totalTrans._43);
}
bool Cube::frontFrontCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42, totalTrans._43 + 8);
}
bool Cube::backBackCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42, totalTrans._43 - 8);
}
bool Cube::rightRightCollision()
{
	return this->levelHandler->collision(totalTrans._41 + 8, totalTrans._42, totalTrans._43);
}
bool Cube::leftLeftCollision()
{
	return this->levelHandler->collision(totalTrans._41 - 8, totalTrans._42, totalTrans._43);
}
bool Cube::botCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 - 4, totalTrans._43);
}
bool Cube::topCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 + 4, totalTrans._43);
}
bool Cube::frontRightCollision()
{
	return this->levelHandler->collision(totalTrans._41 + 4, totalTrans._42, totalTrans._43 + 4);
}
bool Cube::frontLeftCollision()
{
	return this->levelHandler->collision(totalTrans._41 - 4, totalTrans._42, totalTrans._43 + 4);
}
bool Cube::backRightCollision()
{
	return this->levelHandler->collision(totalTrans._41 + 4, totalTrans._42, totalTrans._43 - 4);
}
bool Cube::backLeftCollision()
{
	return this->levelHandler->collision(totalTrans._41 - 4, totalTrans._42, totalTrans._43 - 4);
}
bool Cube::frontFrontBotCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 - 4, totalTrans._43 + 8);
}
bool Cube::frontFrontTopCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 + 4, totalTrans._43 + 8);
}
bool Cube::backBackBotCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 - 4, totalTrans._43 - 8);
}
bool Cube::backBackTopCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 + 4, totalTrans._43 - 8);
}
bool Cube::rightRightBotCollision()
{
	return this->levelHandler->collision(totalTrans._41 + 8, totalTrans._42 - 4, totalTrans._43);
}
bool Cube::rightRightTopCollision()
{
	return this->levelHandler->collision(totalTrans._41 + 8, totalTrans._42 + 4, totalTrans._43);
}
bool Cube::leftLeftBotCollision()
{
	return this->levelHandler->collision(totalTrans._41 - 8, totalTrans._42 - 4, totalTrans._43);
}
bool Cube::leftLeftTopCollision()
{
	return this->levelHandler->collision(totalTrans._41 - 8, totalTrans._42 + 4, totalTrans._43);
}
bool Cube::rightRightFrontCollision()
{
	return this->levelHandler->collision(totalTrans._41 + 8, totalTrans._42, totalTrans._43 + 4);
}
bool Cube::rightRightBackCollision()
{
	return this->levelHandler->collision(totalTrans._41 + 8, totalTrans._42, totalTrans._43 - 4);
}
bool Cube::leftLeftFrontCollision()
{
	return this->levelHandler->collision(totalTrans._41 - 8, totalTrans._42, totalTrans._43 + 4);
}
bool Cube::leftLeftBackCollision()
{
	return this->levelHandler->collision(totalTrans._41 - 8, totalTrans._42, totalTrans._43 - 4);
}
bool Cube::frontFrontLeftCollision()
{
	return this->levelHandler->collision(totalTrans._41 - 4, totalTrans._42, totalTrans._43 + 8);
}
bool Cube::frontFrontRightCollision()
{
	return this->levelHandler->collision(totalTrans._41 + 4, totalTrans._42, totalTrans._43 + 8);
}
bool Cube::backBackLeftCollision()
{
	return this->levelHandler->collision(totalTrans._41 - 4, totalTrans._42, totalTrans._43 - 8);
}
bool Cube::backBackRightCollision()
{
	return this->levelHandler->collision(totalTrans._41 + 4, totalTrans._42, totalTrans._43 - 8);
}
bool Cube::rightBotBotCollision()
{
	return this->levelHandler->collision(totalTrans._41 + 4, totalTrans._42 - 8, totalTrans._43);
}
bool Cube::rightTopTopCollision()
{
	return this->levelHandler->collision(totalTrans._41 + 4, totalTrans._42 + 8, totalTrans._43);
}
bool Cube::leftBotBotCollision()
{
	return this->levelHandler->collision(totalTrans._41 - 4, totalTrans._42 - 8, totalTrans._43);
}
bool Cube::leftTopTopCollision()
{
	return this->levelHandler->collision(totalTrans._41 - 4, totalTrans._42 + 8, totalTrans._43);
}
bool Cube::botBotCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 - 8, totalTrans._43);
}
bool Cube::topTopCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 + 8, totalTrans._43);
}
bool Cube::frontBotBotCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 - 8, totalTrans._43 + 4);
}
bool Cube::backBotBotCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 - 8, totalTrans._43 - 4);
}
bool Cube::frontTopTopCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 + 8, totalTrans._43 + 4);
}
bool Cube::backTopTopCollision()
{
	return this->levelHandler->collision(totalTrans._41, totalTrans._42 + 8, totalTrans._43 - 4);
}
vector<D3DXVECTOR3> Cube::getOBBPoints()
{
	vector<D3DXVECTOR3> points;
	float value = 1.37;
	points.push_back(D3DXVECTOR3(value, value, value));
	points.push_back(D3DXVECTOR3(value, value, -value));
	points.push_back(D3DXVECTOR3(value, -value, value));
	points.push_back(D3DXVECTOR3(value, -value, -value));

	points.push_back(D3DXVECTOR3(-value, value, value));
	points.push_back(D3DXVECTOR3(-value, value, -value));
	points.push_back(D3DXVECTOR3(-value, -value, value));
	points.push_back(D3DXVECTOR3(-value, -value, -value));

	for(int i = 0; i < (int)points.size(); i++)
	{
		D3DXVec3TransformCoord(&points[i], &points[i], &cubeMatrix);
	}

	return points;
}

void Cube::rotateControls(bool right)
{
	char sTempFor = tempFor;
	char sTempBack = tempBack;
	char sTempRight = tempRight;
	char sTempLeft = tempLeft;

	if(right)
	{
		tempFor = sTempRight;
		tempBack = sTempLeft;
		tempRight = sTempBack;
		tempLeft = sTempFor;
	}
	else
	{
		tempFor = sTempLeft;
		tempBack = sTempRight;
		tempRight = sTempFor;
		tempLeft = sTempBack;
	}

	updateControles();
}

void Cube::updateControles()
{
		bool colRight = this->rightMidCollision();
		bool colLeft = this->leftMidCollision();
		bool colBack = this->backMidCollision();
		bool colTop = this->topCollision();
		bool colBot = this->botCollision();
		bool colFront = this->frontMidCollision();

		if(colBot || (colFront && !colLeft && !colRight && !colBack && !colTop) || (colLeft && colRight && !colTop))
		{
			forChar = tempFor;
			backChar = tempBack;
			rightChar = tempRight;
			leftChar = tempLeft;
		}
		else if(colTop)
		{
			forChar = tempBack;
			backChar = tempFor;
			rightChar = tempLeft;
			leftChar = tempRight;
		}
		else if(colRight)
		{
			forChar = tempLeft;
			backChar = tempRight;
			rightChar = tempBack;
			leftChar = tempFor;
		}
		else if(colLeft)
		{
			forChar = tempLeft;
			backChar = tempRight;
			rightChar = tempFor;
			leftChar = tempBack;
		}
		else if(colBack)
		{
			forChar = tempFor;
			backChar = tempBack;
			rightChar = tempLeft;
			leftChar = tempRight;
		}
}

bool Cube::stopBlockCollision(int x, int y, int z)
{
	return levelHandler->stopBlockCollision(totalTrans._41 + x, totalTrans._42 + y, totalTrans._43 + z);
}

void Cube::playAudio()
{
	if(levelHandler->blockType1Collision(totalTrans._41 + landingSpot.x, totalTrans._42 + landingSpot.y, totalTrans._43 + landingSpot.z))
	{
		SOUND* sfx = SOUNDMANAGER->PlaySound("../Bin/Sounds/collision1.wav");
		sfx->SetVolume(0.1f);
	}
	else if(levelHandler->blockType2Collision(totalTrans._41 + landingSpot.x, totalTrans._42 + landingSpot.y, totalTrans._43 + landingSpot.z))
	{
		SOUND* sfx = SOUNDMANAGER->PlaySound("../Bin/Sounds/collision2.wav");
		sfx->SetVolume(0.1f);
	}
	else if(levelHandler->blockType3Collision(totalTrans._41 + landingSpot.x, totalTrans._42 + landingSpot.y, totalTrans._43 + landingSpot.z))
	{
		SOUND* sfx = SOUNDMANAGER->PlaySound("../Bin/Sounds/collision3.wav");
		sfx->SetVolume(0.1f);
	}
}
bool Cube::jumpPossible(D3DXVECTOR3 position)
{
	if(this->levelHandler->collision(totalTrans._41 + position.x, totalTrans._42 + position.y, totalTrans._43 + position.z))
		return false;
	if(this->levelHandler->collision(totalTrans._41 + position.x - 4, totalTrans._42 + position.y, totalTrans._43 + position.z))
		return true;
	if(this->levelHandler->collision(totalTrans._41 + position.x + 4, totalTrans._42 + position.y, totalTrans._43 + position.z))
		return true;
	if(this->levelHandler->collision(totalTrans._41 + position.x, totalTrans._42 + position.y + 4, totalTrans._43 + position.z))
		return true;
	if(this->levelHandler->collision(totalTrans._41 + position.x, totalTrans._42 + position.y - 4, totalTrans._43 + position.z))
		return true;
	if(this->levelHandler->collision(totalTrans._41 + position.x, totalTrans._42 + position.y, totalTrans._43 + position.z + 4))
		return true;
	if(this->levelHandler->collision(totalTrans._41 + position.x, totalTrans._42 + position.y, totalTrans._43 + position.z - 4))
		return true;


	return false;
}

void Cube::setPosition(D3DXVECTOR3 pos)
{
	totalTrans._41 = pos.x;
	totalTrans._42 = pos.y;
	totalTrans._43 = pos.z;
}

bool Cube::puzzlePiecesLeft()
{
	return levelHandler->puzzlePiecesLeft();
}

void Cube::reset()
{
	GetAsyncKeyState('W');
	GetAsyncKeyState('A');
	GetAsyncKeyState('S');
	GetAsyncKeyState('D');

	rotationX = 0;
	rotationY = 0;
	rotationZ = 0;
	rotationXz = 0;
	rotationXy = 0;
	rotationXy2 = 0;

	loopStart.clear();

	yPivot = false;
	zPivot = false;
	invRot = false;

	forward = false;
	backwards = false;
	right = false;
	left = false;

	camRot = 0;
	camY = 1;

	forChar = 'W';
	backChar = 'S';
	leftChar = 'A';
	rightChar = 'D';

	tempFor = 'W';
	tempBack = 'S';
	tempLeft = 'A';
	tempRight = 'D';

	sides[0]->setString("bot");
	sides[1]->setString("top");
	sides[2]->setString("front");
	sides[3]->setString("back");
	sides[4]->setString("left");
	sides[5]->setString("right");

	D3DXMatrixIdentity(&totalRot);
	moving = false;
	colliding = false;

	points = 0;
	timeLeft = 0;
}

void Cube::sendTime(int timeLeft)
{
	this->timeLeft = timeLeft;
}

void Cube::checkWin()
{
	if(levelHandler->puzzlePiecesLeft())
	{
		levelHandler->setScore(points + timeLeft * POINTS_PER_SECOND);
		reset();

		levelHandler->changeLevel();
		setPosition(levelHandler->getPlayerPos());
	}
}
bool Cube::loopStuck()
{
	for(int i = 0; i < loopStart.size(); i++)
	{
		if(totalTrans._41 == loopStart[i].x && totalTrans._42 == loopStart[i].y && totalTrans._43 == loopStart[i].z)
			return true;
	}
	return false;
}
