#include "OtherModels.h"

OtherModels::OtherModels(int id, string filename, ID3D10Device* device, ID3D10ShaderResourceView* diff, ID3D10ShaderResourceView* spec, D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
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

	mID = id;

	mDevice = device;
	mPosition = position;
	mStartPos = mPosition;
	mScale = scale;
	mRotation.x = rotation.x * (PI / 180);
	mRotation.y = rotation.y * (PI / 180);
	mRotation.z = rotation.z * (PI / 180);

	D3DXMatrixScaling(&S, mScale.x, mScale.y, mScale.z);
	D3DXMatrixRotationX(&Rx, mRotation.x);
	D3DXMatrixRotationY(&Ry, mRotation.y);
	D3DXMatrixRotationZ(&Rz, mRotation.z);
	D3DXMatrixTranslation(&T, mPosition.x,  mPosition.y, mPosition.z);

	

	//mW = Effects::mHudFX->GetVariableByName("gWorld")->AsMatrix();
	//mV = Effects::mHudFX->GetVariableByName("gV")->AsMatrix();
	//mP = Effects::mHudFX->GetVariableByName("gP")->AsMatrix();

	//mTechnique = Effects::mModelFX ->GetTechniqueByName("Render");
	//mfxModelLightWVPVar = Effects::mm->GetVariableByName("gLightWVP")->AsMatrix();

	mDiff = diff;
	mSpec = spec;

	//Get texture
	//mEffectDiffMapVar = Effects::mHudFX ->GetVariableByName("gDiffuseMap")->AsShaderResource();
	//mEffectSpecMapVar = Effects::mHudFX ->GetVariableByName("gSpecMap")->AsShaderResource();

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

void OtherModels::Update()
{
	if(mID == 1)
	{
		D3DXMatrixScaling(&S, mScale.x, mScale.y, mScale.z);
		D3DXMatrixRotationX(&Rx, mRotation.x);
		D3DXMatrixRotationY(&Ry, mRotation.y);
		D3DXMatrixRotationZ(&Rz, mRotation.z);
	
		mPosition.y += gDeltaTime * 0.5f;

		D3DXMatrixTranslation(&T, mPosition.x,  mStartPos.y + sinf(mPosition.y) * 5.0f, mPosition.z);
	}
	else if(mID == 2)
	{
		D3DXMatrixScaling(&S, mScale.x, mScale.y, mScale.z);
		//mPosition.y += gDeltaTime;
		mRotation.y += gDeltaTime * 0.1f;
		D3DXMatrixRotationY(&Ry, mRotation.y);
		//D3DXMatrixTranslation(&T, mPosition.x,  sinf(mPosition.y) * 20.0f, mPosition.z);
	}
	else if(mID == 3)
	{
		//mRotation.x += gDeltaTime * 0.1f;
		mRotation.y += gDeltaTime * 0.1f;
		mRotation.z += gDeltaTime * 0.1f;

		D3DXMatrixScaling(&S, mScale.x, mScale.y, mScale.z);
		D3DXMatrixRotationX(&Rx, mRotation.x);
		D3DXMatrixRotationY(&Ry, mRotation.y);
		D3DXMatrixRotationZ(&Rz, mRotation.z);
	}
	else if(mID == 4)
	{
		mRotation.x += gDeltaTime * 0.15f;
		//mRotation.y += gDeltaTime * 0.15f;
		mRotation.z += gDeltaTime * 0.15f;

		D3DXMatrixScaling(&S, mScale.x, mScale.y, mScale.z);
		D3DXMatrixRotationX(&Rx, mRotation.x);
		D3DXMatrixRotationY(&Ry, mRotation.y);
		D3DXMatrixRotationZ(&Rz, mRotation.z);
	}

	W = S * Rx * Ry * Rz * T;
}

void OtherModels::Draw()
{
	if(mID == 2)
	{
		mW = Effects::mHudFX->GetVariableByName("gWorld")->AsMatrix();
		mV = Effects::mHudFX->GetVariableByName("gV")->AsMatrix();
		mP = Effects::mHudFX->GetVariableByName("gP")->AsMatrix();
		mEffectDiffMapVar = Effects::mHudFX ->GetVariableByName("gDiffuseMap")->AsShaderResource();
		mEffectSpecMapVar = Effects::mHudFX ->GetVariableByName("gSpecMap")->AsShaderResource();
		mTechnique = Effects::mHudFX ->GetTechniqueByName("Render");
		mfxModelLightWVPVar = Effects::mHudFX->GetVariableByName("gLightWVP")->AsMatrix();
		mDevice->IASetInputLayout(InputLayout::mHud);
	}
	else
	{
		mW = Effects::mModelFX->GetVariableByName("gWorld")->AsMatrix();
		mV = Effects::mModelFX->GetVariableByName("gV")->AsMatrix();
		mP = Effects::mModelFX->GetVariableByName("gP")->AsMatrix();
		mEffectDiffMapVar = Effects::mModelFX ->GetVariableByName("gDiffuseMap")->AsShaderResource();
		mEffectSpecMapVar = Effects::mModelFX ->GetVariableByName("gSpecMap")->AsShaderResource();
		mTechnique = Effects::mModelFX ->GetTechniqueByName("Render");
		mfxModelLightWVPVar = Effects::mModelFX->GetVariableByName("gLightWVP")->AsMatrix();
		mDevice->IASetInputLayout(InputLayout::mModel);
	}

	mEffectDiffMapVar->SetResource(mDiff);
	mEffectSpecMapVar->SetResource(mSpec);
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