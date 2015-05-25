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
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/matare.dds", nullptr, &RutTex4);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Highscorebox6.dds", nullptr, &HSbox);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/rePlay_1.dds", nullptr, &Replay);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/rePlay_2.dds", nullptr, &Replay2);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/black.dds", nullptr, &blackscreen);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/gameover.dds", nullptr, &GO_over);
	/////
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter_org.dds", nullptr, &Meter);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter.dds", nullptr, &Meter1);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter1.dds", nullptr, &Meter2);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter2.dds", nullptr, &Meter3);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter3.dds", nullptr, &Meter4);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter4.dds", nullptr, &Meter5);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter5.dds", nullptr, &Meter6);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter6.dds", nullptr, &Meter7);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter7.dds", nullptr, &Meter8);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter8.dds", nullptr, &Meter9);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter9.dds", nullptr, &Meter10);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter10.dds", nullptr, &Meter11);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/deathscreen.dds", nullptr, &deathscreen);


	
}

void Menu::gameover(ID3D11DeviceContext* gDeviceContext, float width, float height, IDXGISwapChain* gSwapChain, bool HS, bool reply)
{
	
	GOSpriteBatch.reset(new DirectX::SpriteBatch(gDeviceContext));
	GOSpriteBatch->Begin();
	GOSpriteBatch->Draw(blackscreen, DirectX::XMFLOAT2(width / 2, 100));
	GOSpriteBatch->Draw(GO_over, DirectX::XMFLOAT2(width / 3.5, 100));
	GOSpriteBatch->End();

	gSwapChain->Present(0, 0);


}
void Menu::ActiveMenu(ID3D11DeviceContext* gDeviceContext, float width, float height, IDXGISwapChain* gSwapChain, bool HS, bool reply)
{

	MenuspriteBatch.reset(new DirectX::SpriteBatch(gDeviceContext));
	MenuspriteBatch2.reset(new DirectX::SpriteBatch(gDeviceContext));
	MenuspriteBatch3.reset(new DirectX::SpriteBatch(gDeviceContext));
	GOSpriteBatch.reset(new DirectX::SpriteBatch(gDeviceContext));
	//Drawing the menu

	if (g_over == true)
	{
		
		if (Win == true)
		{
			GOSpriteBatch->Begin();
			GOSpriteBatch->Draw(blackscreen, DirectX::XMFLOAT2(0, 0));
			GOSpriteBatch->Draw(deathscreen, DirectX::XMFLOAT2(width / 4, 100));
			GOSpriteBatch->End();
		}
		if (Win == false)
		{
			GOSpriteBatch->Begin();
			GOSpriteBatch->Draw(blackscreen, DirectX::XMFLOAT2(0, 0));
			GOSpriteBatch->Draw(GO_over, DirectX::XMFLOAT2(width / 3.9, 100));
			GOSpriteBatch->End();
		}

	}
	else if (g_over == false)
	{

		if (currentTab == 1)
		{
			//MenuspriteBatch2.reset(new DirectX::SpriteBatch(gDeviceContext));
			//MenuspriteBatch3.reset(new DirectX::SpriteBatch(gDeviceContext));

			MenuspriteBatch->Begin();
			if (reply == false)
			{

				MenuspriteBatch->Draw(RutTex1, DirectX::XMFLOAT2(width / 3.5, 100));

			}

			else if (reply == true)
			{

				MenuspriteBatch->Draw(Replay, DirectX::XMFLOAT2(width / 3.5, 100));

			}
			MenuspriteBatch->Draw(RutTex22, DirectX::XMFLOAT2(width / 3.5, 300));
			MenuspriteBatch->Draw(RutTex32, DirectX::XMFLOAT2(width / 3.5, 500));
			MenuspriteBatch->End();

		}
		else if (currentTab == 2)
		{
			//MenuspriteBatch.reset(new DirectX::SpriteBatch(gDeviceContext));
			//MenuspriteBatch2.reset(new DirectX::SpriteBatch(gDeviceContext));
			//MenuspriteBatch2.reset(new DirectX::SpriteBatch(gDeviceContext));

			if (reply == false)
			{
				MenuspriteBatch2->Begin();
				MenuspriteBatch2->Draw(RutTex12, DirectX::XMFLOAT2(width / 3.5, 100));
				MenuspriteBatch2->Draw(RutTex2, DirectX::XMFLOAT2(width / 3.5, 300));
				MenuspriteBatch2->Draw(RutTex32, DirectX::XMFLOAT2(width / 3.5, 500));
				MenuspriteBatch2->End();
			}

			if (reply == true)
			{
				//MenuspriteBatch2.reset(new DirectX::SpriteBatch(gDeviceContext));
				MenuspriteBatch2->Begin();
				MenuspriteBatch2->Draw(Replay2, DirectX::XMFLOAT2(width / 3.5, 100));
				MenuspriteBatch2->Draw(RutTex2, DirectX::XMFLOAT2(width / 3.5, 300));
				MenuspriteBatch2->Draw(RutTex32, DirectX::XMFLOAT2(width / 3.5, 500));
				MenuspriteBatch2->End();
			}


		}
		else if (currentTab == 3)
		{
			//MenuspriteBatch.reset(new DirectX::SpriteBatch(gDeviceContext));
			//MenuspriteBatch2.reset(new DirectX::SpriteBatch(gDeviceContext));
			MenuspriteBatch3->Begin();
			if (reply == false)
			{

				MenuspriteBatch3->Draw(RutTex12, DirectX::XMFLOAT2(width / 3.5, 100));

			}

			else if (reply == true)
			{

				MenuspriteBatch3->Draw(Replay2, DirectX::XMFLOAT2(width / 3.5, 100));

			}
			MenuspriteBatch3->Draw(RutTex22, DirectX::XMFLOAT2(width / 3.5, 300));
			MenuspriteBatch3->Draw(RutTex3, DirectX::XMFLOAT2(width / 3.5, 500));
			MenuspriteBatch3->End();

		}
		//MenuspriteBatch->Draw(RutTex3, DirectX::XMFLOAT2(0, 10));
		//MenuspriteBatch->Draw(RutTex4, DirectX::XMFLOAT2(0, 30));
		//Drawing the menu


		if (HS == TRUE)
		{
			MenuspriteBatch3->Begin();
			MenuspriteBatch3->Draw(HSbox, DirectX::XMFLOAT2(width / 1.4, 100));

			MenuspriteBatch3->End();
		}
		gSwapChain->Present(0, 0);

	
	}
}


void Menu::menuInit(ID3D11DeviceContext* gDeviceContext)
{
	//MeterSpriteBatch.reset(new DirectX::SpriteBatch(gDeviceContext));
}

void Menu::setreplay(bool play)
{
	rePlay = play;
}

void Menu::Meterfunc(ID3D11DeviceContext* gDeviceContext, float width, int year)
{
	//mainCamera.getWindowWidth(), mainCamera.getWindowHeight()
	float meterwidth = width;
	MeterSpriteBatch.reset(new DirectX::SpriteBatch(gDeviceContext));

	if (year < 10)
	{
		MeterSpriteBatch->Begin();
		MeterSpriteBatch->Draw(Meter, DirectX::XMFLOAT2(meterwidth / 3.5, 10));
		MeterSpriteBatch->End();
	}
	else if (year >= 10 && year < 20){
		MeterSpriteBatch->Begin();
		MeterSpriteBatch->Draw(Meter1, DirectX::XMFLOAT2(meterwidth / 3.5, 10));
		MeterSpriteBatch->End();
	}
	else if (year >= 20 && year < 30){
		MeterSpriteBatch->Begin();
		MeterSpriteBatch->Draw(Meter2, DirectX::XMFLOAT2(meterwidth / 3.5, 10));
		MeterSpriteBatch->End();
	}
	else if (year >= 30 && year < 40){
		MeterSpriteBatch->Begin();
		MeterSpriteBatch->Draw(Meter3, DirectX::XMFLOAT2(meterwidth / 3.5, 10));
		MeterSpriteBatch->End();
	}
	else if (year >= 40 && year < 50){
		MeterSpriteBatch->Begin();
		MeterSpriteBatch->Draw(Meter4, DirectX::XMFLOAT2(meterwidth / 3.5, 10));
		MeterSpriteBatch->End();
	}
	else if (year >= 50 && year < 60){
		MeterSpriteBatch->Begin();
		MeterSpriteBatch->Draw(Meter6, DirectX::XMFLOAT2(meterwidth / 3.5, 10));
		MeterSpriteBatch->End();
	}
	else if (year >= 60 && year < 70){
		MeterSpriteBatch->Begin();
		MeterSpriteBatch->Draw(Meter7, DirectX::XMFLOAT2(meterwidth / 3.5, 10));
		MeterSpriteBatch->End();
	}
	else if (year >= 70 && year < 80){
		MeterSpriteBatch->Begin();
		MeterSpriteBatch->Draw(Meter8, DirectX::XMFLOAT2(meterwidth / 3.5, 10));
		MeterSpriteBatch->End();
	}
	else if (year >= 80 && year < 90){
		MeterSpriteBatch->Begin();
		MeterSpriteBatch->Draw(Meter9, DirectX::XMFLOAT2(meterwidth / 3.5, 10));
		MeterSpriteBatch->End();
	}
	else if (year >= 90 && year < 100){
		MeterSpriteBatch->Begin();
		MeterSpriteBatch->Draw(Meter10, DirectX::XMFLOAT2(meterwidth / 3.5, 10));
		MeterSpriteBatch->End();
	}
	else if (year >= 100){
		MeterSpriteBatch->Begin();
		MeterSpriteBatch->Draw(Meter11, DirectX::XMFLOAT2(meterwidth / 3.5, 10));
		MeterSpriteBatch->End();
	}

	
	

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
	if (keyboardState[DIK_K])		//Moves right when the D key or the RIGHT arrow is pressed
	{
		//chosenTab = currentTab;

		return 5;
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
bool  Menu::getreplay()
{
	return rePlay;
}
void Menu::setgameover(bool set)
{
	g_over = set;
}
bool Menu::getgameover()
{
	return g_over;
}

void Menu::setwin(bool set)
{
	Win = set;
}
bool Menu::getwin()
{
	return Win;
}