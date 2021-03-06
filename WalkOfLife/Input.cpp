#include "Input.h"

void Input::initInput(HINSTANCE hInstance, HWND hWindow)
{
	HRESULT WINAPI DirectInput8Create(
		HINSTANCE hinst,
		DWORD dwVersion,
		REFIID riidltf,
		LPVOID *ppvOut,
		LPUNKNOWN punkOuter
		);

	HRESULT hr = DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DirectInput,
		NULL);


	hr = DirectInput->CreateDevice(GUID_SysKeyboard,
		&DIKeyboard,
		NULL);

	hr = DirectInput->CreateDevice(GUID_SysMouse,
		&DIMouse,
		NULL);

	
	hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = DIKeyboard->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	hr = DIMouse->SetCooperativeLevel(hWindow, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
}

int Input::detectInput(HWND hWindow)
{

	//DIMOUSESTATE mouseCurrentState;		//Mouse input
	BYTE keyboardState[256];			//Keyboard input

	DIKeyboard->Acquire();
	//DIMouse->Acquire();
	
	//DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrentState);
	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	if (keyboardState[DIK_LEFT] && 0x80 || keyboardState[DIK_A] && 0x80)		//Moves left when the A key or the LEFT arrow is pressed

	{
		return 1;
		
	}

	else if (keyboardState[DIK_RIGHT] && 0x80|| keyboardState[DIK_D]&& 0x80)		//Moves right when the D key or the RIGHT arrow is pressed

	{
		return 2;
	}


	else if (keyboardState[DIK_P] && 0x80)
	{
		return 3;
	}
	else if (keyboardState[DIK_R]&& 0x80)
	{
		return 4;
	}
	else if (keyboardState[DIK_1]&& 0x80)
	{
		return 5;
	}
	else if (keyboardState[DIK_2]&& 0x80)
	{
		return 6;
	}
	else if (keyboardState[DIK_3]&& 0x80)
	{
		return 7;
	}
	else if (keyboardState[DIK_M]&& 0x80)
	{
		return 8;
	}
	else if (keyboardState[DIK_N]&& 0x80)
	{
		return 9;
	}
	
	else if (keyboardState[DIK_C]&& 0x80)
	{
		return 11;
	}

	else if (keyboardState[DIK_8] && 0x80)
	{
		return 12;
	}
	else if (keyboardState[DIK_9] && 0x80)
	{
		return 13;
	}
	else if (keyboardState[DIK_0] && 0x80)
	{
		return 14;
	}



	else
	{
		return 0;
	}
	
	//if (keyboardState[DIK_UP] || keyboardState[DIK_W])			//Moves forward when the W key or the UP arrow is pressed
	//{
	//	moveFB = speed;
	//}
	//if (keyboardState[DIK_DOWN] || keyboardState[DIK_S])		//Moves backwards when the S key or the DOWN arrow is pressed
	//{
	//	moveFB = -speed;
	//}
}

bool Input::detectJump(HWND hWindow)
{
	BYTE keyboardState[256];

	DIKeyboard->Acquire();
	
	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);


	if (keyboardState[DIK_UP] && 0x80 || keyboardState[DIK_W] && 0x80)		//Moves right when the D key or the RIGHT arrow is pressed
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

bool Input::detectDash(HWND hWindow)
{
	BYTE keyboardState[256];

	DIKeyboard->Acquire();

	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);


	if (keyboardState[DIK_LSHIFT] && 0x80 || keyboardState[DIK_SPACE] && 0x80)		//Moves right when the D key or the RIGHT arrow is pressed
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Input::detectCameraLean(HWND)
{
	BYTE keyboardState[256];

	DIKeyboard->Acquire();

	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);


	if (keyboardState[DIK_LCONTROL] && 0x80)
	{
		return true;
	}
	else
		return false;
}

int Input::detectRenderState(HWND)
{
	BYTE keyboardState[256];

	DIKeyboard->Acquire();

	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);


	if (keyboardState[DIK_F1] && 0x80)
	{
		return 0;
	}
	if (keyboardState[DIK_F2] && 0x80)
	{
		return 1;
	}
	if (keyboardState[DIK_F3] && 0x80)
	{
		return 2;
	}
	if (keyboardState[DIK_F4] && 0x80)
	{
		return 3;
	}

	else
	{
		return 0;
	}

	//return 1000000;
}