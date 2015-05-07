#include "Entity.h"

#define MAX_LIGHTS 8

enum LightType
{
	l_Directional = 1,
	l_Point = 2,
	l_Spot = 3
};

struct Light
{
	//	General Light Properties
	XMFLOAT4	Position;
	XMFLOAT4	Color;
	int			Type;
	int			Active;
	int			pad[2];

	// Specific Light Properties
	XMFLOAT4	Direction;					//Spot and Directional
	float		SpotConeAngle;				//Spot
	float		AttConst;					//Spot and Point
	float		AttLinear;					//Spot and Point
	float		AttQuadratic;				//Spot and Point
	
	//Basic Directional Light Default Constructor
	Light() :
		Position(0.0f, 0.0f, 0.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f, 1.0f),
		Type(l_Directional),
		Active(0),
		Direction(0.0f, 0.0f, 1.0f, 0.0f),
		SpotConeAngle(XM_PIDIV2),
		AttConst(1.0f),
		AttLinear(0.0f),
		AttQuadratic(0.0f)
	{}

};

struct LightProperties
{
	XMFLOAT4 CamPosition;
	XMFLOAT4 GlobalAmbient;

	Light lights[MAX_LIGHTS];
};

class LightClass : public Entity{
protected:	
	
	ID3D11Buffer *lightBuffer;
	



public:
	LightClass(int type, XMFLOAT3 pos, bool isActive, bool isStatic);
	LightClass(){}
	~LightClass();

	Light lightObject;

	virtual void CreateLightBuffer(ID3D11Device* dev);

	void ToggleActive();
	void Render(ID3D11Device* dev, ID3D11DeviceContext* devcon);

};


//class LightClass : public Entity{
//protected:
//	float intensity;
//	int lightType;
//	ID3D11Buffer *lightBuffer;
//
//	virtual void CreateLightBuffer(ID3D11Device* dev);
//
//
//
//public:
//	LightClass(float intensity, XMFLOAT3 pos, bool isActive, bool isStatic) : Entity(pos, isActive, isStatic){
//		this->intensity = intensity;
//	}
//	~LightClass();
//	// Directional Light
//	struct DirLight
//	{
//		XMFLOAT4 ambient;
//		XMFLOAT4 diffuse;
//		XMFLOAT4 specular;
//		XMFLOAT3 direction;
//		float shaderpadding;
//	};
//
//	// Point Light
//	struct PointLight
//	{
//		XMFLOAT4 ambient;
//		XMFLOAT4 diffuse;
//		XMFLOAT4 specular;
//		XMFLOAT3 position;
//		float range;
//		XMFLOAT3 attenuation;
//		float shaderpadding;
//	};
//	// Spot Light
//	struct SpotLight
//	{
//
//	};
//	DirLight dLightData;
//	void CreateDirLight(ID3D11Device* dev,
//		XMFLOAT4 amb,
//		XMFLOAT4 diff,
//		XMFLOAT4 spec,
//		XMFLOAT3 direction);
//
//	void Render(ID3D11Device* dev, ID3D11DeviceContext* devcon);
//	//void UpdateBuffer(ID3D11Device* dev);
//
//};


//NOTES 
//		2015-04-29 Ändrade i project config -> general -> Intermediate Directory
//					från $(Platform)\$(Configuration)\
//					till $(Platform)\$(Configuration)\$(ProjectName)\
//					pga Fatal Error: C1041	