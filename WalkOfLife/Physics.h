#pragma once
#include "PlayerObject.h"
#include "Platform.h"
#include "CollectableObject.h"
#include "Collision.h"
class Physics
{
public:
	Physics();
	~Physics();
	void Gravitation(Collision, PlayerObject*);
	void Jump(Collision, PlayerObject*);
	bool onPlatform;
protected:
	float downforce;
	float upforce;
	
};

