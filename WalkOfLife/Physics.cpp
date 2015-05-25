#include "Physics.h"


Physics::Physics()
{
	this->downforce = 0.01f;
	this->upforce = 0;
	this->onPlatform = false;
}


Physics::~Physics()
{
}

void Physics::Gravitation(Collision *theCollision, PlayerObject *theCharacter)
{
	/*if (theCharacter->jumping == true)
	{
		this->upforce = this->upforce * 1.70;
	}

	if (this->upforce > theCharacter->getJumpHeight())
	{
		theCharacter->jumping = false;
	}*/

	if (theCollision->isGrounded() && this->upforce <= 0.0f)
	{
		this->onPlatform = true;
		this->downforce = 0.1f;
		theCharacter->jumpMomentumState = false;
		if (someBool)
		{
			theCharacter->momentum = 1.0f;
			someBool = false;
			theCharacter->jumpMomentumX = 0.0f;
		}
		
	
		
		
		
	}
	if (!theCollision->isGrounded() || this->upforce > 0)
	{

		if (!theCollision->isGrounded() && this->downforce < 0.7f && this->upforce - this->downforce < 0.1f)
		{
			this->downforce = this->downforce + 0.01f;
		}
		theCharacter->Translate(theCharacter->jumpMomentumX, (this->upforce - this->downforce)/2, 0.0f);
		theCharacter->yPos += (this->upforce - this->downforce)/2;
		theCharacter->xPos += theCharacter->jumpMomentumX;
	}

	if (this->upforce > 0)
	{
		this->upforce -= this->downforce/7;
	}
}

void Physics::Jump(Collision *theCollision, PlayerObject *theCharacter)
{
	this->upforce = theCharacter->getJumpHeight();
	theCharacter->jumping = true;
}