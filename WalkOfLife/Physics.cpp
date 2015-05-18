#include "Physics.h"


Physics::Physics()
{
	this->downforce = 0.01;
	this->upforce = 0;
	this->onPlatform = false;
}


Physics::~Physics()
{
}

void Physics::Gravitation(Collision theCollision, PlayerObject *theCharacter)
{
	if (theCharacter->jumping == true)
	{
		this->upforce = this->upforce * 1.30;
	}

	if (this->upforce > theCharacter->getJumpHeight())
	{
		theCharacter->jumping = false;
	}

	if(theCollision.isGrounded() && this->upforce <= 0)
	{
		this->onPlatform = true;
		this->downforce = 0.1;
		theCharacter->jumpMomentumState = false;
		theCharacter->jumpMomentumX = 0;
	}
	if (!theCollision.isGrounded() || this->upforce > 0)
	{
		if (!theCollision.isGrounded() && this->downforce < 1 && this->upforce - this->downforce < 0.1)
		{
			this->downforce = this->downforce + 0.01;
		}
		theCharacter->Translate(theCharacter->jumpMomentumX, (this->upforce - this->downforce)/2, 0.0);
		theCharacter->yPos += (this->upforce - this->downforce)/2;
		theCharacter->xPos += theCharacter->jumpMomentumX;
	}

	if (this->upforce > 0)
	{
		this->upforce -= this->downforce/7;
	}
}

void Physics::Jump(Collision theCollision, PlayerObject *theCharacter)
{
	this->upforce = 0.1;
	theCharacter->jumping = true;
}