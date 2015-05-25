#pragma once

#pragma comment (lib, "dinput8.lib")	//Keyboard/mouse input
#pragma comment (lib, "dxguid.lib")		//Keyboard/mouse input
#include <dinput.h>						//Keyboard/mouse input
#include "RenderEngine.h"

class Input
{
public:
	~Input(){
		DIKeyboard->Release();
		DIMouse->Release();
		DirectInput->Release();
	}
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;
	DIMOUSESTATE mouseLastState;
	
	LPDIRECTINPUT8 DirectInput;
	void initInput(HINSTANCE hInstance, HWND);
	int detectInput(HWND);
	bool detectJump(HWND);
	bool detectDash(HWND);
	bool detectCameraLean(HWND);
	int detectRenderState(HWND);
};