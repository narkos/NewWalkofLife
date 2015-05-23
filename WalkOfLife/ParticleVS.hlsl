//VERTEX SHADER
cbuffer World : register (c0)
{
	matrix View;
	matrix Projection;
	matrix WorldSpace;
	matrix InvWorld;
	matrix WVP;

	//Shadow Calculations
	matrix lightView;
	matrix lightProjection;
};

struct VS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
	
};

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float2 Tex		: TEXCOORD;

};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output;

	output.Pos = mul(float4(input.Pos, 1.0f), WVP);
	output.Tex = input.Tex;
	
	return output;
};
