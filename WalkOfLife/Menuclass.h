#pragma once
#include <windows.h>
#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>
#include <DirectXMathVector.inl>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <DirectXCollision.h>

#pragma comment (lib, "dinput8.lib")	//Keyboard/mouse input
#pragma comment (lib, "dxguid.lib")		//Keyboard/mouse input
#include <dinput.h>						//Keyboard/mouse input
#include "Extra DirectXLibs\Inc\DDSTextureLoader.h"
#include "Extra DirectXLibs\Inc\SpriteFont.h"
#include "Extra DirectXLibs\Inc\SpriteBatch.h"
#include "Extra DirectXLibs\Inc\SimpleMath.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")




class Menu{

public:
	Menu();
	void menuInit(ID3D11DeviceContext* gDeviceContext);
	void scrollUp();
	void scrollDown();
	void setreplay(bool play);
	bool getreplay();
	void setgameover(bool set);
	bool getgameover();
	void Meterfunc(ID3D11DeviceContext* gDeviceContext, float width, int year);
	void CreateTextures(ID3D11Device* gDevice);
	std::unique_ptr<DirectX::SpriteBatch> MenuspriteBatch;
	std::unique_ptr<DirectX::SpriteBatch> MenuspriteBatch2;
	std::unique_ptr<DirectX::SpriteBatch> MenuspriteBatch3;
	std::unique_ptr<DirectX::SpriteBatch> MeterSpriteBatch;
	std::unique_ptr<DirectX::SpriteBatch> GOSpriteBatch;
	void ActiveMenu(ID3D11DeviceContext* gDeviceContext, float width, float height, IDXGISwapChain* SwapChain, bool HS, bool reply);

	/// Inputs
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;
	DIMOUSESTATE mouseLastState;

	LPDIRECTINPUT8 DirectInput;

	int menuInput(HWND);
	void menuInit(HINSTANCE hInstance, HWND);
	
	/// Inputs


	//------ get functions
	int getCurrentTab();
	void setCurrentTab(int tab);


	struct MenuStruct
	{
		int years;
		int months;
	};
	
	void setPause(bool set);
	bool getPause();
	void setwin(bool set);
	
	bool getwin();
	void Menurelease();
	void gameover(ID3D11DeviceContext* gDeviceContext, float width, float height, IDXGISwapChain* gSwapChain, bool HS, bool reply);
	
private:
	int t;
	int currentTab = 1;
	int chosenTab;
	bool pause = false;
	bool rePlay = false;
	bool g_over;
	int backtogame = 0;
	bool Win = false;
	ID3D11ShaderResourceView* RutTex1 = nullptr;
	ID3D11ShaderResourceView* RutTex12 = nullptr;
	ID3D11ShaderResourceView* RutTex2 = nullptr;
	ID3D11ShaderResourceView* RutTex22 = nullptr;
	ID3D11ShaderResourceView* RutTex3 = nullptr;
	ID3D11ShaderResourceView* RutTex32 = nullptr;
	//ID3D11ShaderResourceView* RutTex4 = nullptr;
//	ID3D11ShaderResourceView* RutTex42 = nullptr;
	ID3D11ShaderResourceView* Replay = nullptr;
	ID3D11ShaderResourceView* Replay2 = nullptr;
	
	ID3D11ShaderResourceView* Meter = nullptr;
	ID3D11ShaderResourceView* Meter1 = nullptr;
	ID3D11ShaderResourceView* Meter2 = nullptr;
	ID3D11ShaderResourceView* Meter3 = nullptr;
	ID3D11ShaderResourceView* Meter4 = nullptr;
	ID3D11ShaderResourceView* Meter5 = nullptr;
	ID3D11ShaderResourceView* Meter6 = nullptr;
	ID3D11ShaderResourceView* Meter7 = nullptr;
	ID3D11ShaderResourceView* Meter8 = nullptr;
	ID3D11ShaderResourceView* Meter9 = nullptr;
	ID3D11ShaderResourceView* Meter10 = nullptr;
	ID3D11ShaderResourceView* Meter11 = nullptr;

	ID3D11ShaderResourceView* HSbox = nullptr;
	ID3D11ShaderResourceView* GO_over = nullptr;
	ID3D11ShaderResourceView* blackscreen = nullptr;
	ID3D11ShaderResourceView* deathscreen = nullptr;
	

};