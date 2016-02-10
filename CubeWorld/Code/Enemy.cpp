#include "Enemy.h"

Enemy::Enemy(float x, float y, float z, float speed)
{
	D3DXMatrixIdentity(&W);
	W._41 = x;
	W._42 = y;
	W._43 = z;

	this->speed = speed;
	nextNode = 1;
	currentNode = 0;
	firstMove = true;
	exists = true;
	tempVel = 0.2f;
	posY = 0.0f;

	nextNodeAABB = NULL;

	aabb = new AABB((int)x, (int)y, (int)z, 1.4f, 1.4f, 1.4f);
}
void Enemy::addNode(D3DXVECTOR3 node)
{
	nodeList.push_back(node);
}
bool Enemy::collision(vector<D3DXVECTOR3> points)
{
	return aabb->OBBvAABB(points);
}
void Enemy::move()
{
	if(firstMove || nextNodeAABB->collision(W._41, W._42, W._43))
	{

		W._41 = nodeList[currentNode].x;
		W._42 = nodeList[currentNode].y;
		W._43 = nodeList[currentNode].z;

		moveVector = nodeList[nextNode] - nodeList[currentNode];
		D3DXVec3Normalize(&moveVector, &moveVector);

		if(nextNodeAABB != NULL)
			delete nextNodeAABB;

		nextNodeAABB = new AABB((int)nodeList[nextNode].x, (int)nodeList[nextNode].y, (int)nodeList[nextNode].z, tempVel, tempVel,  tempVel);

		nextNode++;
		currentNode++;

		if(nextNode == nodeList.size())
		{
			nextNode = 0;
		}
		if(currentNode == nodeList.size())
		{
			currentNode = 0;
		}

		firstMove = false;
	}
	aabb->update(W._41, W._42, W._43, 1.4f, 1.4f, 1.4f);
	nextNodeAABB->updateSize(tempVel, tempVel, tempVel);

	tempVel = speed * gDeltaTime * 2;

	W._41 += moveVector.x * speed * gDeltaTime;
	W._42 += moveVector.y * speed * gDeltaTime;
	W._43 += moveVector.z * speed * gDeltaTime;
}