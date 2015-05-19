#pragma once

#ifndef COLLISION_H
#define COLLISION_H
#endif

#include "PlayerObject.h"
#include "CollectableObject.h"
#include "Platform.h"

using namespace DirectX;

class Collision{
public:
	float distLeft;
	float distRight;
	float distUp;
	float distDown;


	Collision(PlayerObject *player){
	this->player = player;
		onGround = false;
	}

	Collision(){
		onGround = false;
	}

	void TestCollision(vector<Platform> &platforms, PlayerObject &theCharacter);
	XMFLOAT2 TestCollision(vector<CollectableObject> &collectables, PlayerObject *theCharacter);
	//void TestCollision(vector<Platform> platforms, vector<CollectableObject> collectables);


	bool rightValid(){ return canGoRight; }
	bool leftValid(){ return canGoLeft; }
	bool upValid(){ return canGoUp; }
	bool isGrounded(){ return onGround; }

private:
	//anv�nds f�r att kolla hur spelaren kommer kunna flytta sig
	PlayerObject *player;
	

	bool canGoRight;
	bool canGoLeft;
	bool canGoUp;
	bool onGround;
	
	
};