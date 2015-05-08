#include "PlayerObject.h"

void PlayerObject::Move(bool right)
{
	
	if (right)
	{
		if (this->momentum < 1)
		{
			this->momentum += 0.05;
		}
		
		this->Translate(this->getSpeed() * momentum, 0.0, 0.0);
		//this->Rotate(XMLoadFloat3(&XMFLOAT3(0,0,1)), this->xPos);
		//this->Scale(5, 1, 1);
		this->xPos += this->getSpeed() * momentum;
	}
		
	else
	{
		if (this->momentum < 1)
		{
			this->momentum += 0.05;
		}
		
		this->Translate(-this->getSpeed() * momentum, 0.0, 0.0);
		this->xPos -= this->getSpeed() * momentum;
	}
}

void PlayerObject::Jump()
{
	
}

float PlayerObject::getSpeed()
{
	return this->speed;
}

float PlayerObject::getJumpHeight()
{
	return this->jumpHeight;
}

int PlayerObject::getDivision()
{
	return this->division;
}

void PlayerObject::setDivision(int division)
{
	this->division = division;
}

void PlayerObject::UpdateDivision(int pixelsPerDivision)
{
	if ((this->xPos >= division * pixelsPerDivision) && (this->xPos < (division * pixelsPerDivision) + pixelsPerDivision))
	{
		return;
	}

	else if (this->xPos < division * pixelsPerDivision)
	{
		this->division = this->division - 1;
	}

	else if (this->xPos >= (division * pixelsPerDivision) + pixelsPerDivision)
	{
		this->division = this->division + 1;
	}
	
}