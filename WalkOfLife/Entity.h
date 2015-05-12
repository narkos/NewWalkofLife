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


	float xInterval, yInterval;
	XMFLOAT3 currIntervalPosition;
public:
	XMFLOAT3 playerStartIntervalPosition; //anv�nds som ett sorts index f�r moving objects, se PlayerObject TestDown()
	XMMATRIX pos;
	float xPos;
	float yPos;
	Entity(XMFLOAT3 pos, bool isActive, bool isStatic){
		this->isActive = isActive;
		this->isStatic = isStatic;
		this->xPos = pos.x;
		this->yPos = pos.y;

		this->currIntervalPosition = XMFLOAT3(0, 0, 0);
		this->playerStartIntervalPosition = XMFLOAT3(0, 0, 0);
		xInterval = 12;
		yInterval = 0;

		if (pos.x < 0.000001f && pos.x > -0.0000001f && pos.y < 0.000001f && pos.y > -0.0000001f){
			this->pos = XMMatrixIdentity();
			this->world = XMMatrixIdentity();
		}
		else
			Translate(pos.x, pos.y, pos.z);

		rot = XMMatrixIdentity(); //den rotationen och skalningen som den importeras in som kommer vara standard v�rdet
		scale = XMMatrixIdentity();
	}

	Entity(){
		this->currIntervalPosition = XMFLOAT3(0, 0, 0);
	}
	~Entity(){

	}

	XMMATRIX world;
	void Translate(float x, float y, float z){
		pos = XMMatrixTranslation(this->xPos + x, this->yPos + y, z);
	}

	void TranslateExact(float x, float y, float z){
		//this->xPos = x;
		//this->yPos = y;
		pos = XMMatrixTranslation(x, y, z);
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
	bool GetStatic(){ return isStatic; }

	void PatrolInterval(float time){
		this->currIntervalPosition.x = sinf(time) * xInterval;
		this->currIntervalPosition.y = sinf(time) * yInterval;
		this->currIntervalPosition.z = 0;
		this->Translate(currIntervalPosition.x, currIntervalPosition.y, currIntervalPosition.z);
	}

	XMFLOAT3 GetCurrIntervalPos(){
		return currIntervalPosition;
	}

	XMFLOAT3 SubXMFLOAT3(XMFLOAT3 a, XMFLOAT3 b){
		return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	bool SameXMFLOAT3(XMFLOAT3 a, XMFLOAT3 b){
		if ((a.x - b.x < 0.00001f) && (a.x - b.x > -0.00001f) && (a.y - b.y < 0.00001f) && (a.y - b.y > -0.00001f) && (a.z - b.z < 0.00001f) && (a.z - b.z > -0.00001f))
			return true;
		else return false;
	}
};