#define MAX_LIGHTS 8

#define L_DIRECTIONAL 1
#define L_POINT 2
#define L_SPOT 3
struct Light
{
	float4	Position;
	float4	Color;
	float4	Direction;
	float	SpotConeAngle;
	float	AttConst;
	float	AttLinear;
	float	AttQuadratic;
	int		Type;
	int		Active;
	float	Range;
	float	Padding;	
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

cbuffer MaterialProperties : register(b1)
{
	MatInfo Material;
};

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

cbuffer LightProperties : register(b2)
{
	float4 CamPosition;
	float4 GlobalAmbient;

	Light lights[MAX_LIGHTS];
};

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

LightingResult ComputeLighting(float4 P, float4 N, float4 lightViewPos, Texture2D depthMap, SamplerState samplerType, int shadowz)
{
	//SHADOW MAPPING-----------////-----------////-----------////-----------////
	//Shadow mapping requires a bias adjustment when comparing the depth of the light and the depth of the object due to the low floating point precision of the depth map.
	float bias = 0.001f;	//Set the bias value for fixing the floating point precision issues.
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;

	lightViewPos.xyz /= lightViewPos.w;

	//Calculate the projected texture coordinates for sampling the shadow map (depth buffer texture) based on the light's viewing position
	//lightViewPos.xy is in [-1, 1], but to sample the shadow map we need [0, 1], so scale by ½ and add ½
	projectTexCoord.x = lightViewPos.x / 2.0f + 0.5f;
	projectTexCoord.y = -lightViewPos.y / 2.0f + 0.5f;
	//SHADOW MAPPING-----------////-----------////-----------////-----------////

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
			if (shadowz == 1)
			{
				//SHADOW MAPPING-----------////-----------////-----------////-----------////
				//Check if the projected coordinates are in the view of the light, if not then the pixel gets just an ambient value.
				//Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
				if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
				{
					//Now that we are in the view of the light we will retrieve the depth value from the shadow map (depthMap). 
					//The depth value we get from the texture translates into the distance to the nearest object.
					//Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
					depthValue = depthMap.Sample(samplerType, projectTexCoord).r;	//We only sample the RED channel couse its a greyscale map

					//Now that we have the depth of the object for this pixel we need the depth of the light to determine if it is in front or behind the object.
					//We get this from the lightViewPosition. Note that we need to subtract the bias from this or we will get the floating point precision issue.
					lightDepthValue = lightViewPos.z / lightViewPos.w;		//Calculate the depth of the light.
					lightDepthValue = lightDepthValue - bias;		//Subtract the bias from the lightDepthValue.

					//Now we perform the comparison between the light depth and the object depth. If the light is closer to us then no shadow.
					//But if the light is behind an object in the shadow map then it gets shadowed. Note that a shadow just means we only apply ambient light. 
					//Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
					if (lightDepthValue < depthValue)	//If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
					{
						result = createDirectional(lights[i], V, P, N);
					}
				}
				//SHADOW MAPPING-----------////-----------////-----------////-----------////
			}
			else
				result = createDirectional(lights[i], V, P, N);
		}
		break;
		case L_POINT:
		{
			result = createPointLight(lights[i], V, P, N);
		/*	float3 PtoL = lights[i].Position.xyz - P.xyz;
			float distance = length(PtoL);
			if (distance <= lights[i].Range)
			{
			result = createPointLight(lights[i], V, P, N);
			}
					*/	
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