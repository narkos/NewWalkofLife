//PIXEL SHADER
#include "LightComputations.fx"
Texture2D txDiffuse : register(t0);
//Texture2D ObjNormMap: register(t1); //-----------------> normap
Texture2D depthMapTexture : register(t1);	//The Shadow Map, it contains the scene depth buffer rendered from the light's perspective.
//sampler Sampler : register(s0);
SamplerState sampAni : register(s0);

cbuffer shadowSettings : register (b3)
{
	int shadowTesting;
	int pad1;
	int pad2;
	int pad3;
};

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float2 Tex		: TEXCOORD;
	float4 Nor		: NORMAL;
	float4 wPos		: POSITION;
	//float4 tangent : TANGENT;

	//Shadow Calculations
	float4 lightViewPos : TEXCOORD1;
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

	//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
	//SHADOW TESTING 4 DUMMIES
	//0 = Render the scene as usual WITHOUT shadows
	//1 = Render the scene as usual WITH shadows
	//2 = Render the depth value
	//3 = Render shadowed and non shadowed parts in two contrasts (Shadows = Black, Lit parts = White)
	int shadowTest = shadowTesting;
	//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

	if (shadowTest == 0 || shadowTest == 1)
	{
		int shadowz = 0;
		if (shadowTest == 0)
			shadowz = 1;

		LightingResult lightCalcs = ComputeLighting(input.wPos, normalize(input.Nor), input.lightViewPos, depthMapTexture, sampAni, shadowz);

		float4 Texdiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };

		if (Material.UseTexture == 1)
		{
			Texdiffuse = txDiffuse.Sample(sampAni, input.Tex);
			Texdiffuse = saturate(Texdiffuse);
		}

		float4 emissive = Material.Emissive;
		float4 ambient = GlobalAmbient*Material.Ambient;
		float4 diffuse = lightCalcs.Diffuse* Material.Diffuse;
		float4 specular = lightCalcs.Specular*Material.Specular;

		float4 finalColor = (ambient + diffuse + specular) * Texdiffuse;

		return finalColor;
	}

	///////-----/////-----/////-----/////-----/////-----/////-----/////-----/////-----/////-----/////-----
	//-----/////-----/////-----/////-----/////-----/////-----/////-----/////-----/////-----/////-----/////
	///////-----/////-----/////-----/////-----/////-----/////-----/////-----/////-----/////-----/////-----
	//-----/////-----/////-----/////-----/////-----/////-----/////-----/////-----/////-----/////-----/////

	if (shadowTest == 2)
	{
		//////ALL OF THE BELOW IS FOR SHADOW TESTING PURPOSES
		float bias = 0.001f;
		float2 projectTexCoord;
		projectTexCoord.x = input.lightViewPos.x / 2.0f + 0.5f;
		projectTexCoord.y = -input.lightViewPos.y / 2.0f + 0.5f;
		float depthValue = depthMapTexture.Sample(sampAni, projectTexCoord).r;	//We only sample the RED channel couse its a greyscale map

		float lightDepthValue = input.lightViewPos.z / input.lightViewPos.w;	//Calculate the depth of the light.
		lightDepthValue = lightDepthValue - bias;	//Subtract the bias from the lightDepthValue.

		return depthValue;
	}

	if (shadowTest == 3)
	{
		float bias = 0.001f;	//Set the bias value for fixing the floating point precision issues.
		float2 projectTexCoord;
		float depthValue;
		float lightDepthValue;

		input.lightViewPos.xyz /= input.lightViewPos.w;

		projectTexCoord.x = input.lightViewPos.x / 2.0f + 0.5f;
		projectTexCoord.y = -input.lightViewPos.y / 2.0f + 0.5f;

		float3 depthDiffuse = depthMapTexture.Sample(sampAni, projectTexCoord).r;
		if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
		{
			depthValue = depthMapTexture.Sample(sampAni, projectTexCoord).r;	//We only sample the RED channel couse its a greyscale map
			lightDepthValue = input.lightViewPos.z / input.lightViewPos.w;	//Calculate the depth of the light.
			lightDepthValue = lightDepthValue - bias;	//Subtract the bias from the lightDepthValue.
			if (lightDepthValue < depthValue)	//If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
			{
				return float4(1.0f, 1.0f, 1.0f, 1.0f);
				//return float4(0.0, 0.0, 0.3, 1.0);	//DARK BLUE
			}
		}
		//return float4(0.3, 0.0, 0.0, 1.0);		//DARK RED
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	return float4(0.3, 0.0, 0.0, 1.0);
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