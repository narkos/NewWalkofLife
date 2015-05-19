#include "Collision.h"

void Collision::TestCollision(vector<Platform> &platforms, vector<Platform> &platforms2, vector<Platform> &platforms3){

	this->distDown = 0;
	this->canGoRight = true;
	this->canGoLeft = true;
	this->canGoUp = true;

	for (int i = 0; i < platforms.size(); i++){
		if (player->TestRight(platforms[i]) == true){ //den tr�ffa n�tt till h�ger
			canGoRight = false;

		}
		if (player->TestLeft(platforms[i]) == true){ //borde nog vara mindre �n 10, men testa!
			canGoLeft = false;

		}
		if (player->TestUp(platforms[i]) == true){
			canGoUp = false;
		}

	}
	bool tempOnGround = false;
	for (int i = 0; i < platforms.size(); i++){
		if (player->TestDown(platforms[i], this->onGround) == true){
			//groundPlatform = platforms[i]; //sen anv�nder man denna f�r att kolla vilket y v�rde
			tempOnGround = true;
			break;
			//testa f�rst mot triangeln
		}
		else
			tempOnGround = false;
		
	}

	for (int i = 0; i < platforms2.size(); i++){
			if (canGoRight)
			{
				if (player->TestRight(platforms2[i]) == true){ //den tr�ffa n�tt till h�ger
					canGoRight = false;
			}
			}

			if (canGoLeft)
			{
				if (player->TestLeft(platforms2[i]) == true){ //borde nog vara mindre �n 10, men testa!
					canGoLeft = false;
			}

			}

			if (canGoUp)
			{
				if (player->TestUp(platforms2[i]) == true){
					canGoUp = false;
			}
			
			}

		}

		if (!tempOnGround)
		{
			for (int i = 0; i < platforms2.size(); i++){
				if (player->TestDown(platforms2[i], this->onGround) == true){
					//groundPlatform = platforms[i]; //sen anv�nder man denna f�r att kolla vilket y v�rde
					tempOnGround = true;
					break;
					//testa f�rst mot triangeln
				}
				else
					tempOnGround = false;
		}
	

		
	}
	
		for (int i = 0; i < platforms2.size(); i++){
			if (canGoRight)
			{
				if (player->TestRight(platforms2[i]) == true){ //den tr�ffa n�tt till h�ger
					canGoRight = false;
				}
			}

			if (canGoLeft)
			{
				if (player->TestLeft(platforms2[i]) == true){ //borde nog vara mindre �n 10, men testa!
					canGoLeft = false;
				}

			}

			if (canGoUp)
			{
				if (player->TestUp(platforms2[i]) == true){
					canGoUp = false;
				}

			}

		}

		if (!tempOnGround)
		{
			for (int i = 0; i < platforms2.size(); i++){
				if (player->TestDown(platforms2[i], this->onGround) == true){
					//groundPlatform = platforms[i]; //sen anv�nder man denna f�r att kolla vilket y v�rde
					tempOnGround = true;
					break;
					//testa f�rst mot triangeln
				}
				else
					tempOnGround = false;
			}



		}
		
	

	this->onGround = tempOnGround;
	}

bool Collision::TestCollisionDeadly(vector<Platform> &platforms)
{


	for (int i = 0; i < platforms.size(); i++){
		if (player->TestRight(platforms[i]) == true){ //den tr�ffa n�tt till h�ger
			canGoRight = false;

		}
		if (player->TestLeft(platforms[i]) == true){ //borde nog vara mindre �n 10, men testa!
			canGoLeft = false;

		}
		if (player->TestUp(platforms[i]) == true){
			canGoUp = false;
		}

	}
	bool tempOnGround = false;
	for (int i = 0; i < platforms.size(); i++){
		if (player->TestDown(platforms[i], this->onGround) == true){
			//groundPlatform = platforms[i]; //sen anv�nder man denna f�r att kolla vilket y v�rde
			tempOnGround = true;
			break;
			//testa f�rst mot triangeln
		}
		else
			tempOnGround = false;

	}
	return tempOnGround;
}


XMFLOAT2 Collision::TestCollision(vector<CollectableObject> &collectables)
{
	for (int i = 0; i < collectables.size(); i++)
	{
		if (player->TestIntersect(collectables[i]) == true)
		{
			XMFLOAT2 temp;
			temp.x = collectables[i].GetCoinValue();
			temp.y = collectables[i].GetTimeValue();
			collectables[i].SetActive(false);
			return temp;
		}
	}

	return XMFLOAT2(0, 0);
}