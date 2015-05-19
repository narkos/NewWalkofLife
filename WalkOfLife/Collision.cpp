#include "Collision.h"

void Collision::TestCollision(vector<Platform> &platforms, PlayerObject& player){

	this->distDown = 0;
	this->canGoRight = true;
	this->canGoLeft = true;
	this->canGoUp = true;

	//this->onGround = false;

	//for each (Platform p in platforms)
	//{
	//	if (player->TestRight(p) == true){ //den träffa nått till höger
	//		canGoRight = false;		
	//		
	//	}
	//	if (player->TestLeft(p) == true){ //borde nog vara mindre än 10, men testa!
	//		canGoLeft = false;
	//		
	//	}
	//	if (player->TestUp(p) == true){ 
	//		canGoUp = false;			
	//	}

	//	if (player->TestDown(p) == true){
	//		groundPlatform = p; //sen använder man denna för att kolla vilket y värde
	//		this->onGround = true;
	//		//testa först mot triangeln
	//		
	//	}
	//	
	//}

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
	this->onGround = tempOnGround;
	}

bool Collision::TestCollisionDeadly(vector<Platform> &platforms){


	//this->onGround = false;

	//for each (Platform p in platforms)
	//{
	//	if (player->TestRight(p) == true){ //den träffa nått till höger
	//		canGoRight = false;		
	//		
	//	}
	//	if (player->TestLeft(p) == true){ //borde nog vara mindre än 10, men testa!
	//		canGoLeft = false;
	//		
	//	}
	//	if (player->TestUp(p) == true){ 
	//		canGoUp = false;			
	//	}

	//	if (player->TestDown(p) == true){
	//		groundPlatform = p; //sen använder man denna för att kolla vilket y värde
	//		this->onGround = true;
	//		//testa först mot triangeln
	//		
	//	}
	//	
	//}

	for (int i = 0; i < platforms.size(); i++){
		if (player->TestRight(platforms[i]) == true){ //den träffa nått till höger
			canGoRight = false;

		}
		if (player->TestLeft(platforms[i]) == true){ //borde nog vara mindre än 10, men testa!
			canGoLeft = false;

		}
		if (player->TestUp(platforms[i]) == true){
			canGoUp = false;
		}

	}
	bool tempOnGround = false;
	for (int i = 0; i < platforms.size(); i++){
		if (player->TestDown(platforms[i], this->onGround) == true){
			//groundPlatform = platforms[i]; //sen använder man denna för att kolla vilket y värde
			tempOnGround = true;
			break;
			//testa först mot triangeln
		}
		else
			tempOnGround = false;

	}
	return tempOnGround;
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