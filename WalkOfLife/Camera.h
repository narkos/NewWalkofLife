#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

using namespace DirectX;

class Camera{

private:
	float WINDOW_WIDTH = 1280;		//The game window width.
	float WINDOW_HEIGHT = 720;		//The game window height.

	float cameraXPos = 0.0f;
	float cameraYPos = 0.0f;
	float cameraZPos = -10.0f;

	float cameraXPosOffset = 6.0f;		//How far back in the screen the character will be.
	float cameraYPosOffset = 1.0f;		//How high the camera will be relative to the character.

	float damping = 0.03f;				//The value wich determines the "smoothness" of the camera. (Lower = Smoother).

	float playerXPos;
	float playerYPos;

	float rightWall = 6.0f;				//The Right wall's position on the invisible cube in wich the player can move freely.
	float leftWall = 5.0f;				//The Left wall's position.
	float upperWall = 4.0f;				//The Upper wall's position.
	float lowerWall = (-3.0f);			//The Lower wall's position.

	float focusY;
	float currYFocus = 0.0f;

	// Sets camera pos and angle
	XMMATRIX CamView;					//The Camera View Matrix
	XMMATRIX CamProjection;				//The Camera Projection Matrix

public:
	Camera();

	void updateCamera();
	void leanCamera(bool state);

	float characterXPos;

	void setCharacterXPos(float pos);
	float getCharacterXPos();

	void setCameraXPos(float pos);
	float getCameraXPos();

	void setCameraYPos(float pos);
	float getCameraYPos();

	void setCameraZPos(float pos);
	float getCameraZPos();

	float getWindowWidth();
	float getWindowHeight();

	void setPlayerXPos(float pos);
	void setPlayerYPos(float pos);

	XMMATRIX getCamProjection();
	XMMATRIX getCamView();
};

#endif