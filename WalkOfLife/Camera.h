#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

using namespace DirectX;

class Camera{

private:
	float WINDOW_WIDTH = 1200.0f;
	float WINDOW_HEIGHT = 900.0f;

	float cameraXPos = 0.0f;
	float cameraYPos = 0.0f;
	float cameraXPosOffset = 6.0f;
	float cameraYPosOffset = 3.0f;

	float playerXPos;
	float playerYPos;

	// Sets camera pos and angle
	XMMATRIX CamView;
	XMMATRIX CamProjection;
public:
	Camera();

	void updateCamera();

	float characterXPos;

	void setCharacterXPos(float pos);
	float getCharacterXPos();

	void setCameraXPos(float pos);
	float getCameraXPos();

	void setCameraYPos(float pos);
	float getCameraYPos();

	float getWindowWidth();
	float getWindowHeight();

	void setPlayerXPos(float pos);
	void setPlayerYPos(float pos);

	XMMATRIX getCamProjection();
	XMMATRIX getCamView();
};

#endif