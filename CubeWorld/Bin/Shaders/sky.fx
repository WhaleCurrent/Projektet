Texture2D gDiffuseMap;

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gVP;
};

struct VS_IN
{
	float3 posL		: POSITION;
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
	vOut.posW = mul(float4(vIn.posL, 1.0f), gWorld);
	vOut.posH = mul(float4(vOut.posW, 1.0f), gVP);
	vOut.posH.z = vOut.posH.w - 0.0001f;
	

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
	// Make sure the depth function is LESS_EQUAL and not just LESS.  
	// Otherwise, the normalized depth values at z = 1 (NDC) will 
	// fail the depth test if the depth buffer was cleared to 1.
    DepthFunc = LESS_EQUAL;
};

// Pixel Shader
float4 PS( PS_IN pIn )  : SV_Target
{
	//Get materials from texture maps.
	float4 diffuse = gDiffuseMap.Sample(gTriLinearSam, pIn.texC);

    return float4(diffuse) * (1 - pIn.posH.z / pIn.posH.w);
}

// Render
technique10 Render
{
    pass P0
    {
        SetVertexShader( CompileShader(   vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(	  ps_4_0, PS() ) );

		SetRasterizerState(NoCull);
        //SetDepthStencilState(LessEqualDSS, 0);
    }
}
