//cbuffer cbPerObject : register(c0)
//{
//	float4x4 WVP;
//	float4x4 World;
//	float4 diffuseColor;
//	bool hasTexture;
//};
//cbuffer cbPerFrame:register(c5)
//{
//	float3 camPosition;
//	float pad1;
//};
struct BBVS_IN
{
	float3 Pos : POSITION;
	
	float2 Size : SIZE;
	
};


struct BBVS_OUT
{
	float3 wCenter : POSITION;
	
	float2 wSize : SIZE;
	
};

BBVS_OUT main( BBVS_IN bbvs_input ) //: SV_POSITION
{
	BBVS_OUT bbvsout = (BBVS_OUT)0;
	
	bbvsout.wCenter = bbvs_input.Pos;
	
	bbvsout.wSize = bbvs_input.Size;
	
	return bbvsout;
}