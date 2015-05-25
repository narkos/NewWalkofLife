#pragma once
#include "CollisionObject.h"

class CollectableObject : public CollisionObject{
protected:

	int coinValue; //hur mycket denne ska ge när man collidar med den
	int timeValue;
	XMMATRIX temp;
	XMMATRIX temp2;
	XMVECTOR bajs;
	float rotationValue;

public:
	CollectableObject(int coinValue, int timeValue, ID3D11Buffer *b, XMFLOAT3 pos, bool isActive, bool isStatic, BoundingBox bbox, float xInter, float yInter, float xSpeed, float ySpeed) : CollisionObject(b, pos, isActive, isStatic, bbox, xInter, yInter, xSpeed, ySpeed){
		this->coinValue = coinValue;
		this->timeValue = timeValue;
		this->rotationValue = 2;
		//this->isCollectable = true;		
	}

	CollectableObject(){}
	~CollectableObject(){}

	//bool isCollectable(){ return isCollectable; }
	//bool isDeadly(){ return false; }

	void PickUp(){ //kallas när spelaren collidar med detta object
		isActive = false; //så att man inte kan fortsätta plocka upp
		//ge spelaren coins eller tid eller whatevs!
	}

	int GetCoinValue(){ return coinValue; }
	int GetTimeValue(){ return timeValue; }

	void rotate()
	{
		temp = XMMatrixTranslation(this->bbox.Center.x, this->bbox.Center.y, this->bbox.Center.z);
		temp2 = XMMatrixInverse(&bajs, temp);
		this->rot = XMMatrixMultiply(temp2, XMMatrixRotationY(rotationValue));
		//this->rot = XMMatrixMultiply(this->rot, XMMatrixRotationX(rotationValue));
		//this->rot = XMMatrixMultiply(this->rot, XMMatrixRotationZ(rotationValue));
		this->world = scale * rot * temp;
		this->rotationValue += 0.01f;
	}
};