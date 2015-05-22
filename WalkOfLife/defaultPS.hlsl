//PIXEL SHADER
#include "LightComputations.fx"
Texture2D txDiffuse : register(t0);
//Texture2D ObjNormMap: register(t1); //-----------------> normap
//sampler Sampler : register(s0);
SamplerState sampAni : register(s0);

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float2 Tex		: TEXCOORD;
	float4 Nor		: NORMAL;
	float4 wPos		: POSITION;
	float4 tangent : TANGENT;
};

//cbuffer cbPerObject : register(b1)
//{
//	float4x4 WVP;
//	float4x4 World;
//	bool hasNormMap;
//};

float4 PS_main(VS_OUT input) : SV_Target
{
	
	//if (hasNormMap == true)
	//{
	//	float4 normalMap = ObjNormMap.Sample(sampAni, input.Tex);
	//	//Change normal map range from [0, 1] to [-1, 1]
	//	normalMap = (2.0f*normalMap) - 1.0f;
	//	//
	//	float3 biTangent = cross(input.normal, input.tangent);
	//	biTangent = normalize(biTangent);

	//	float3x3 texSpace = float3x3(input.tangent, biTangent, input.normal);
	//	input.normal = mul(normalMap, texSpace);
	//	normalize(input.normal);
	//}

	LightingResult lightCalcs = ComputeLighting(input.wPos, normalize(input.Nor));


			float4 Texdiffuse = txDiffuse.Sample(sampAni, input.Tex);
		

		float4 emissive = Material.Emissive;
		float4 ambient = GlobalAmbient*Material.Ambient;
		float4 diffuse = lightCalcs.Diffuse* Material.Diffuse;
		float4 specular = lightCalcs.Specular*Material.Specular;

		float4 finalColor = (ambient + diffuse + specular) * Texdiffuse;

		return (finalColor);
		

};


//Texture2D txDiffuse : register(t0);
//
//SamplerState sampAni
//{
//	Filter = ANISOTROPIC;
//	MaxAnisotropy = 4;
//	AdressU = WRAP;
//	AdressV = WRAP;
//};
//
//
//
//
//struct VS_OUT
//{
//	float3 Pos		: POSITION;
//	float2 Tex		: TEXCOORD;
//	float4 tunormal : NORMAL;
//	float4 wPos		: SV_POSITION;
//};
//
//float4 PS_main(VS_OUT input) : SV_Target
//
//{
//
//	float4 Texdiffuse = txDiffuse.Sample(sampAni, input.Tex);
//
//	return Texdiffuse;
//	//return float4(1.0f,0.0f,0.0f,0.0f);
//
//};