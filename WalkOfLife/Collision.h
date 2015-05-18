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

	void TestCollision(vector<Platform> &platforms);
	bool TestCollisionDeadly(vector<Platform> &platforms);
	XMFLOAT2 TestCollision(vector<CollectableObject> &collectables);
	//void TestCollision(vector<Platform> platforms, vector<CollectableObject> collectables);


	bool rightValid(){ return canGoRight; }
	bool leftValid(){ return canGoLeft; }
	bool upValid(){ return canGoUp; }
	bool isGrounded(){ return onGround; }

private:
	//används för att kolla hur spelaren kommer kunna flytta sig
	PlayerObject *player;
	Platform groundPlatform;

	bool canGoRight;
	bool canGoLeft;
	bool canGoUp;
	bool onGround;
	
	float yValue; //den på marken under spelaren
};