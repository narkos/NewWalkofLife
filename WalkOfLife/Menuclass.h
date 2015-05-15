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
	void scrollUp();
	void scrollDown();
	void CreateTextures(ID3D11Device* gDevice);
	std::unique_ptr<DirectX::SpriteBatch> MenuspriteBatch;
	std::unique_ptr<DirectX::SpriteBatch> MenuspriteBatch2;
	std::unique_ptr<DirectX::SpriteBatch> MenuspriteBatch3;
	
	void ActiveMenu(ID3D11DeviceContext* gDeviceContext, float width, float height, IDXGISwapChain* SwapChain);

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
private:
	int t;
	int currentTab = 1;
	int chosenTab;
	bool pause = false;
	ID3D11ShaderResourceView* RutTex1 = nullptr;
	ID3D11ShaderResourceView* RutTex12 = nullptr;
	ID3D11ShaderResourceView* RutTex2 = nullptr;
	ID3D11ShaderResourceView* RutTex22 = nullptr;
	ID3D11ShaderResourceView* RutTex3 = nullptr;
	ID3D11ShaderResourceView* RutTex32 = nullptr;
	ID3D11ShaderResourceView* RutTex4 = nullptr;
	ID3D11ShaderResourceView* RutTex42 = nullptr;

};