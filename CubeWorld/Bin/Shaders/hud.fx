#include "light.fx"

Texture2D gDiffuseMap;

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
    AddressV = Wrap;
};

cbuffer cbPerFrame
{
	Light gLight;
	float3 gEyePosW;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gV;
	float4x4 gP;
};

struct VS_IN
{
	float4 posL		: POSITION;
	float3 normalL	: NORMAL;
	float2 texC		: TEXCOORD;
};

struct PS_IN
{
	float4 posH		: SV_POSITION;
	float3 posW		: POSITION;
	float3 normalW	: NORMAL;
	float2 texC		: TEXCOORD;
};


// Vertex Shader
PS_IN VS( VS_IN vIn )
{
	PS_IN vOut;

	vOut.texC = vIn.texC;
	vOut.normalW = mul(float4(vIn.normalL, 0.0f), gWorld);
	vOut.posW = mul(vIn.posL, gWorld);
	vOut.posH = mul(float4(vOut.posW, 1.0f), gV);
	vOut.posH = mul(vOut.posH, gP);

	return vOut;
}

RasterizerState NoCull
{
    CullMode = None;
};

DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

DepthStencilState LessEqualDSS
{
    DepthFunc = LESS_EQUAL;
};

// Pixel Shader
float4 PS( PS_IN pIn )  : SV_Target
{
	//Get materials from texture maps.
	float4 diffuse = gDiffuseMap.Sample(gTriLinearSam, pIn.texC);


	//Interpolating normal can make it not be of unit length so normalize it.
	//float3 normalW = normalize(pIn.normalW);

	//SurfaceInfo v = {pIn.posW, normalW, diffuse, float4(1,1,1,1)};

	//float3 litColor = ParallelLight(v, gLight, gEyePosW, 1.0f);

    return float4(diffuse);
}

// Render
technique10 Render
{
    pass P0
    {
        SetVertexShader( CompileShader(   vs_4_0, VS() ) );
		SetGeometryShader( NULL);
        SetPixelShader( CompileShader(	  ps_4_0, PS() ) );

		SetRasterizerState(NoCull);
		SetDepthStencilState(LessEqualDSS, 0);
    }
}
