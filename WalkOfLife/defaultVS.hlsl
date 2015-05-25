
//VERTEX SHADER
cbuffer World : register (c0)
{
	matrix View;
	matrix Projection;
	float4x4 WorldSpace;
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
	float3 Tan : TANGENT;
};

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float2 Tex		: TEXCOORD;
	float3 Nor		: NORMAL;
	float4 wPos		: POSITION;
	float3 Tan : TANGENT;

	//Shadow Calculations
	float4 lightViewPos : TEXCOORD1;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	/*float3 c1 = cross(input.Nor, float3(0.0, 0.0, 1.0));
		float3 c2 = cross(input.Nor, float3(0.0, 1.0, 0.0));

		if (length(c1)>length(c2))
		{
			input.Tan = c1;
		}
		else
		{
			input.Tan = c2;
		}*/

	input.Tan = normalize(input.Tan);

	VS_OUT output;
	float4 tempTan = float4(input.Tan, 0.0f);
	output.Pos = mul(float4(input.Pos, 1.0f), WVP);
	output.Tex = input.Tex;
	output.Nor = mul(float4(input.Nor, 0.0), (float3x3)WorldSpace);
	//output.Tan = mul(tempTan, (float3x3)WorldSpace);
	output.Tan = mul(float4(input.Tan, 0.0), (float3x3)WorldSpace);
	output.wPos = mul(float4(input.Pos, 1.0f), WorldSpace);

	//Shadow Calculations
	//Calculate the vertex pos from the lights view
	output.lightViewPos = mul(float4(input.Pos, 1.0), WorldSpace);
	output.lightViewPos = mul(output.lightViewPos, lightView);
	output.lightViewPos = mul(output.lightViewPos, lightProjection);

	return output;
};
