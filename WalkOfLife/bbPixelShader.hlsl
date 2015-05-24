
/*cbuffer cbPerFrame :register(c5)
{
	float3 camPosition;
};*/
Texture2D ParticleTexture : register(t0);
SamplerState sampAni : register(s0);

struct BBGS_OUT
{
	float4 posH: SV_POSITION;
	float3 posW: POSITION;
	//	float3 norW: NORMAL;
	float2 tex: TEXCOORD;

};
float4 main(BBGS_OUT bbps_in) : SV_TARGET
{

	float4 color = ParticleTexture.Sample(sampAni, bbps_in.tex);
	//clip(color.a - .25);
	return color;
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
	/*float3 outputColor = float3(0.0f, 0.0f, 0.0f);
		float3 diffuse = diffuseColor.xyz;
		if (hasTexture == true)
		{
			diffuse = ParticleTexture.Sample(LinearSampler, bbps_in.tex);
			return float4(diffuse, 1.0f);
		}
		else
		{
			return float4(float3(0.0f, 0.0f, 1.0f), 1.0f);
		}
*/
}