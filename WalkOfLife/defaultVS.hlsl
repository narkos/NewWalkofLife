
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
	float3 Nor : NORMAL;
	//float3 Tan : TANGENT;
};

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float2 Tex		: TEXCOORD;
	float4 Nor		: NORMAL;
	float4 wPos		: POSITION;

	//Shadow Calculations
	float4 lightViewPos : TEXCOORD1;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output;

	output.Pos = mul(float4(input.Pos, 1.0f), WVP);
	output.Tex = input.Tex;
	output.Nor = (float4(input.Nor, 0.0f) );
	output.wPos = mul(float4(input.Pos, 1.0f), WorldSpace);

	//Shadow Calculations
	//Calculate the vertex pos from the lights view
	output.lightViewPos = mul(float4(input.Pos, 1.0), WorldSpace);
	output.lightViewPos = mul(output.lightViewPos, lightView);
	output.lightViewPos = mul(output.lightViewPos, lightProjection);

	return output;
};
