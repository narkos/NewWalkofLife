#pragma once

#include "Entity.h"

struct MatInfo
{
	MatInfo()
		: Emissive(0.0f, 0.0f, 0.0f, 1.0f),
		Ambient(0.1f, 0.1f, 0.1f, 1.0f),
		Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
		Specular(1.0f, 1.0f, 1.0f, 1.0f),
		SpecPow(128.0f),
		UseTexture(0){}

	MatInfo(XMFLOAT4 A, XMFLOAT4 D, XMFLOAT4 S, float SP)
	{
		Emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		Ambient = A;
		Diffuse = D;
		Specular = S;
		SpecPow = SP;
		UseTexture = 1;
	}

	XMFLOAT4 Emissive;
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	float SpecPow;
	int UseTexture;
	float Padding[2];

};

struct MaterialProperties
{
	MatInfo Material;
};

class GameObject : public Entity{
public:
	int texIndex;
	GameObject(ID3D11Buffer *b, XMFLOAT3 pos, bool isActive, bool isStatic, float xInter, float yInter, float xSpeed, float ySpeed) : Entity(pos, isActive, isStatic){
		this->vertexBuffer = b;
		xInterval = xInter;
		yInterval = yInter;
		this->xSpeed = xSpeed;
		this->ySpeed = ySpeed;
	}

	GameObject(){}

	~GameObject(){
		//vertexBuffer->Release();
		//indexBuffer->Release();
	}

	ID3D11Buffer* GetVertexBuffer(){
		return vertexBuffer;
	}

	void setVertexBuffer(ID3D11Buffer* VertexBufferz){
		vertexBuffer = VertexBufferz;
	}

	ID3D11Buffer* GetIndexBuffer(){
		return indexBuffer;
	}

	int nrElements;

	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;

	//Material Info
	MatInfo material;


protected:
	struct Triangle{
		XMVECTOR x, y, z;
	};
	//texture
	
};


namespace MatPresets
{
	// Material Presets
	const MatInfo Emerald(
		XMFLOAT4(0.0215f, 0.1745f, 0.0215f, 1.0f),
		XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f),
		XMFLOAT4(0.633f, 0.727811f, 0.633f, 1.0f),
		float(76.8f));
		
	const MatInfo Copper(
		XMFLOAT4(0.19125f, 0.0735f, 0.0225f, 1.0f),
		XMFLOAT4(0.7038f, 0.27048f, 0.0828f, 1.0f),
		XMFLOAT4(0.256777f, 0.137622f, 0.086014f, 1.0f),
		float(0.1f));
	const MatInfo Lambert(
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		float(0.0f));

	const MatInfo BlinnBase(
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f),
		XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f),
		float(0.3f));
}