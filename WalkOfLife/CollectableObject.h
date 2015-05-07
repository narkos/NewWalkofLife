#pragma once
#include "CollisionObject.h"

class CollectableObject : public CollisionObject{
protected:

	int coinValue; //hur mycket denne ska ge när man collidar med den
	int timeValue;

public:
	CollectableObject(int coinValue, int timeValue, ID3D11Buffer *b, XMFLOAT3 pos, bool isActive, bool isStatic, BoundingBox bbox) : CollisionObject(b, pos, isActive, isStatic, bbox){
		this->coinValue = coinValue;
		this->timeValue = timeValue;

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
};