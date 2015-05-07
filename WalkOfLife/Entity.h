#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#endif

#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>
#include <vector>

using namespace DirectX;
using namespace std;
//using namespace DirectX::TriangleTests;

class Entity{
protected:
	bool isActive; //ska vi rendera detta eller inte? jag l�gger denna h�r ifall vi tex vill kunna st�nga av ljus ox�
	bool isStatic;



	XMMATRIX rot;
	XMMATRIX scale;

public:
	XMMATRIX pos;
	float xPos;
	float yPos;
	Entity(XMFLOAT3 pos, bool isActive, bool isStatic){
		this->isActive = isActive;
		this->isStatic = isStatic;
		this->xPos = pos.x;
		this->yPos = pos.y;

		Translate(pos.x, pos.y, pos.z);
		rot = XMMatrixIdentity(); //den rotationen och skalningen som den importeras in som kommer vara standard v�rdet
		scale = XMMatrixIdentity();
	}

	Entity(){}
	~Entity(){

	}

	XMMATRIX world;
	void Translate(float x, float y, float z){
		pos = XMMatrixTranslation(this->xPos + x, this->yPos + y, z);
	}

	void Rotate(XMVECTOR rotAxis, float rotValue){
		rot = XMMatrixRotationAxis(rotAxis, rotValue);
	}

	void Scale(float x, float y, float z){
		scale = XMMatrixScaling(x, y, z);
	}

	void CalculateWorld(){ //denna ska kallas innan man skickar in den i GPUn
		world = scale * rot * pos; //st�mmer detta?
	}

	void SetActive(bool a){
		isActive = a; //anv�nds ifall vi vill st�nga av detta object, tex n�r man plockar upp den
	}

	bool GetActive(){ return isActive; }

};