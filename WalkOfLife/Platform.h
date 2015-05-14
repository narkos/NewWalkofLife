#pragma once

#ifndef PLATFORM_H
#define PLATFORM_H
#endif

#include "CollisionObject.h"

class Platform : public CollisionObject{

protected:

public:
	bool isHitByPlayer;
	Platform(bool isDeadly, ID3D11Buffer *b, XMFLOAT3 pos, bool isActive, bool isStatic, BoundingBox bbox) : CollisionObject(b, pos, isActive, isStatic, bbox){ //och eventuellt ha med tex isBouncy
		this->isDeadly = isDeadly;
		isHitByPlayer = false;
	}
	Platform(){}
	~Platform(){}

	/*bool isCollectable(){ return false; }
	bool isDeadly(){ return isDeadly; }	*/
};