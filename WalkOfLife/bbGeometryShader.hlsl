
cbuffer World : register (c0)
{
	float4x4 View;
	float4x4 Projection;
	float4x4 WorldSpace;
	float4x4 InvWorld;
	float4x4 WVP;

	//Shadow Calculations
	matrix lightView;
	matrix lightProjection;
};

static const float4 g_positions[4] =
{
	float4(-1, 1, 0, 0), float4(1, 1, 0, 0), float4(-1, -1, 0, 0), float4(1, -1, 0, 0)
};
static const float2 gsTex[4] =
{
	float2(0.0f, 1.0f),
	float2(1.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 0.0f)

	/*float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 1.0f),
	float2(1.0f, 0.0f)*/
};

struct BBVS_OUT
{
	float3 wCenter : POSITION;

	float2 wSize : TEXCOORD;

};

struct BBGS_OUT
{
	float4 posH: SV_POSITION;
	float3 posW: POSITION;
//	float3 norW: NORMAL;
	float2 tex: TEXCOORD;
	
};

[maxvertexcount(4)]
void main(point BBVS_OUT bbgs_in[1]/* : SV_POSITION*/, inout TriangleStream< BBGS_OUT > bbgs_out
)
{

	BBGS_OUT poutput;
	float4x4 WorldView = mul(WorldSpace, View);
	float4 viewPos = mul(float4(bbgs_in[0].wCenter, 1.0f), WorldView);


	float halfWidth = 0.5*bbgs_in[0].wSize.x;
	float halfHeight = 0.5*bbgs_in[0].wSize.y;




	for (int i = 0; i < 4; i++)
	{
		poutput.posH = mul(viewPos + float4(
											g_positions[i].x * halfWidth,
											g_positions[i].y * halfHeight, 0.0f, 0.0f), 
											Projection);

		poutput.posW = viewPos + float4(	g_positions[i].x * halfWidth,
											g_positions[i].y * halfHeight, 0.0f, 0.0f);
		poutput.tex = gsTex[i];
		bbgs_out.Append(poutput);
	}
	bbgs_out.RestartStrip();


	
}