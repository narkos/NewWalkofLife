//PIXEL SHADER
#include "LightComputations.fx"
Texture2D txDiffuse : register(t1);
//sampler Sampler : register(s0);
SamplerState sampAni : register(s0);

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


		float4	Texdiffuse = txDiffuse.Sample(sampAni, input.Tex);

	
		//return	float4(1.0,0,0,1);

		return Texdiffuse;
};

