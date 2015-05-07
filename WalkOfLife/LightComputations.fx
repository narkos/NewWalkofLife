#define MAX_LIGHTS 8

#define L_DIRECTIONAL 1
#define L_POINT 2
#define L_SPOT 3


struct Light
{
	float4	Position;
	float4	Color;
	int		Type;
	int		Active;
	int2	pad;
	float4	Direction;
	float	SpotConeAngle;
	float	AttConst;
	float	AttLinear;
	float	AttQuadratic;
};

struct MatInfo
{
	float4	Emissive;
	float4	Ambient;
	float4	Diffuse;
	float4	Specular;
	float	SpecPow;
	int	UseTexture;
	float2	Padding;
};

cbuffer MaterialProperties : register(b0)
{
	MatInfo Material;
};

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

cbuffer LightProperties : register(b1)
{
	float4 CamPosition;
	float4 GlobalAmbient;

	Light lights[MAX_LIGHTS];
}

// Variable and Parameter Abbreviations Glossary
//		L = Light Vector
//		N = Surface Normal
//		V = Point to Eye Vector
//		P = Point in World Space



float4 calcDiffuse(Light light, float3 L, float4 N)
{
	float diffCalc;
	diffCalc = max(0, dot(L, N));	//Returns 0 if surface isn't facing camera.
	return light.Color * diffCalc;
}

float4 calcSpecular(Light light, float3 V, float3 L, float4 N)
{
	//Phong
	float3 R = normalize(reflect(-L, N));
	float RdotV = max(0, dot(N, V));

	// Blin Phong
	float3 H = normalize(L + V);
	float NdotH = max(0, dot(N, H));

	return light.Color * pow(RdotV, Material.SpecPow);
	
}

float calcAtt(Light light, float distance)
{
	return (1.0f / (light.AttConst + light.AttLinear * distance + light.AttQuadratic * distance*distance));
}


LightingResult createPointLight(Light light, float3 V, float4 P, float4 N)
{
	LightingResult result;

	float3 L = (light.Position - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = calcAtt(light, distance);

	result.Diffuse = calcDiffuse(light, L, N)*attenuation;
	result.Specular = calcSpecular(light, V, L, N) * attenuation;


	return result;

}

LightingResult createDirectional(Light light, float3 V, float4 P, float4 N)
{
	LightingResult result;
	float3 L = -light.Direction.xyz;

	result.Diffuse = calcDiffuse(light, L, N);
	result.Specular = calcSpecular(light, V, L, N);

	return result;

}
//
//float calcSpotCone(Light light, float3 L)
//{
//
//}
//
//LightingResult createSpotLight(Light light, float3 V, float4 P, float3 N)
//{
//
//}

LightingResult ComputeLighting(float4 P, float4 N)
{
	LightingResult finalResult = { { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } };
	
	float3 V = normalize(CamPosition - P).xyz;

		[unroll]
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		LightingResult result = { { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } };
		if (lights[i].Active != 1) continue; // Continue if light isn't active

		switch (lights[i].Type)
		{
		case L_DIRECTIONAL:
		{
			result = createDirectional(lights[i], V, P, N);
		}
		break;
		case L_POINT:
		{
			result = createPointLight(lights[i], V, P, N);
		}
		break;
		case L_SPOT:
		{
			//result = createSpotLight(lights[i], V, P, N);
		}
		break;

		}
		finalResult.Diffuse += result.Diffuse;
		finalResult.Specular += result.Specular;
	}
	finalResult.Diffuse = saturate(finalResult.Diffuse);
	finalResult.Specular = saturate(finalResult.Specular);

	return finalResult;
}