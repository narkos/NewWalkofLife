//PIXEL SHADER
#include "LightComputations.fx"
Texture2D txDiffuse : register(t0);
//sampler Sampler : register(s0);
SamplerState sampAni : register(s0)
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

};
struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float2 Tex		: TEXCOORD;
	float4 Nor		: NORMAL;
	float4 wPos		: POSITION;
};

float4 PS_main(VS_OUT input) : SV_Target
{


	LightingResult lightCalcs = ComputeLighting(input.wPos, normalize(input.Nor));

	float4 Texdiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		if (Material.UseTexture == 1)
		{
			Texdiffuse = txDiffuse.Sample(sampAni, input.Tex);
		}

	float4 emissive = Material.Emissive;
		float4 ambient = GlobalAmbient*Material.Ambient;
		float4 diffuse = lightCalcs.Diffuse* Material.Diffuse;
		float4 specular = lightCalcs.Specular*Material.Specular;

		float4 finalColor = (ambient + diffuse + specular) * Texdiffuse;

		return	float4(1.0f, 0.1f, 0.0f, 1.0f);


};
