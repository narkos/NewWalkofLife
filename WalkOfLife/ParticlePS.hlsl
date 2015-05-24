Texture2D txDiffuse : register(t0);
SamplerState sampAni : register(s0);

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float2 Tex		: TEXCOORD;
	
};

float4 main(VS_OUT input) : SV_Target
{

	float4 Texdiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };

	Texdiffuse = txDiffuse.Sample(sampAni, input.Tex);
	Texdiffuse = saturate(Texdiffuse);
	

	return float4(1.0f, 1.0f, 1.0f, 1.0f);
};
