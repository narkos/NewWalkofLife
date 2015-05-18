#include "Menuclass.h"

Menu::Menu()
{


}

void Menu::CreateTextures(ID3D11Device* gDevice)
{ 
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Ress_1.dds", nullptr, &RutTex1);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Ress_2.dds", nullptr, &RutTex12);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/High_1.dds", nullptr, &RutTex2);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/High_2.dds", nullptr, &RutTex22);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Exit_1.dds", nullptr, &RutTex3);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Exit_2.dds", nullptr, &RutTex32);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Resume.dds", nullptr, &RutTex4);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Resume2.dds", nullptr, &RutTex42);
	
}

void Menu::ActiveMenu(ID3D11DeviceContext* gDeviceContext, float width, float height, IDXGISwapChain* gSwapChain)
{
	
	MenuspriteBatch.reset(new DirectX::SpriteBatch(gDeviceContext));
	MenuspriteBatch2.reset(new DirectX::SpriteBatch(gDeviceContext));
	MenuspriteBatch3.reset(new DirectX::SpriteBatch(gDeviceContext));
	
	//Drawing the menu
	
	
	if (currentTab == 1)
	{
		//MenuspriteBatch2.reset(new DirectX::SpriteBatch(gDeviceContext));
		//MenuspriteBatch3.reset(new DirectX::SpriteBatch(gDeviceContext));
		MenuspriteBatch->Begin();
		MenuspriteBatch->Draw(RutTex12, DirectX::XMFLOAT2(width / 3.5, 100));
		MenuspriteBatch->Draw(RutTex2, DirectX::XMFLOAT2(width / 3.5, 300));
		MenuspriteBatch->Draw(RutTex3, DirectX::XMFLOAT2(width / 3.5, 500));
		MenuspriteBatch->End();
	}
	else if (currentTab == 2)
	{
		//MenuspriteBatch.reset(new DirectX::SpriteBatch(gDeviceContext));
		//MenuspriteBatch3.reset(new DirectX::SpriteBatch(gDeviceContext));
		MenuspriteBatch2->Begin();
		MenuspriteBatch2->Draw(RutTex1, DirectX::XMFLOAT2(width / 3.5, 100));
		MenuspriteBatch2->Draw(RutTex22, DirectX::XMFLOAT2(width / 3.5, 300));
		MenuspriteBatch2->Draw(RutTex3, DirectX::XMFLOAT2(width / 3.5, 500));
		
		MenuspriteBatch2->End();
	}
	else if (currentTab == 3)
	{
		//MenuspriteBatch.reset(new DirectX::SpriteBatch(gDeviceContext));
		//MenuspriteBatch2.reset(new DirectX::SpriteBatch(gDeviceContext));
		MenuspriteBatch3->Begin();
		MenuspriteBatch3->Draw(RutTex1, DirectX::XMFLOAT2(width / 3.5, 100));
		MenuspriteBatch3->Draw(RutTex2, DirectX::XMFLOAT2(width / 3.5, 300));
		MenuspriteBatch3->Draw(RutTex32, DirectX::XMFLOAT2(width / 3.5, 500));

		MenuspriteBatch3->End();
	}
	//MenuspriteBatch->Draw(RutTex3, DirectX::XMFLOAT2(0, 10));
	//MenuspriteBatch->Draw(RutTex4, DirectX::XMFLOAT2(0, 30));
	//Drawing the menu

	
	
	gSwapChain->Present(0, 0);
	

}
void Menu::scrollUp()
{
	int tabb = getCurrentTab();
	if (tabb>1)
		setCurrentTab(-1);
}

void Menu::scrollDown()
{
	int tabb = getCurrentTab();
	if (tabb<4)
		setCurrentTab(1);
}

int Menu::getCurrentTab()
{
	return currentTab;
}

void Menu::setCurrentTab(int tab)
{
	currentTab = tab;
}

void Menu::menuInit(HINSTANCE hInstance, HWND hWindow)
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

int Menu::menuInput(HWND hWindow)
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

void Menu::setPause(bool set)
{
	pause = set;
}
bool Menu::getPause()
{
	return pause;
}