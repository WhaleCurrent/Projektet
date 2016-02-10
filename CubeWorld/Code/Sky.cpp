#include "Sky.h"

Sky::Sky(string filename, ID3D10Device* device, ID3D10ShaderResourceView* diff)
{
	mModelData = new LoadModel(filename);
	mModelSize = mModelData->getModelDataSize();
	mModel = new VertexPosNorTex[mModelSize];
	for(int i = 0; i < mModelSize; i++)
	{
		mModel[i].pos.x = mModelData->getModelPosition(i).x;
		mModel[i].pos.y = mModelData->getModelPosition(i).y;
		mModel[i].pos.z = mModelData->getModelPosition(i).z;

		mModel[i].normal.x = mModelData->getModelNormal(i).x;
		mModel[i].normal.y = mModelData->getModelNormal(i).y;
		mModel[i].normal.z = mModelData->getModelNormal(i).z;

		mModel[i].texC = mModelData->getModelTexture(i);
	}

	mDevice = device;

	mShaderResourceDiff = diff;

	mW = Effects::mSkyFX->GetVariableByName("gWorld")->AsMatrix();
	mVP = Effects::mSkyFX->GetVariableByName("gVP")->AsMatrix();

	mTechnique = Effects::mSkyFX->GetTechniqueByName("Render");

	//Get texture
	mEffectDiffMapVar = Effects::mSkyFX->GetVariableByName("gDiffuseMap")->AsShaderResource();


	//init buffer with defined line list data
	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(VertexPosNorTex) * mModelSize;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vinitData;
	 vinitData.pSysMem = mModel;

	device->CreateBuffer(
		&vbd,		//description of buffer to create
		&vinitData, //data to initialize buffer with
		&mVertexBuffer);		//return the created buffer
}

void Sky::Update()
{
	//W = S * Rx * Ry * Rz * T;
}

void Sky::Draw()
{
	D3DXVECTOR3 eyePos = GetCamera().mPosition;

	// center Sky about eye in world space
	D3DXMatrixTranslation(&W, eyePos.x, eyePos.y, eyePos.z);
	mW->SetMatrix((float*)W);
	mVP->SetMatrix((float*)(GetCamera().mView * GetCamera().mProj));

	mEffectDiffMapVar->SetResource(mShaderResourceDiff);
	

	mDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDevice->IASetInputLayout(InputLayout::mSky);

	UINT stride = sizeof(VertexPosNorTex);
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

void Sky::setTexture(ID3D10ShaderResourceView* diff)
{
	mShaderResourceDiff = diff;
}