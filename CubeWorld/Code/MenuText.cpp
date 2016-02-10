#include "MenuText.h"

MenuText::MenuText(int id, string filename, ID3D10Device* device, ID3D10ShaderResourceView* diff, ID3D10ShaderResourceView* spec, D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
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

	mId = id;

	mDevice = device;
	mPosition = position;
	mRotation.x = rotation.x * (PI / 180);
	if(mId == 3 || mId == 4)
		mRotation.y = RandF(0.0f, 360.0f) * (PI / 180);
	else
		mRotation.y = rotation.y * (PI / 180);

	mRotation.z = rotation.z * (PI / 180);
	mScale = scale;
	D3DXMatrixScaling(&S, mScale.x, mScale.y, mScale.z);
	D3DXMatrixRotationX(&Rx, mRotation.x);
	D3DXMatrixRotationY(&Ry, mRotation.y);
	D3DXMatrixRotationZ(&Rz, mRotation.z);
	D3DXMatrixTranslation(&T, mPosition.x, mPosition.y, mPosition.z);
	W = S * Rx * Ry * Rz * T;
	mStartPos = position;

	mW = Effects::mModelFX->GetVariableByName("gWorld")->AsMatrix();
	mV = Effects::mModelFX->GetVariableByName("gV")->AsMatrix();
	mP = Effects::mModelFX->GetVariableByName("gP")->AsMatrix();

	mTechnique = Effects::mModelFX ->GetTechniqueByName("Render");
	mfxModelLightWVPVar = Effects::mModelFX->GetVariableByName("gLightWVP")->AsMatrix();

	mDiff = diff;
	mSpec = spec;

	mRotated = false;

	//Get texture
	mEffectDiffMapVar = Effects::mModelFX ->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mEffectSpecMapVar = Effects::mModelFX ->GetVariableByName("gSpecMap")->AsShaderResource();

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

void MenuText::Update()
{
	//Cube vs World
	if(mId == 1)
	{
		//mRotation.y += gDeltaTime * 0.5f;
		mPosition.y += gDeltaTime * 0.1f;
		mPosition.z = -2.2f;
		mPosition.x = -0.6f;

		D3DXMatrixScaling(&S, mScale.x, mScale.y, mScale.z);
		D3DXMatrixRotationX(&Rx, mRotation.x);
		D3DXMatrixRotationY(&Ry, mRotation.y);
		D3DXMatrixRotationZ(&Rz, mRotation.z);
		D3DXMatrixTranslation(&T, mPosition.x, sinf(mPosition.y) * 2.0f + 1.0f, mPosition.z);
		W = S * Rx * Ry * Rz * T;
	}
	//Volume
	else if(mId == 2)
	{
		mPosition.y += gDeltaTime * 0.1f;
		mPosition.z = -1.8f;
		mPosition.x = -2.0f;

		D3DXMatrixScaling(&S, mScale.x, mScale.y, mScale.z);
		D3DXMatrixRotationX(&Rx, mRotation.x);
		D3DXMatrixRotationY(&Ry, mRotation.y);
		D3DXMatrixRotationZ(&Rz, mRotation.z);
		D3DXMatrixTranslation(&T, mPosition.x, sinf(mPosition.y) - 1.0f, mPosition.z);
		W = S * Rx * Ry * Rz * T;
	}
	//Plus
	else if(mId == 3)
	{
		mPosition.y += gDeltaTime * 0.1f;
		mPosition.z = -0.2f;
		mPosition.x = -6.6f;

		mRotation.y += gDeltaTime * 0.2f;

		D3DXMatrixScaling(&S, mScale.x, mScale.y, mScale.z);
		D3DXMatrixRotationX(&Rx, mRotation.x);
		D3DXMatrixRotationY(&Ry, mRotation.y);
		D3DXMatrixRotationZ(&Rz, mRotation.z);
		D3DXMatrixTranslation(&T, mPosition.x, sinf(mPosition.y), mPosition.z);
		W = S * Rx * Ry * Rz * T;
	}
	//Minus
	else if(mId == 4)
	{
		mRotation.y += gDeltaTime * 0.2f;

		mPosition.y += gDeltaTime * 0.1f;
		mPosition.z = -0.2f;
		mPosition.x = 4.0f;

		D3DXMatrixScaling(&S, mScale.x, mScale.y, mScale.z);
		D3DXMatrixRotationX(&Rx, mRotation.x);
		D3DXMatrixRotationY(&Ry, mRotation.y);
		D3DXMatrixRotationZ(&Rz, mRotation.z);
		D3DXMatrixTranslation(&T, mPosition.x, sinf(mPosition.y), mPosition.z);
		W = S * Rx * Ry * Rz * T;
	}
}

void MenuText::Draw()
{
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

	//Draw triangles
	D3D10_TECHNIQUE_DESC techDesc;
	mTechnique->GetDesc( &techDesc );
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        mTechnique->GetPassByIndex( p )->Apply( 0 );
		mDevice->Draw(mModelSize, 0);
    }

}