#include "Level.h"

Level::Level(ID3D10Device *tDevice, float x, float y, float z, int time)
{
	device = tDevice;
	offset = 4.0f;
	octree = new Octree(1, x, y, z);
	levelX = (int)x;
	levelY = (int)y;
	levelZ = (int)z;
	currentNode = 0;
	player = new ModelInstance(0,0,0);
	this->time = time;

	score = 0;

	frontArrow = new OtherModels(0, "../Bin/Models/Arrows/front.obj", device, TextureManager::mArrowFront);
	backArrow = new OtherModels(0, "../Bin/Models/Arrows/back.obj", device, TextureManager::mArrowBack);
	leftArrow = new OtherModels(0, "../Bin/Models/Arrows/left.obj", device, TextureManager::mArrowLeft);
	rightArrow = new OtherModels(0, "../Bin/Models/Arrows/right.obj", device, TextureManager::mArrowRight);
	botArrow = new OtherModels(0, "../Bin/Models/Arrows/bot.obj", device, TextureManager::mArrowBot);
	topArrow = new OtherModels(0, "../Bin/Models/Arrows/top.obj", device, TextureManager::mArrowTop);
}
void Level::clearLevel()
{
	delete player;
	delete octree;

	for(int i = 0; i < (int)boxInstances.size(); i++)
		delete boxInstances[i];
	for(int i = 0; i < (int)sandInstances.size(); i++)
		delete sandInstances[i];
	for(int i = 0; i < (int)stoneInstances.size(); i++)
		delete stoneInstances[i];
	for(int i = 0; i < (int)topBlockInstance.size(); i++)
		delete topBlockInstance[i];
	for(int i = 0; i < (int)botBlockInstance.size(); i++)
		delete botBlockInstance[i];
	for(int i = 0; i < (int)leftBlockInstance.size(); i++)
		delete leftBlockInstance[i];
	for(int i = 0; i < (int)rightBlockInstance.size(); i++)
		delete  rightBlockInstance[i];
	for(int i = 0; i < (int)frontBlockInstance.size(); i++)
		delete frontBlockInstance[i];
	for(int i = 0; i < (int)backBlockInstance.size(); i++)
		delete backBlockInstance[i];

	delete frontArrow;
	delete backArrow;
	delete rightArrow;
	delete leftArrow;
	delete topArrow;
	delete botArrow;
}

void Level::createInstance(int type, int y, int z, int x)
{
	int node = 0;
	if(x < levelX / 2)
	{
		if(y < levelY / 2)
		{
			if(z < levelZ / 2)
			{
				node = 0;
			}
			else
				node = 1;
		}
		else
			if(z < levelZ / 2)
			{
				node = 2;
			}
			else
				node = 3;
	}
	else
	{
		if(y < levelY / 2)
		{
			if(z < levelZ / 2)
			{
				node = 4;
			}
			else
				node = 5;
		}
		else
			if(z < levelZ / 2)
			{
				node = 6;
			}
			else
				node = 7;
	}

	if(type == 1)
	{
		boxInstances.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(boxInstances.at(boxInstances.size() - 1), node);
	}
	else if(type == 2)
	{
		sandInstances.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(sandInstances.at(sandInstances.size() - 1), node);
	}
	else if(type == 3)
	{
		stoneInstances.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(stoneInstances.at(stoneInstances.size() - 1), node);
	}
	else if(type == 4)
	{
		topBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(topBlockInstance.at(topBlockInstance.size() - 1), node);
	}
	else if(type == 5)
	{
		botBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(botBlockInstance.at(botBlockInstance.size() - 1), node);
	}
	else if(type == 6)
	{
		rightBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(rightBlockInstance.at(rightBlockInstance.size() - 1), node);
	}
	else if(type == 7)
	{
		leftBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(leftBlockInstance.at(leftBlockInstance.size() - 1), node);
	}
	else if(type == 8)
	{
		frontBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(frontBlockInstance.at(frontBlockInstance.size() - 1), node);
	}
	else if(type == 9)
	{
		backBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(backBlockInstance.at(backBlockInstance.size() - 1), node);
	}
	else if(type == 10)
	{
		puzzleBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		puzzlePieces.push_back(true);
		octree->addToNode(puzzleBlockInstance.at(puzzleBlockInstance.size() - 1), node);
	}
	else if(type == 11)
	{
		coinBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		coins.push_back(true);
		octree->addToNode(coinBlockInstance.at(coinBlockInstance.size() - 1), node);
	}
	else if(type == 13)
	{
		stopBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(stopBlockInstance.at(stopBlockInstance.size() - 1), node);
	}
	else if(type == 14)
	{
		invTopBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(invTopBlockInstance.at(invTopBlockInstance.size() - 1), node);
	}
	else if(type == 15)
	{
		invBotBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(invBotBlockInstance.at(invBotBlockInstance.size() - 1), node);
	}
	else if(type == 16)
	{
		invRightBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(invRightBlockInstance.at(invRightBlockInstance.size() - 1), node);
	}
	else if(type == 17)
	{
		invLeftBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(invLeftBlockInstance.at(invLeftBlockInstance.size() - 1), node);
	}
	else if(type == 18)
	{
		invFrontBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(invFrontBlockInstance.at(invFrontBlockInstance.size() - 1), node);
	}
	else if(type == 19)
	{
		invBackBlockInstance.push_back(new ModelInstance(x * offset, y * offset, z * offset));
		octree->addToNode(invBackBlockInstance.at(invBackBlockInstance.size() - 1), node);
	}
	else if(type == 99)
	{
		player->W._41 = x * offset;
		player->W._42 = y * offset;
		player->W._43 = z * offset;
	}
}

void Level::draw(vector<ModelType*> blockTypes, vector<ModelType*> enemyTypes)
{
	//drawTypes(blockTypes[1], 1);

	drawTypes(blockTypes[10], 10);
	drawTypes(blockTypes[11], 11);
	//Inget block, endast för ful-fix
	//drawTypes(blockTypes[12], 12);

	drawPlayer(blockTypes[0]);

	for(int i = 0; i < (int)frontBlockInstance.size(); i++)
	{
		frontArrow->W = frontBlockInstance[i]->W;
		frontArrow->Draw();
	}

	for(int i = 0; i < (int)backBlockInstance.size(); i++)
	{
		backArrow->W = backBlockInstance[i]->W;
		backArrow->Draw();
	}

	for(int i = 0; i < (int)leftBlockInstance.size(); i++)
	{
		leftArrow->W = leftBlockInstance[i]->W;
		leftArrow->Draw();
	}

	for(int i = 0; i < (int)rightBlockInstance.size(); i++)
	{
		rightArrow->W = rightBlockInstance[i]->W;
		rightArrow->Draw();
	}

	for(int i = 0; i < (int)topBlockInstance.size(); i++)
	{
		topArrow->W = topBlockInstance[i]->W;
		topArrow->Draw();
	}

	for(int i = 0; i < (int)botBlockInstance.size(); i++)
	{
		botArrow->W = botBlockInstance[i]->W;
		botArrow->Draw();
	}

	//------------------------------

	for(int i = 0; i < (int)invFrontBlockInstance.size(); i++)
	{
		frontArrow->W = invFrontBlockInstance[i]->W;
		frontArrow->Draw();
	}

	for(int i = 0; i < (int)invBackBlockInstance.size(); i++)
	{
		backArrow->W = invBackBlockInstance[i]->W;
		backArrow->Draw();
	}

	for(int i = 0; i < (int)invLeftBlockInstance.size(); i++)
	{
		leftArrow->W = invLeftBlockInstance[i]->W;
		leftArrow->Draw();
	}

	for(int i = 0; i < (int)invRightBlockInstance.size(); i++)
	{
		rightArrow->W = invRightBlockInstance[i]->W;
		rightArrow->Draw();
	}

	for(int i = 0; i < (int)invTopBlockInstance.size(); i++)
	{
		topArrow->W = invTopBlockInstance[i]->W;
		topArrow->Draw();
	}

	for(int i = 0; i < (int)invBotBlockInstance.size(); i++)
	{
		botArrow->W = invBotBlockInstance[i]->W;
		botArrow->Draw();
	}
}

void Level::drawTrans(vector<ModelType*> blockTypes, vector<ModelType*> enemyTypes)
{
	drawTypes(blockTypes[1], 1);
	drawTypes(blockTypes[2], 2);
	drawTypes(blockTypes[3], 3);
	drawTypes(blockTypes[4], 4);
	drawTypes(blockTypes[5], 5);
	drawTypes(blockTypes[6], 6);
	drawTypes(blockTypes[7], 7);
	drawTypes(blockTypes[8], 8);
	drawTypes(blockTypes[9], 9);

	drawTypes(blockTypes[13], 13);
	drawTypes(blockTypes[14], 14);
	drawTypes(blockTypes[15], 15);
	drawTypes(blockTypes[16], 16);
	drawTypes(blockTypes[17], 17);
	drawTypes(blockTypes[18], 18);
	drawTypes(blockTypes[19], 19);


	for(int i = 0; i < (int)enemyTypes.size(); i++)
		drawEnemies(enemyTypes[i], i);
	
}

void Level::drawTypes(ModelType* type, int i)
{
	D3DXMATRIX W;
	ID3D10EffectMatrixVariable* mW;
	ID3D10EffectTechnique*	mTechnique;
	mTechnique = Effects::mModelFX->GetTechniqueByName("Render");
	mW = Effects::mModelFX->GetVariableByName("gWorld")->AsMatrix();
	type->Update(0);

	if(i == 1)
		for(int i = 0; i < (int)boxInstances.size(); i++)
		{
			W = boxInstances[i]->W;
			mW->SetMatrix((float*)W);

			D3D10_TECHNIQUE_DESC techDesc;
			mTechnique->GetDesc( &techDesc );
			mTechnique->GetPassByIndex(0)->Apply( 0 );
			device->Draw(36, 0);
		}
	else if(i == 2)
			for(int i = 0; i < (int)sandInstances.size(); i++)
			{
				W = sandInstances[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
	else if(i == 3)
			for(int i = 0; i < (int)stoneInstances.size(); i++)
			{
				W = stoneInstances[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
	else if(i == 4)
			for(int i = 0; i < (int)topBlockInstance.size(); i++)
			{
				W = topBlockInstance[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
	else if(i == 5)
			for(int i = 0; i < (int)botBlockInstance.size(); i++)
			{
				W = botBlockInstance[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
	else if(i == 7)
			for(int i = 0; i < (int)rightBlockInstance.size(); i++)
			{
				W = rightBlockInstance[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
	else if(i == 6)
			for(int i = 0; i < (int)leftBlockInstance.size(); i++)
			{
				W = leftBlockInstance[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
	else if(i == 8)
			for(int i = 0; i < (int)frontBlockInstance.size(); i++)
			{
				W = frontBlockInstance[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
	else if(i == 9)
			for(int i = 0; i < (int)backBlockInstance.size(); i++)
			{
				W = backBlockInstance[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
	else if(i == 10)
			for(int i = 0; i < (int)puzzleBlockInstance.size(); i++)
			{
				if(puzzlePieces[i])
				{
					puzzleBlockInstance[i]->Update(i);
					W = puzzleBlockInstance[i]->W;
					mW->SetMatrix((float*)W);

					D3D10_TECHNIQUE_DESC techDesc;
					mTechnique->GetDesc( &techDesc );
					mTechnique->GetPassByIndex(0)->Apply( 0 );
					device->Draw(828, 0);
				}
			}
	else if(i == 11)
			for(int i = 0; i < (int)coinBlockInstance.size(); i++)
			{
				if(coins[i])
				{
					coinBlockInstance[i]->Update(i);
					W = coinBlockInstance[i]->W;
					mW->SetMatrix((float*)W);

					D3D10_TECHNIQUE_DESC techDesc;
					mTechnique->GetDesc( &techDesc );
					mTechnique->GetPassByIndex(0)->Apply( 0 );
					device->Draw(240, 0);
				}
			}
	else if(i == 13)
				for(int i = 0; i < (int)stopBlockInstance.size(); i++)
			{
				W = stopBlockInstance[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
	else if(i == 14)
			for(int i = 0; i < (int)invTopBlockInstance.size(); i++)
			{
				W = invTopBlockInstance[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
	else if(i == 15)
			for(int i = 0; i < (int)invBotBlockInstance.size(); i++)
			{
				W = invBotBlockInstance[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
	else if(i == 16)
			for(int i = 0; i < (int)invLeftBlockInstance.size(); i++)
			{
				W = invLeftBlockInstance[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
	else if(i == 17)
			for(int i = 0; i < (int)invRightBlockInstance.size(); i++)
			{
				W = invRightBlockInstance[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
	else if(i == 18)
			for(int i = 0; i < (int)invFrontBlockInstance.size(); i++)
			{
				W = invFrontBlockInstance[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
	else if(i == 19)
			for(int i = 0; i < (int)invBackBlockInstance.size(); i++)
			{
				W = invBackBlockInstance[i]->W;
				mW->SetMatrix((float*)W);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
}

void Level::drawPlayer(ModelType* playerType)
{
	D3DXMATRIX W;
	ID3D10EffectMatrixVariable* mW;
	ID3D10EffectTechnique*	mTechnique;
	mTechnique = Effects::mModelFX->GetTechniqueByName("Render");
	mW = Effects::mModelFX->GetVariableByName("gWorld")->AsMatrix();
	playerType->Update(0);
	W = player->W;
	mW->SetMatrix((float*)W);

	D3D10_TECHNIQUE_DESC techDesc;
	mTechnique->GetDesc( &techDesc );
	mTechnique->GetPassByIndex(0)->Apply( 0 );
	device->Draw(36, 0);
}

void Level::drawEnemies(ModelType* type, int i)
{
	D3DXMATRIX W;
	ID3D10EffectMatrixVariable* mW;
	ID3D10EffectTechnique*	mTechnique;
	mTechnique = Effects::mModelFX->GetTechniqueByName("Render");
	mW = Effects::mModelFX->GetVariableByName("gWorld")->AsMatrix();
	type->Update(0);
	
	if(i == 0)
		for(int i = 0; i < (int)enemies1.size(); i++)
		{
			if(enemies1[i]->exists)
			{
				enemies1[i]->posY += gDeltaTime * 10.0f;
				enemies1[i]->move();
				W = enemies1[i]->W;
				D3DXMATRIX tempW = W;
				tempW._42 = (sinf(enemies1[i]->posY) * 0.6f) + W._42;
				mW->SetMatrix((float*)tempW);

				D3D10_TECHNIQUE_DESC techDesc;
				mTechnique->GetDesc( &techDesc );
				mTechnique->GetPassByIndex(0)->Apply( 0 );
				device->Draw(36, 0);
			}
		}
}

void Level::changeNode()
{
	if(currentNode == 7)
		currentNode = 0;
	else 
		currentNode++;
}

void Level::setPlayerMatrix(D3DXMATRIX World)
{
	player->W = World;
}

bool Level::checkCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->stopBlockInstance.size(); i++)
	{
		if(stopBlockInstance[i]->collision(x, y, z))
			return true;
	}
	for(int i = 0; i < (int)this->boxInstances.size(); i++)
	{
		if(boxInstances[i]->collision(x, y, z))
			return true;
	}
	for(int i = 0; i < (int)this->sandInstances.size(); i++)
	{
		if(sandInstances[i]->collision(x, y, z))
			return true;
	}
	for(int i = 0; i < (int)this->stoneInstances.size(); i++)
	{
		if(stoneInstances[i]->collision(x, y, z))
			return true;
	}

	for(int i = 0; i < (int)this->topBlockInstance.size(); i++)
	{
		if(topBlockInstance[i]->collision(x, y, z))
			return true;
	}
	for(int i = 0; i < (int)this->botBlockInstance.size(); i++)
	{
		if(botBlockInstance[i]->collision(x, y, z))
			return true;
	}
	for(int i = 0; i < (int)this->rightBlockInstance.size(); i++)
	{
		if(rightBlockInstance[i]->collision(x, y, z))
			return true;
	}
	for(int i = 0; i < (int)this->leftBlockInstance.size(); i++)
	{
		if(leftBlockInstance[i]->collision(x, y, z))
			return true;
	}
	for(int i = 0; i < (int)this->frontBlockInstance.size(); i++)
	{
		if(frontBlockInstance[i]->collision(x, y, z))
			return true;
	}
	for(int i = 0; i < (int)this->backBlockInstance.size(); i++)
	{
		if(backBlockInstance[i]->collision(x, y, z))
			return true;
	}
	for(int i = 0; i < (int)this->invTopBlockInstance.size(); i++)
	{
		if(invTopBlockInstance[i]->collision(x, y, z))
			return true;
	}
	for(int i = 0; i < (int)this->invBotBlockInstance.size(); i++)
	{
		if(invBotBlockInstance[i]->collision(x, y, z))
			return true;
	}
	for(int i = 0; i < (int)this->invRightBlockInstance.size(); i++)
	{
		if(invRightBlockInstance[i]->collision(x, y, z))
			return true;
	}
	for(int i = 0; i < (int)this->invLeftBlockInstance.size(); i++)
	{
		if(invLeftBlockInstance[i]->collision(x, y, z))
			return true;
	}
	for(int i = 0; i < (int)this->invFrontBlockInstance.size(); i++)
	{
		if(invFrontBlockInstance[i]->collision(x, y, z))
			return true;
	}
	for(int i = 0; i < (int)this->invBackBlockInstance.size(); i++)
	{
		if(invBackBlockInstance[i]->collision(x, y, z))
			return true;
	}
	return false;
}

bool Level::topBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->topBlockInstance.size(); i++)
	{
		if(topBlockInstance[i]->collision(x, y, z))
			return true;
	}

	return false;
}
bool Level::botBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->botBlockInstance.size(); i++)
	{
		if(botBlockInstance[i]->collision(x, y, z))
			return true;
	}

	return false;
}
bool Level::rightBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->rightBlockInstance.size(); i++)
	{
		if(rightBlockInstance[i]->collision(x, y, z))
			return true;
	}

	return false;
}
bool Level::leftBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->leftBlockInstance.size(); i++)
	{
		if(leftBlockInstance[i]->collision(x, y, z))
			return true;
	}

	return false;
}
bool Level::frontBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->frontBlockInstance.size(); i++)
	{
		if(frontBlockInstance[i]->collision(x, y, z))
			return true;
	}

	return false;
}
bool Level::backBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->backBlockInstance.size(); i++)
	{
		if(backBlockInstance[i]->collision(x, y, z))
			return true;
	}

	return false;
}
bool Level::coinBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->coinBlockInstance.size(); i++)
	{
		if(coinBlockInstance[i]->collision(x, y, z) && coins[i])
		{
			coins[i] = false;
			return true;
		}
	}

	return false;
}
bool Level::puzzleBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->puzzleBlockInstance.size(); i++)
	{
		if(puzzleBlockInstance[i]->collision(x, y, z) && puzzlePieces[i])
		{
			puzzlePieces[i] = false;
			return true;
		}
	}

	return false;
}

bool Level::enemyCollision(vector<D3DXVECTOR3> points)
{
	for(int i = 0; i < (int)this->enemies1.size(); i++)
	{
		if(enemies1[i]->collision(points))
		{
			return true;
		}
	}

	return false;
}
bool Level::coinsLeft()
{
	for(int i = 0; i < (int)coins.size(); i++)
	{
		if(coins[i])
			return false;
	}
	return true;
}
bool Level::puzzlePiecesLeft()
{
	for(int i = 0; i < (int)puzzlePieces.size(); i++)
	{
		if(puzzlePieces[i])
			return false;
	}
	return true;
}

bool Level::stopBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->stopBlockInstance.size(); i++)
	{
		if(stopBlockInstance[i]->collision(x, y, z))
			return true;
	}
	return false;
}
D3DXVECTOR3 Level::getPlayerPos()
{
	return D3DXVECTOR3(player->W._41, player->W._42, player->W._43);
}

void Level::addEnemies(string filename)
{
	fstream in(filename);
	int nrOfNodes, x, y, z;
	float speed;

	if(in)
		while(!in.eof())
		{
			
			in >> nrOfNodes >> speed;

			if(nrOfNodes == 0)
				break;

			in >> x >> y >> z;
			x *= 4;
			y *= 4;
			z *= 4;
			enemies1.push_back(new Enemy((float)x, (float)y, (float)z, speed));
			enemies1[enemies1.size() - 1]->addNode(D3DXVECTOR3((float)x, (float)y, (float)z));

			for(int i = 1; i < nrOfNodes; i++)
			{
				in >> x >> y >> z;
				x *= 4;
				y *= 4;
				z *= 4;
				enemies1[enemies1.size() - 1]->addNode(D3DXVECTOR3((float)x, (float)y, (float)z));
			}

		}

	in.close();
}

int Level::getTime()
{
	return time;
}

void Level::setScore(int points)
{
	score = points;
}

int Level::getScore()
{
	return score;
}
bool Level::blockType1Collision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->stoneInstances.size(); i++)
	{
		if(stoneInstances[i]->collision(x, y, z))
			return true;
	}

	return false;
}
bool Level::blockType2Collision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->sandInstances.size(); i++)
	{
		if(sandInstances[i]->collision(x, y, z))
			return true;
	}

	return false;
}
bool Level::blockType3Collision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->boxInstances.size(); i++)
	{
		if(boxInstances[i]->collision(x, y, z))
			return true;
	}

	return false;
}

int Level::getPuzzlesLeft()
{
	int left = 0;

	for(int i = 0; i < (int)puzzlePieces.size(); i++)
		if(puzzlePieces[i] == true)
			left++;

	return left;
}
bool Level::invTopBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->invTopBlockInstance.size(); i++)
	{
		if(invTopBlockInstance[i]->collision(x, y, z))
			return true;
	}

	return false;
}
bool Level::invBotBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->invBotBlockInstance.size(); i++)
	{
		if(invBotBlockInstance[i]->collision(x, y, z))
			return true;
	}

	return false;
}
bool Level::invRightBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->invRightBlockInstance.size(); i++)
	{
		if(invRightBlockInstance[i]->collision(x, y, z))
			return true;
	}

	return false;
}
bool Level::invLeftBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->invLeftBlockInstance.size(); i++)
	{
		if(invLeftBlockInstance[i]->collision(x, y, z))
			return true;
	}

	return false;
}
bool Level::invFrontBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->invFrontBlockInstance.size(); i++)
	{
		if(invFrontBlockInstance[i]->collision(x, y, z))
			return true;
	}

	return false;
}
bool Level::invBackBlockCollision(float x, float y, float z)
{
	for(int i = 0; i < (int)this->invBackBlockInstance.size(); i++)
	{
		if(invBackBlockInstance[i]->collision(x, y, z))
			return true;
	}

	return false;
}