#include "ModelType.h"

ModelType::ModelType(string filename, ID3D10Device* device, ID3D10ShaderResourceView* diff, ID3D10ShaderResourceView* spec)
{
	type = filename;

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

	mV = Effects::mModelFX->GetVariableByName("gV")->AsMatrix();
	mP = Effects::mModelFX->GetVariableByName("gP")->AsMatrix();

	mTechnique = Effects::mModelFX ->GetTechniqueByName("Render");
	mfxModelLightWVPVar = Effects::mModelFX->GetVariableByName("gLightWVP")->AsMatrix();

	mDiff = diff;
	mSpec = spec;

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

void ModelType::Update(float dt)
{
	mEffectDiffMapVar->SetResource(mDiff);
	mEffectSpecMapVar->SetResource(mSpec);
	mDevice->IASetInputLayout(InputLayout::mModel);
	mDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mV->SetMatrix((float*)GetCamera().mView);
	mP->SetMatrix((float*)GetCamera().mProj);

	UINT stride = sizeof(VertexPos4NorTex);
	UINT offset = 0;
	mDevice->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
}

void ModelType::Draw()
{
	//D3DXMATRIX lightWVP = W*A*B;
	//mfxModelLightWVPVar->SetMatrix((float*)lightWVP);

	//Draw triangles
	D3D10_TECHNIQUE_DESC techDesc;
	mTechnique->GetDesc( &techDesc );
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        mTechnique->GetPassByIndex( p )->Apply( 0 );
		mDevice->Draw(mModelSize, 0);
    }
}

void ModelType::setTexture(ID3D10ShaderResourceView* diff)
{
	mDiff = diff;
}