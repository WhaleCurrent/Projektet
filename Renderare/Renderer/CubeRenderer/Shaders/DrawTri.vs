struct VSIn
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEX;
};

struct PSIn
{
	float4 Pos  : SV_Position;
	float3 Normal : NORMAL;
	float2 TexCoord : TEX;
};

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSIn VS_main(VSIn input)
{
	PSIn output = (PSIn)0;

	output.Pos = float4(input.Pos, 1);
	output.Normal = input.Normal;
	output.TexCoord = input.TexCoord;
		
	return output;
}