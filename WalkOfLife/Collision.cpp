#include "Collision.h"


void Collision::TestCollision(vector<Platform> &platforms, vector<Platform> &platforms2, vector<Platform> &platforms3, PlayerObject& player){

	this->distDown = 0;
	this->canGoRight = true;
	this->canGoLeft = true;
	this->canGoUp = true;

	for (int i = 0; i < platforms.size(); i++){

		if (player.TestRight(platforms[i]) == true){ //den träffa nått till höger
			canGoRight = false;

		}
		if (player.TestLeft(platforms[i]) == true){ //borde nog vara mindre än 10, men testa!
			canGoLeft = false;

		}
		if (player.TestUp(platforms[i]) == true){
			canGoUp = false;
		}

	}
	bool tempOnGround = false;
	for (int i = 0; i < platforms.size(); i++){
		if (player.TestDown(platforms[i], this->onGround) == true){
			//groundPlatform = platforms[i]; //sen använder man denna för att kolla vilket y värde
			tempOnGround = true;
			break;
			//testa först mot triangeln
		}
		else
			tempOnGround = false;
		
	}

	for (int i = 0; i < platforms2.size(); i++){
			if (canGoRight)
			{
				if (player.TestRight(platforms2[i]) == true){ //den träffa nått till höger
					canGoRight = false;
			}
			}

			if (canGoLeft)
			{
				if (player.TestLeft(platforms2[i]) == true){ //borde nog vara mindre än 10, men testa!
					canGoLeft = false;
				}

			}

			if (canGoUp)
			{
				if (player.TestUp(platforms2[i]) == true){
					canGoUp = false;
			}
			
			}

		}

		if (!tempOnGround)
		{
			for (int i = 0; i < platforms2.size(); i++){
				if (player.TestDown(platforms2[i], this->onGround) == true){
					//groundPlatform = platforms[i]; //sen använder man denna för att kolla vilket y värde
					tempOnGround = true;
					break;
					//testa först mot triangeln
				}
				else
					tempOnGround = false;
		}
	

		
	}
	
		for (int i = 0; i < platforms3.size(); i++){
			if (canGoRight)
			{
				if (player.TestRight(platforms3[i]) == true){ //den träffa nått till höger

					canGoRight = false;
				}
			}

			if (canGoLeft)
			{
				if (player.TestLeft(platforms3[i]) == true){ //borde nog vara mindre än 10, men testa!

					canGoLeft = false;
				}

			}

			if (canGoUp)
			{
				if (player.TestUp(platforms3[i]) == true){
					canGoUp = false;
				}

			}

		}

		if (!tempOnGround)
		{

			for (int i = 0; i < platforms3.size(); i++){
				if (player.TestDown(platforms3[i], this->onGround) == true){

					//groundPlatform = platforms[i]; //sen använder man denna för att kolla vilket y värde
					tempOnGround = true;
					break;
					//testa först mot triangeln
				}
				else
					tempOnGround = false;
			}



		}
		
	

	this->onGround = tempOnGround;
	
	}

	bool Collision::TestCollisionDeadly(vector<Platform> &platforms, PlayerObject* player)
{
	for (int i = 0; i < platforms.size(); i++)
	{
		if (player->TestIntersect(platforms[i]) == true)
		{
		
			platforms[i].SetActive(false);
			return true;
		}
	}
	return false;
	
}


XMFLOAT2 Collision::TestCollision(vector<CollectableObject> &collectables, PlayerObject* player)
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