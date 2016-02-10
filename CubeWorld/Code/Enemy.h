#ifndef ENEMY_H
#define ENEMY_H
#include "stdafx.h"
#include "AABB.h"


class Enemy
{
public:

	D3DXMATRIX W;
	AABB *aabb;
	AABB *nextNodeAABB;
	float speed;
	vector<D3DXVECTOR3> nodeList;
	D3DXVECTOR3 moveVector;
	int nextNode;
	int currentNode;
	bool firstMove;
	bool exists;
	float tempVel;
	float posY;

	Enemy(float x, float y, float z, float speed);
	void addNode(D3DXVECTOR3 node);
	bool collision(vector<D3DXVECTOR3> points);
	void move();
	

};
#endif