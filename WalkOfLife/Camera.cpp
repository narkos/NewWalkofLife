#include "Camera.h"

Camera::Camera()
{

	CamView = XMMatrixLookAtLH(XMVectorSet((cameraXPos + cameraXPosOffset), 0.0f, cameraZPos, 1.0f), XMVectorSet((cameraXPos + cameraXPosOffset), (cameraYPos), 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0, 0.0f));
	CamProjection = XMMatrixPerspectiveFovLH(3.14f*(0.45f), WINDOW_WIDTH / WINDOW_HEIGHT, 0.5f, 1000.0f);

}

//////////

void Camera::updateCamera()
{
	//Gets the players position in "Camera space" and bases the calculations on that.
	float playerToCameraX = playerXPos - cameraXPos;
	float playerToCameraY = playerYPos - cameraYPos;
	//-------------------------------------------------------------------------------

	//Calculations for when the camera should follow the player. 
	//Smooth follow kicks in when the player hits a "wall" (on the invisible cube in wich the player can move freely).
	if (playerToCameraX > rightWall)
	{
		cameraXPos = (playerXPos - playerToCameraX);
		cameraXPos = cameraXPos - ((rightWall - playerToCameraX) * damping);
	}
	
	if (playerToCameraX < leftWall)
	{
		cameraXPos = (playerXPos - playerToCameraX);
		cameraXPos = cameraXPos - ((leftWall - playerToCameraX) * damping);
	}

	if (playerToCameraY > upperWall)
	{
		cameraYPos = (playerYPos - playerToCameraY);
		cameraYPos = cameraYPos - ((upperWall - playerToCameraY) * damping);
	}

	if (playerToCameraY < lowerWall)
	{
		cameraYPos = (playerYPos - playerToCameraY);
		cameraYPos = cameraYPos - ((lowerWall - playerToCameraY) * damping);
	}

	//Sets and updates the camera's view- and projection matrices based of the calculations.
	CamView = XMMatrixLookAtLH(XMVectorSet((cameraXPos + cameraXPosOffset), (cameraYPos + cameraYPosOffset), cameraZPos, 1.0f), XMVectorSet((cameraXPos + cameraXPosOffset), (cameraYPos + cameraYPosOffset), 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0, 0.0f));
	CamProjection = XMMatrixPerspectiveFovLH(3.14f*(0.45f), WINDOW_WIDTH / WINDOW_HEIGHT, 0.5f, 1000.0f);

}

void Camera::leanCamera(bool state)
{
	if (state)
	{
		if (cameraZPos < (-3.0))
		{
			if (cameraYPos < (playerYPos + 3.0))
			{
				cameraYPos += 0.1f;
			}
			cameraZPos += 0.3f;
			CamView = XMMatrixLookAtLH(XMVectorSet((cameraXPos + cameraXPosOffset), (cameraYPos + cameraYPosOffset), cameraZPos, 1.0f), XMVectorSet((cameraXPos + cameraXPosOffset), (playerYPos), 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0, 0.0f));
		}
	}
	else
	{
		if (cameraZPos >(-10.0))
		{
			if (cameraYPos > playerYPos)
			{
				cameraYPos -= 0.1f;
			}
			cameraZPos -= 0.3f;
			CamView = XMMatrixLookAtLH(XMVectorSet((cameraXPos + cameraXPosOffset), (cameraYPos + cameraYPosOffset), cameraZPos, 1.0f), XMVectorSet((cameraXPos + cameraXPosOffset), (playerYPos), 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0, 0.0f));
		}
		//updateCamera();
	}

}

void Camera::setCharacterXPos(float pos)
{
	characterXPos = pos;
}

float Camera::getCharacterXPos()
{
	return characterXPos;
}

//////////

void Camera::setCameraXPos(float pos)
{
	cameraXPos = pos;
}

float Camera::getCameraXPos()
{
	return cameraXPos;
}

//////////

void Camera::setCameraYPos(float pos)
{
	cameraYPos = pos;
}

float Camera::getCameraYPos()
{
	return cameraYPos;
}

//////////

void Camera::setCameraZPos(float pos)
{
	cameraZPos = pos;
}

float Camera::getCameraZPos()
{
	return cameraZPos;
}


float Camera::getWindowWidth()
{
	return WINDOW_WIDTH;
}

float Camera::getWindowHeight()
{
	return WINDOW_HEIGHT;
}

//////////

void Camera::setPlayerXPos(float pos)
{
	playerXPos = pos;
}

void Camera::setPlayerYPos(float pos)
{
	playerYPos = pos;
}

//////////

XMMATRIX Camera::getCamProjection()
{
	return CamProjection;
}

XMMATRIX Camera::getCamView()
{
	return CamView;
}

//////////
//
//
//
//
//
//
//
//
//######################################################################################################################################################
//###																																				 ###
//###					  //	OLD CAMERA STUFF BELOW (SCREEN SPACE CONVERTING, WORLD SPACE FOLLOWING etc.)	//									 ###	
//###																																				 ###	
//######################################################################################################################################################


//NED TO GET CHARACTER POS IN SCREEN SPACE TO FIX SMOOTHNESS

//http://www.gamedev.net/topic/574628-world-space-to-screen-space-co-ordinates/g

//if (playerToCamera > 6.0f)
//{
//	cameraXPos = playerXPos - 6.0f;
//}

//if (playerYPos < (cameraYPos - cameraYPosOffset))
//{
//	cameraYPos = playerYPos + cameraYPosOffset;
//}

//////		SCREEN SPACE FUNCTIONALITY		////
//XMVECTOR LocalSpace = XMVector3Transform(XMVectorSet(playerXPos, playerYPos, 0.0f, 1.0f), (CamView * CamProjection));

//float screenX = (XMVectorGetX(LocalSpace) / (XMVectorGetZ(LocalSpace)) * (WINDOW_WIDTH / 2)) + (WINDOW_WIDTH / 2);
//float screenY = - (XMVectorGetY(LocalSpace) / (XMVectorGetZ(LocalSpace)) * (WINDOW_HEIGHT / 2)) + (WINDOW_HEIGHT / 2);

//if (screenX > (WINDOW_WIDTH * 0.5f))
//{
//	cameraXPos = playerXPos - cameraXPosOffset;
//}
//if (screenX > (WINDOW_WIDTH * 0.4f) && screenX < (WINDOW_WIDTH * 0.5f))
//{
//	//float dist = (WINDOW_WIDTH * 0.5f) - screenX;
//	float dist = (playerXPos - cameraXPosOffset);


//	cameraXPos = cameraXPos + (dist * 0.05f);
//	//cameraXPos = screenX - (WINDOW_WIDTH * 0.5f) - cameraXPosOffset;
//	//cameraXPos = cameraXPos - (((WINDOW_WIDTH * 0.5f) - screenX) * 0.01f);
//	//cameraXPos = cameraXPos + (((WINDOW_WIDTH * 0.5f) - (WINDOW_WIDTH * 0.4f)) * 0.5f) + 10.0f;
//}

////////
//if (playerXPos < (cameraXPos - cameraXPosOffset))
//{
//	cameraXPos = playerXPos + cameraXPosOffset;
//}
////////

//if (screenX < (WINDOW_WIDTH * 0.15))
//{
//	cameraXPos = playerXPos + cameraXPosOffset;
//}

//if (playerYPos > (cameraYPos + cameraYPosOffset))
//{
//	cameraYPos = playerYPos - cameraYPosOffset;
//}
//
//if (playerYPos < (cameraYPos - cameraYPosOffset))
//{
//	cameraYPos = playerYPos + cameraYPosOffset;
//}
////		SCREEN SPACE FUNCTIONALITY		////


////if (theCharacter->xPos > (cameraXPos + 4.0) && theCharacter->xPos <= (cameraXPos + 6.0))
////{
////	cameraXPos = theCharacter->xPos - cameraXPos;
////	//cameraXPos = theCharacter->xPos - (4.0);
////}


//	WORLD SPACE FUNCTIONALITY	//
//if (playerXPos > (cameraXPos + cameraXPosOffset))
//{
//	cameraXPos = playerXPos - cameraXPosOffset;
//	//cameraPosOffset = 0.0f;
//}
//if (playerXPos < (cameraXPos - cameraXPosOffset))
//{
//	cameraXPos = playerXPos + cameraXPosOffset;
//	//cameraPosOffset = 6.0f;
//}

//if (playerYPos > (cameraYPos + cameraYPosOffset))
//{
//	cameraYPos = playerYPos - cameraYPosOffset;
//}
//if (playerYPos < (cameraYPos - cameraYPosOffset))
//{
//	cameraYPos = playerYPos + cameraYPosOffset;
//}
//	WORLD SPACE FUNCTIONALITY	//
