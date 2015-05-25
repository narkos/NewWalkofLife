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

	if (keyboardState[DIK_ESCAPE] & 0x80)						//Shuts the window when the ESCAPE key is pressed
	{
		PostMessage(hWindow, WM_DESTROY, 0, 0);
	}

	if (keyboardState[DIK_LEFT] || keyboardState[DIK_A])		//Moves left when the A key or the LEFT arrow is pressed
	{
		return 1;
		
	}
	if (keyboardState[DIK_RIGHT] || keyboardState[DIK_D])		//Moves right when the D key or the RIGHT arrow is pressed
	{
		return 2;
	}

	if (keyboardState[DIK_G])		
	{
		return 3;
	}

	if (keyboardState[DIK_R])
	{
		return 4;
	}
	if (keyboardState[DIK_1])
	{
		return 5;
	}
	if (keyboardState[DIK_2])
	{
		return 6;
	}
	if (keyboardState[DIK_3])
	{
		return 7;
	}
	if (keyboardState[DIK_M])
	{
		return 8;
	}
	if (keyboardState[DIK_N])
	{
		return 9;
	}
	

	return 0;
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

	if (keyboardState[DIK_UP] || keyboardState[DIK_W])		//Moves right when the D key or the RIGHT arrow is pressed
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

	if (keyboardState[DIK_LSHIFT] || keyboardState[DIK_SPACE])		//Moves right when the D key or the RIGHT arrow is pressed
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

	if (keyboardState[DIK_LCONTROL])
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

	if (keyboardState[DIK_F1])
	{
		return 0;
	}
	if (keyboardState[DIK_F2])
	{
		return 1;
	}
	if (keyboardState[DIK_F3])
	{
		return 2;
	}
	if (keyboardState[DIK_F4])
	{
		return 3;
	}
}