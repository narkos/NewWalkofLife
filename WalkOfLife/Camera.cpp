#include "Camera.h"

Camera::Camera()
{
	CamView = XMMatrixLookAtLH(XMVectorSet((cameraXPos + cameraXPosOffset), 0.0f, -15.0f, 1.0f), XMVectorSet((cameraXPos + cameraXPosOffset), (cameraXPos + 1.0), 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0, 0.0f));
	CamProjection = XMMatrixPerspectiveFovLH(3.14f*(0.45f), 640.0f / 480.0f, 0.5f, 50.0f);
}

//////////

void Camera::updateCamera()
{
	//http://www.gamedev.net/topic/574628-world-space-to-screen-space-co-ordinates/g

	//NED TO GET CHARACTER POS IN SCREEN SPACE TO FIX SMOOTHNESS

	//if (theCharacter->xPos > (cameraXPos + 4.0) && theCharacter->xPos <= (cameraXPos + 6.0))
	//{
	//	cameraXPos = theCharacter->xPos - cameraXPos;
	//	//cameraXPos = theCharacter->xPos - (4.0);
	//}

	if (playerXPos > (cameraXPos + cameraXPosOffset))
	{
		cameraXPos = playerXPos - cameraXPosOffset;
		//cameraPosOffset = 0.0f;
	}
	if (playerXPos < (cameraXPos - cameraXPosOffset))
	{
		cameraXPos = playerXPos + cameraXPosOffset;
		//cameraPosOffset = 6.0f;
	}

	if (playerYPos > (cameraYPos + cameraYPosOffset))
	{
		cameraYPos = playerYPos - cameraYPosOffset;
	}
	if (playerYPos < (cameraYPos - cameraYPosOffset))
	{
		cameraYPos = playerYPos + cameraYPosOffset;
	}

	//Translate all of the above to screen space functionality

	CamView = XMMatrixLookAtLH(XMVectorSet((cameraXPos + cameraXPosOffset), (cameraYPos + cameraYPosOffset), -15.0f, 1.0f), XMVectorSet((cameraXPos + cameraXPosOffset), (cameraYPos + 1.0), 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0, 0.0f));
	CamProjection = XMMatrixPerspectiveFovLH(3.14f*(0.45f), 640.0f / 480.0f, 0.5f, 50.0f);
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