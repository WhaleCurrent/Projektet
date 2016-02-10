#include "InputLayouts.h"

ID3D10InputLayout* InputLayout::mModel			= NULL;
ID3D10InputLayout* InputLayout::mSky			= NULL;
ID3D10InputLayout* InputLayout::mHud			= NULL;
ID3D10InputLayout* InputLayout::mParticleFire	= NULL;
ID3D10InputLayout* InputLayout::mParticleMove	= NULL;

void InputLayout::InitAll(ID3D10Device* device)
{
	D3D10_PASS_DESC PassDesc;
	
	//Sky / Reflect / Model------------------------------------------------------------------------------
	D3D10_INPUT_ELEMENT_DESC vertexPosNorTexDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	Effects::mSkyFX->GetTechniqueByName("Render")->GetPassByIndex(0)->GetDesc(&PassDesc);
	device->CreateInputLayout(vertexPosNorTexDesc, 3, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &mSky);



	D3D10_INPUT_ELEMENT_DESC vertexPos4NorTexDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 16, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	 0, 28, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	Effects::mModelFX->GetTechniqueByName("Render")->GetPassByIndex(0)->GetDesc(&PassDesc);
	device->CreateInputLayout(vertexPos4NorTexDesc, 3, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &mModel);

	Effects::mHudFX->GetTechniqueByName("Render")->GetPassByIndex(0)->GetDesc(&PassDesc);
	device->CreateInputLayout(vertexPos4NorTexDesc, 3, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &mHud);




	D3D10_INPUT_ELEMENT_DESC particleDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, 32, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TYPE",     0, DXGI_FORMAT_R32_UINT,        0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	Effects::mFireFX->GetTechniqueByName("StreamOutTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
    device->CreateInputLayout(particleDesc, 6, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &mParticleFire);

	Effects::mMoveFX->GetTechniqueByName("StreamOutTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
    device->CreateInputLayout(particleDesc, 6, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &mParticleMove);
}