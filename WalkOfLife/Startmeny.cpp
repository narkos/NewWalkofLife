#include "Startmeny.h"

Startmenu::Startmenu()
{

}

void Startmenu::setstartmeny(bool set)
{
	startmenyactive = set;
}
bool Startmenu::getstartmeny()
{
	return startmenyactive;
}
void Startmenu::CreateTextures(ID3D11Device* gDevice)
{
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/black.dds", nullptr, &blackscreen);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Walkoflife2.dds", nullptr, &Walkoflife);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Highscorebox6.dds", nullptr, &HSbox);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Play_1.dds", nullptr, &RutTex1);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Play_2.dds", nullptr, &RutTex12);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/High_1.dds", nullptr, &RutTex2);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/High_2.dds", nullptr, &RutTex22);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Exit_1.dds", nullptr, &RutTex3);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Exit_2.dds", nullptr, &RutTex32);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/matare.dds", nullptr, &RutTex4);
	
	/////


}

void Startmenu::ActivestartMenu(ID3D11DeviceContext* gDeviceContext, float width, float height, IDXGISwapChain* gSwapChain, bool HS)
{

	StartspriteBatch1.reset(new DirectX::SpriteBatch(gDeviceContext));
	StartspriteBatch2.reset(new DirectX::SpriteBatch(gDeviceContext));
	StartspriteBatch3.reset(new DirectX::SpriteBatch(gDeviceContext));
	StartspriteBatch4.reset(new DirectX::SpriteBatch(gDeviceContext));

	//Drawing the menu
	StartspriteBatch4->Begin();
	//StartspriteBatch4->Draw(blackscreen, DirectX::XMFLOAT2(width / 2, 100));
	StartspriteBatch4->Draw(Walkoflife, DirectX::XMFLOAT2(width / 4.75f, 10.0f));
	if (HS == TRUE)
	{
		//StartspriteBatch3->Begin();
		StartspriteBatch4->Draw(HSbox, DirectX::XMFLOAT2(width / 1.4f, 150.0f));

		//StartspriteBatch3->End();
	}
	StartspriteBatch4->End();
	

	if (currentTab == 1)
	{
		//MenuspriteBatch2.reset(new DirectX::SpriteBatch(gDeviceContext));
		//MenuspriteBatch3.reset(new DirectX::SpriteBatch(gDeviceContext));

		StartspriteBatch1->Begin();
		StartspriteBatch1->Draw(RutTex1, DirectX::XMFLOAT2(width / 3.5f, 250.0f));
		StartspriteBatch1->Draw(RutTex22, DirectX::XMFLOAT2(width / 3.5f, 400.0f));
		StartspriteBatch1->Draw(RutTex32, DirectX::XMFLOAT2(width / 3.5f, 550.0f));
		StartspriteBatch1->End();
	}
	else if (currentTab == 2)
	{
		//MenuspriteBatch.reset(new DirectX::SpriteBatch(gDeviceContext));
		//MenuspriteBatch3.reset(new DirectX::SpriteBatch(gDeviceContext));
		StartspriteBatch2->Begin();
		StartspriteBatch2->Draw(RutTex12, DirectX::XMFLOAT2(width / 3.5f, 250.0f));
		StartspriteBatch2->Draw(RutTex2, DirectX::XMFLOAT2(width / 3.5f, 400.0f));
		StartspriteBatch2->Draw(RutTex32, DirectX::XMFLOAT2(width / 3.5f, 550.0f));

		StartspriteBatch2->End();
		
	}
	else if (currentTab == 3)
	{
		//MenuspriteBatch.reset(new DirectX::SpriteBatch(gDeviceContext));
		//MenuspriteBatch2.reset(new DirectX::SpriteBatch(gDeviceContext));
		StartspriteBatch3->Begin();
		StartspriteBatch3->Draw(RutTex12, DirectX::XMFLOAT2(width / 3.5f, 250.0f));
		StartspriteBatch3->Draw(RutTex22, DirectX::XMFLOAT2(width / 3.5f, 400.0f));
		StartspriteBatch3->Draw(RutTex3, DirectX::XMFLOAT2(width / 3.5f, 550.0f));

		StartspriteBatch3->End();
	}
	//MenuspriteBatch->Draw(RutTex3, DirectX::XMFLOAT2(0, 10));
	//MenuspriteBatch->Draw(RutTex4, DirectX::XMFLOAT2(0, 30));
	//Drawing the menu



	gSwapChain->Present(0, 0);


}


int Startmenu::getCurrentTab()
{
	return currentTab;
}

void Startmenu::setCurrentTab(int tab)
{
	currentTab = tab;
}

void Startmenu::menuInit(HINSTANCE hInstance, HWND hWindow)
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

int Startmenu::menuInput(HWND hWindow)
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


	if (keyboardState[DIK_UP] || keyboardState[DIK_W])
	{

		return 1;

	}

	if (keyboardState[DIK_DOWN] || keyboardState[DIK_S])
	{
		//scrollDown();
		return 2;
	}

	if (keyboardState[DIK_RETURN] || keyboardState[DIK_X])		//Moves right when the D key or the RIGHT arrow is pressed
	{
		//chosenTab = currentTab;
		return 3;
	}
	if (keyboardState[DIK_H])		//Moves right when the D key or the RIGHT arrow is pressed
	{
		//chosenTab = currentTab;

		return 4;
	}
	if (chosenTab == 4 || chosenTab == 1)
		PostMessage(hWindow, WM_DESTROY, 0, 0);


	return 0;

}
