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
	//this->player = player;
		onGround = false;
		canGoUp = true;
	}

	Collision(){
		onGround = false;
		canGoUp = true;
	}


	void TestCollision(vector<Platform*> platforms, vector<Platform*> platforms2, vector<Platform*> platforms3, PlayerObject *theCharacter);
	bool TestCollisionDeadly(vector<Platform*> platforms, PlayerObject* player);

	XMFLOAT2 TestCollision(vector<CollectableObject*> collectables, PlayerObject *theCharacter);

	//void TestCollision(vector<Platform> platforms, vector<CollectableObject> collectables);


	bool rightValid(){ return canGoRight; }
	bool leftValid(){ return canGoLeft; }
	bool upValid(){ return canGoUp; }
	bool isGrounded(){ return onGround; }

	void SetGrounded(bool b){ onGround = b; }
	void SetRightValid(bool b){ canGoRight = b; }
	void SetLeftValid(bool b){ canGoLeft = b; }
	void SetUpValid(bool b){ canGoUp = b; }

private:
	//används för att kolla hur spelaren kommer kunna flytta sig
	//PlayerObject *player;
	

	bool canGoRight;
	bool canGoLeft;
	bool canGoUp;
	bool onGround;
	
	
};