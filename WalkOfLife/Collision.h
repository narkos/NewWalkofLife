#pragma once

#ifndef COLLISION_H
#define COLLISION_H
#endif

#include "PlayerObject.h"
#include "CollectableObject.h"
#include "Platform.h"



class Collision{
public:
	float distLeft;
	float distRight;
	float distUp;
	float distDown;


	Collision(PlayerObject *player){
		this->player = player;
	}

	Collision(){}

	void TestCollision(vector<Platform> platforms);
	//void TestCollision(vector<Platform> platforms, vector<CollectableObject> collectables);
	float GetYValueOnGround();

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