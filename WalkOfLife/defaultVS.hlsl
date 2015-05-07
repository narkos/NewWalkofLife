
//VERTEX SHADER
cbuffer World : register (c0)
{
	matrix View;
	matrix Projection;
	matrix WorldSpace;
	matrix InvWorld;
	matrix WVP;
};

struct VS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
	float3 Nor : NORMAL;

};

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float2 Tex		: TEXCOORD;
	float3 Nor		: NORMAL;
	float4 wPos		: POSITION;

};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output;

	output.Pos = mul(float4(input.Pos, 1.0f), WVP);
	output.Tex = input.Tex;
	output.Nor = mul(float4(input.Nor, 0.0f), InvWorld);
	output.wPos = float4(input.Pos, 1.0f);

	return output;
	//float4 outpos = float4(input.Pos, 1.0f);

	//	outpos = mul(outpos, world);
	//	//outpos = mul(outpos, WorldSpace);

	//outpos = mul(outpos, View);
	//outpos = mul(outpos, Projection);

	//float3 temppos = mul(input.Pos, WorldSpace);
	//	output.pos = float4(temppos, 1.0f);
	////output.pos = input.Pos;
	//output.Tex = input.Tex;
	//output.normals = float4(input.normals, 1.0f);
	////output.wPos = outpos;
	//output.wPos = outpos;
	//return output;
};
