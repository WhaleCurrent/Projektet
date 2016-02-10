#include "Particle.h"


Particle::Particle()
{
	mFirstRun = true;
	mGameTime = 0.0f;
	mTimeStep = 0.0f;

	mEyePosW  = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	mEmitPosW = D3DXVECTOR4(0.0f, 10.0f, 0.0f, 1.0f);
	mUpdate = false;

}

Particle::~Particle()
{
	mInitVB->Release();
	mStreamOutVB->Release();
	mRandomTex->Release();
}

void Particle::init(ID3D10Device* device, ID3D10Effect* effects, UINT maxParticles, ID3D10ShaderResourceView* diff)
{
	isFire = false;
	md3dDevice = device;

	mMaxParticles = maxParticles;

	buildRandomTex();

	mTexture = diff;
	if(mTexture == NULL)
	{
		isFire = true;
	}
	mStreamOutTech  = effects->GetTechniqueByName("StreamOutTech");	
	mDrawTech       = effects->GetTechniqueByName("DrawTech");	
	mViewProjVar  = effects->GetVariableByName("gViewProj")->AsMatrix();
	mGameTimeVar  = effects->GetVariableByName("gGameTime")->AsScalar();
	mTimeStepVar  = effects->GetVariableByName("gTimeStep")->AsScalar();
	mEyePosVar    = effects->GetVariableByName("gEyePosW")->AsVector();
	mEmitPosVar   = effects->GetVariableByName("gEmitPosW")->AsVector();
	mRandomTexVar = effects->GetVariableByName("gRandomTex")->AsShaderResource();
	mTextureVar = effects->GetVariableByName("gTexArray")->AsShaderResource();
	mUpdateVar  = effects->GetVariableByName("gUpdate")->AsScalar();
	mParticleColorVar = effects->GetVariableByName("gColor")->AsVector();
	mSizeVar = effects->GetVariableByName("gSize")->AsVector();

	buildVB();
}

void Particle::update(float deltaTime, float gameTime)
{
	mGameTime = gameTime;
	mTimeStep = deltaTime;

	mEyePosW = D3DXVECTOR4(GetCamera().mPosition, 1.0f);

	//mEmitPosW = D3DXVECTOR4(GetCamera().mPosition, 1.0f);
}

void Particle::draw()
{
	//float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	//md3dDevice->OMSetBlendState(0, blendFactor, 0xffffffff);

	mViewProjVar->SetMatrix((float*)&(GetCamera().mView*GetCamera().mProj));
	mGameTimeVar->SetFloat(mGameTime);
	mTimeStepVar->SetFloat(mTimeStep);
	mEyePosVar->SetFloatVector((float*)&mEyePosW);
	mEmitPosVar->SetFloatVector((float*)&gParticleEmitPos);
	mRandomTexVar->SetResource(mRandomTex);
	mTextureVar->SetResource(mTexture);
	mUpdateVar->SetBool(updateParticle);
	mParticleColorVar->SetFloatVector((float*)&gParticleColor);
	mSizeVar->SetFloatVector((float*)&gParticleSize);

	md3dDevice->IASetInputLayout(InputLayout::mParticleFire);
    md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = sizeof(ParticleVertex);
    UINT offset = 0;

	if(mFirstRun)
		md3dDevice->IASetVertexBuffers(0, 1, &mInitVB, &stride, &offset);
	else
		md3dDevice->IASetVertexBuffers(0, 1, &mDrawVB, &stride, &offset);


	md3dDevice->SOSetTargets(1, &mStreamOutVB, &offset);

    D3D10_TECHNIQUE_DESC techDesc;
    mStreamOutTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mStreamOutTech->GetPassByIndex(p)->Apply(0);
        
		if(mFirstRun)
		{
			md3dDevice->Draw(1, 0);
			mFirstRun = false;
		}
		else
		{
			md3dDevice->DrawAuto();
		}
    }

	// done streaming-out--unbind the vertex buffer
	ID3D10Buffer* bufferArray[1] = {0};
	md3dDevice->SOSetTargets(1, bufferArray, &offset);

	// ping-pong the vertex buffers
	swap(mDrawVB, mStreamOutVB);


	// Draw the updated particle system we just streamed-out. 
	md3dDevice->IASetVertexBuffers(0, 1, &mDrawVB, &stride, &offset);
	mDrawTech->GetDesc(&techDesc);
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mDrawTech->GetPassByIndex(p)->Apply(0);
        
		md3dDevice->DrawAuto();
    }
	//md3dDevice->OMSetBlendState(0, blendFactor, 0xffffffff);
	
}

void Particle::buildVB()
{

	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_DEFAULT;
    vbd.ByteWidth = sizeof(ParticleVertex);
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;

	// The initial particle emitter has type 0 and age 0.  The rest
	// of the particle attributes do not apply to an emitter.
	ParticleVertex p;
	ZeroMemory(&p, sizeof(ParticleVertex));
	p.age  = 0.0f;
	p.type = 0; 
 
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &p;

	md3dDevice->CreateBuffer(&vbd, &vinitData, &mInitVB);
	

	// Create the ping-pong buffers for stream-out and drawing.
	vbd.ByteWidth = sizeof(ParticleVertex) * mMaxParticles;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER | D3D10_BIND_STREAM_OUTPUT;

    md3dDevice->CreateBuffer(&vbd, 0, &mDrawVB);
	md3dDevice->CreateBuffer(&vbd, 0, &mStreamOutVB);
}

void Particle::buildRandomTex()
{
	D3DXVECTOR4 randomValues[1024];

	for(int i = 0; i < 1024; ++i)
	{
		randomValues[i].x = RandF(-1.0f, 1.0f);
		randomValues[i].y = RandF(-1.0f, 1.0f);
		randomValues[i].z = RandF(-1.0f, 1.0f);
		randomValues[i].w = RandF(-1.0f, 1.0f);
	}

    D3D10_SUBRESOURCE_DATA initData;
    initData.pSysMem = randomValues;
	initData.SysMemPitch = 1024*sizeof(D3DXVECTOR4);
    initData.SysMemSlicePitch = 1024*sizeof(D3DXVECTOR4);


    D3D10_TEXTURE1D_DESC texDesc;
    texDesc.Width = 1024;
    texDesc.MipLevels = 1;
    texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texDesc.Usage = D3D10_USAGE_IMMUTABLE;
    texDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;
    texDesc.ArraySize = 1;

	ID3D10Texture1D* randomTex = 0;
    md3dDevice->CreateTexture1D(&texDesc, &initData, &randomTex);


    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
    viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;
	
    md3dDevice->CreateShaderResourceView(randomTex, &viewDesc, &mRandomTex);
}