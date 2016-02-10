#include "Menu.h"

Menu::Menu(string filename, ID3D10Device* device, ID3D10ShaderResourceView* diff, ID3D10ShaderResourceView* spec)
{
	mModelData = new LoadModel(filename);
	mModelSize = mModelData->getModelDataSize();
	mModel = new VertexPos4NorTex[mModelSize];
	for(int i = 0; i < mModelSize; i++)
	{
		mModel[i].pos = mModelData->getModelPosition(i);
		mModel[i].normal = mModelData->getModelNormal(i);
		mModel[i].texC = mModelData->getModelTexture(i);
	}

	mDevice = device;

	mW = Effects::mModelFX->GetVariableByName("gWorld")->AsMatrix();
	mV = Effects::mModelFX->GetVariableByName("gV")->AsMatrix();
	mP = Effects::mModelFX->GetVariableByName("gP")->AsMatrix();

	mTechnique = Effects::mModelFX->GetTechniqueByName("Render");
	mfxModelLightWVPVar = Effects::mModelFX->GetVariableByName("gLightWVP")->AsMatrix();

	mDiff = diff;
	mSpec = spec;

	mText = new Text(device);

	mMenuText = new MenuText(1, "../Bin/Models/text.obj", device, TextureManager::mDefaultSpec);
	mMenuTextVolume = new MenuText(2, "../Bin/Models/volume.obj", device, TextureManager::mDefaultSpec);
	mMenuTextPlus = new MenuText(3, "../Bin/Models/plus.obj", device, TextureManager::mDefaultSpec);
	mMenuTextMinus = new MenuText(4, "../Bin/Models/minus.obj", device, TextureManager::mDefaultSpec);
	skybox = new Sky("../Bin/Models/sky.obj", device, TextureManager::mSky1);
	mVolumeObject = new VolumeObject("../Bin/Models/playerCube.obj", device, TextureManager::mPlayer);

	mRotatingX = 0;
	mRotatingY = 0;
	mRotatingZ = 0;

	mMiniScale = 1.0f;

	mSide = 1;
	mRotX = 0;
	mRotY = 0;
	mRotZ = 0;

	mainMenu = true;
	left = false;
	right = false;
	up = false;
	down = false;

	mIsMoving = false;

	 mCompletedLevels = 0;

		mMiniLevel.push_back(MenuObjects("../Bin/Models/miniCube.obj", mDevice, TextureManager::mLevel1, 1, TextureManager::mDefaultSpec, D3DXVECTOR3(3.0f, (float)-1 + 3.0f, 2.0f)));
	for(int i = 2; i < 6; i++)
		mMiniLevel.push_back(MenuObjects("../Bin/Models/miniCube.obj", mDevice, TextureManager::mNoLevel, i, TextureManager::mDefaultSpec, D3DXVECTOR3(3.0f, (float)-i + 3.0f, 2.0f)));
	for(int i = 6; i < 11; i++)
		mMiniLevel.push_back(MenuObjects("../Bin/Models/miniCube.obj", mDevice, TextureManager::mNoLevel, i, TextureManager::mDefaultSpec, D3DXVECTOR3(1.5f, (float)-i + 8.0f, 2.0f)));
	for(int i = 11; i < 16; i++)
		mMiniLevel.push_back(MenuObjects("../Bin/Models/miniCube.obj", mDevice, TextureManager::mNoLevel, i, TextureManager::mDefaultSpec, D3DXVECTOR3(0.0f, (float)-i + 13.0f, 2.0f)));
	for(int i = 16; i < 21; i++)
		mMiniLevel.push_back(MenuObjects("../Bin/Models/miniCube.obj", mDevice, TextureManager::mNoLevel, i, TextureManager::mDefaultSpec, D3DXVECTOR3(-1.5f, (float)-i + 18.0f, 2.0f)));
	for(int i = 21; i < 26; i++)
		mMiniLevel.push_back(MenuObjects("../Bin/Models/miniCube.obj", mDevice, TextureManager::mNoLevel, i, TextureManager::mDefaultSpec, D3DXVECTOR3(-3.0f, (float)-i + 23.0f, 2.0f)));

	for(int i = 1; i < 6; i++)
		mHighscores.push_back(Highscore("../Bin/Models/miniCube.obj", mDevice, TextureManager::mNoLevel, i, TextureManager::mDefaultSpec, D3DXVECTOR3(3.0f, (float)-i + 3.0f, 2.0f)));

	mMiniRotator = 0.0f;

	D3DXMatrixRotationX(&Rx,mRotX);
	D3DXMatrixRotationY(&Ry,mRotY);
	D3DXMatrixRotationZ(&Rz,mRotZ);
	D3DXMatrixScaling(&S, 1.0f, 1.0f, 1.0f);

	mPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//Get texture
	mEffectDiffMapVar = Effects::mModelFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mEffectSpecMapVar = Effects::mModelFX->GetVariableByName("gSpecMap")->AsShaderResource();

	//init buffer with defined line list data
	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(VertexPos4NorTex) * mModelSize;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vinitData;
	 vinitData.pSysMem = mModel;

	mDevice->CreateBuffer(
		&vbd,		//description of buffer to create
		&vinitData, //data to initialize buffer with
		&mVertexBuffer);		//return the created buffer
}

void Menu::changeMenu(int type)
{
	bool change = false;
	switch(type)
	{
	case 1:
		{
			if(mainMenu == true || mSide == 3)
			{
				left = true;
				change = true;
			}
		}
		break;
	case 2:
		{
			if(mainMenu == true || mSide == 2)
			{
				right = true;
				change = true;
			}
		}
		break;
	case 3:
		{
			if(mainMenu == true || mSide == 5)
			{
				up = true;
				change = true;
			}
		}
		break;
	case 4:
		{
			if(mainMenu == true || mSide == 4)
			{
				down = true;
				change = true;
			}
		}
		break;
	}
	if(change)
		if(mainMenu)
			mainMenu = false;
		else
			mainMenu = true;
}

void Menu::Update()
{
	if(left)
	{
		if(mSide == 1)
		{
			if(mRotY < PI/2)
			{
				mIsMoving = true;
				mRotY += 10.0f * gDeltaTime;
			}
			else
			{
				mIsMoving = false;
				mRotY = PI/2;
				left = false;
				mSide = 2;
				SOUNDMANAGER->PlaySound("../Bin/Sounds/swish.wav");
			}
		}
		else
		{
			if(mRotY < 0)
			{
				mIsMoving = true;
				mRotY += 10.0f * gDeltaTime;
			}
			else
			{
				mIsMoving = false;
				mRotY = 0;
				left = false;
				mSide = 1;
				SOUNDMANAGER->PlaySound("../Bin/Sounds/swish.wav");
			}
		}
	}
	if(right)
	{
		if(mSide == 1)
		{
			if(mRotY > -(PI/2))
			{
				mIsMoving = true;
				mRotY -= 10.0f * gDeltaTime;
			}
			else
			{
				mIsMoving = false;
				mRotY = -(PI/2);
				right = false;
				mSide = 3;
				SOUNDMANAGER->PlaySound("../Bin/Sounds/swish.wav");
			}
		}
		else
		{
			if(mRotY > 0)
			{
				mIsMoving = true;
				mRotY -= 10.0f * gDeltaTime;
			}
			else
			{
				mIsMoving = false;
				mRotY = 0;
				right = false;
				mSide = 1;
				SOUNDMANAGER->PlaySound("../Bin/Sounds/swish.wav");
			}
		}
	}
	if(up)
	{
		if(mSide == 1)
		{
			if(mRotZ < PI/2)
			{
				mIsMoving = true;
				mRotZ += 10.0f * gDeltaTime;
			}
			else
			{
				mIsMoving = false;
				mRotZ = (PI/2);
				up = false;
				mSide = 4;
				SOUNDMANAGER->PlaySound("../Bin/Sounds/swish.wav");
			}
		}
		else
		{
			if(mRotZ < 0)
			{
				mIsMoving = true;
				mRotZ += 10.0f * gDeltaTime;
			}
			else
			{
				mIsMoving = false;
				mRotZ = 0;
				up = false;
				mSide = 1;
				SOUNDMANAGER->PlaySound("../Bin/Sounds/swish.wav");
			}
		}
	}
	if(down)
	{
		if(mSide == 1)
		{
			if(mRotZ > -(PI/2))
			{
				mIsMoving = true;
				mRotZ -= 10.0f * gDeltaTime;
			}
			else
			{
				mIsMoving = false;
				mRotZ = -(PI/2);
				down = false;
				mSide = 5;
				SOUNDMANAGER->PlaySound("../Bin/Sounds/swish.wav");
			}
		}
		else
		{
			if(mRotZ > 0)
			{
				mIsMoving = true;
				mRotZ -= 10.0f * gDeltaTime;
			}
			else
			{
				mIsMoving = false;
				mRotZ = 0;
				down = false;
				mSide = 1;
				SOUNDMANAGER->PlaySound("../Bin/Sounds/swish.wav");
			}
		}
	}

	//D3DXMATRIX inversView;
	//float det = D3DXMatrixDeterminant(&GetCamera().mView);
	//D3DXMatrixInverse(&inversView, &det, &GetCamera().mView);

	mPosition.y += gDeltaTime;
	mPosition.x += gDeltaTime;
	mPosition.z += gDeltaTime;

	D3DXMatrixRotationZ(&Rx, mRotX);
	D3DXMatrixRotationX(&Rz, mRotZ);
	D3DXMatrixRotationY(&Ry, mRotY);
	D3DXMatrixTranslation(&T, 0.0f, 0.0f, -2.0f);

	D3DXMatrixTranslation(&T, cosf(mPosition.x) * 0.1f, sinf(mPosition.y) * 0.5f + 0.5f, cosf(mPosition.z) * 0.1f);
	
	W = S * Rx * Ry * Rz * T;


	if(HUDLevelSelect)
	{
		for(int i = 0; i < (int)mMiniLevel.size(); i++)
		{
			if(!mMiniLevel[i].mRotated)
			{
				if(miniSelector == i)
				{
					mMiniLevel[i].mRotation.y -= gDeltaTime;
				}
				else
				{
					mMiniLevel[i].mRotation.y += gDeltaTime;
				}
			}
			mMiniLevel[i].Update();
		}
	
		if(mMiniScale < 1.2f)
			mMiniScale += gDeltaTime;
		mMiniLevel[miniSelector].mScale = D3DXVECTOR3(mMiniScale, mMiniScale, mMiniScale);
		mMiniLevel[miniSelector].Update();
	}

	if(HUDHighScore)
	{
		for(int i = 0; i < (int)mHighscores.size(); i++)
			mHighscores[i].Update();
	}

	//if(HUDOptions)
	//{
	//	mVolumeObject->Update();
	//}

	mMenuTextPlus->Update();
	mMenuTextMinus->Update();
	mMenuTextVolume->Update();
	mMenuText->Update();
}

void Menu::Draw()
{
	mMenuText->Draw();
	mEffectDiffMapVar->SetResource(mDiff);
	mEffectSpecMapVar->SetResource(mSpec);
	mDevice->IASetInputLayout(InputLayout::mModel);
	mDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	mW->SetMatrix((float*)W);
	mV->SetMatrix((float*)GetCamera().mView);
	mP->SetMatrix((float*)GetCamera().mProj);

	UINT stride = sizeof(VertexPos4NorTex);
	UINT offset = 0;
	mDevice->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	if(!HUDHighScore && !HUDLevelSelect && !HUDOptions)
	{
		//Draw triangles
		D3D10_TECHNIQUE_DESC techDesc;
		mTechnique->GetDesc( &techDesc );
		for( UINT p = 0; p < techDesc.Passes; ++p )
		{
			mTechnique->GetPassByIndex( p )->Apply( 0 );
			mDevice->Draw(mModelSize, 0);
		}
	}

	if(HUDLevelSelect)
	{
		for(int i = 0; i < (int)mMiniLevel.size(); i++)
			mMiniLevel[i].Draw();
	}

	if(HUDOptions)
	{
		mVolumeObject->Draw();
		mMenuTextVolume->Draw();
		mMenuTextMinus->Draw();
		mMenuTextPlus->Draw();
	}

	if(HUDHighScore)
	{
		for(int i = 0; i < (int)mHighscores.size(); i++)
			mHighscores[i].Draw();
	}
	
	skybox->Draw();
}

void Menu::setTexture()
{
	if(currentLevel == 0)
	{
		mMiniLevel[currentLevel + 1].setTexture(TextureManager::mLevel2);
	}
	else if(currentLevel == 1)
	{
		mMiniLevel[currentLevel + 1].setTexture(TextureManager::mLevel3);
	}
	else if(currentLevel == 2)
	{
		mMiniLevel[currentLevel + 1].setTexture(TextureManager::mLevel4);
	}
	else if(currentLevel == 3)
	{
		mMiniLevel[currentLevel + 1].setTexture(TextureManager::mLevel5);
	}
	else if(currentLevel == 4)
	{
		mMiniLevel[currentLevel + 1].setTexture(TextureManager::mLevel6);
	}
	else if(currentLevel == 5)
	{
		mMiniLevel[currentLevel + 1].setTexture(TextureManager::mLevel7);
	}
}

void Menu::setSkyTexture(ID3D10ShaderResourceView* diff)
{
	skybox->setTexture(diff);
}

void Menu::loadTextures(int lvl)
{
	if(lvl == 0)
	{
		mMiniLevel[lvl + 1].setTexture(TextureManager::mLevel2);
		mHighscores[lvl].setTexture(TextureManager::mLevel1);
	}
	else if(lvl == 1)
	{
		mMiniLevel[lvl + 1].setTexture(TextureManager::mLevel3);
		mHighscores[lvl].setTexture(TextureManager::mLevel2);
	}
	else if(lvl == 2)
	{
		mMiniLevel[lvl + 1].setTexture(TextureManager::mLevel4);
		mHighscores[lvl].setTexture(TextureManager::mLevel3);
	}
	else if(lvl == 3)
	{
		mMiniLevel[lvl + 1].setTexture(TextureManager::mLevel5);
		mHighscores[lvl].setTexture(TextureManager::mLevel4);
	}
}