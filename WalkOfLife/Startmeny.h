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




class Startmenu{

public:
	Startmenu();
	void menuInit(ID3D11DeviceContext* gDeviceContext);
	void scrollUp();
	void scrollDown();
	
	void CreateTextures(ID3D11Device* gDevice);
	std::unique_ptr<DirectX::SpriteBatch> StartspriteBatch1;
	std::unique_ptr<DirectX::SpriteBatch> StartspriteBatch2;
	std::unique_ptr<DirectX::SpriteBatch> StartspriteBatch3;
	std::unique_ptr<DirectX::SpriteBatch> StartspriteBatch4;
	std::unique_ptr<DirectX::SpriteBatch> StartspriteBatch5;

	void ActivestartMenu(ID3D11DeviceContext* gDeviceContext, float width, float height, IDXGISwapChain* SwapChain, bool HS);

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
	bool getstartmeny();
	void setstartmeny(bool set);
	bool getPause();
private:
	int t;
	int currentTab = 1;
	int chosenTab;
	bool startmenyactive = true;
	ID3D11ShaderResourceView* blackscreen = nullptr;
	ID3D11ShaderResourceView* Walkoflife = nullptr;
	ID3D11ShaderResourceView* RutTex1 = nullptr;
	ID3D11ShaderResourceView* RutTex12 = nullptr;
	ID3D11ShaderResourceView* RutTex2 = nullptr;
	ID3D11ShaderResourceView* RutTex22 = nullptr;
	ID3D11ShaderResourceView* RutTex3 = nullptr;
	ID3D11ShaderResourceView* RutTex32 = nullptr;
	ID3D11ShaderResourceView* RutTex4 = nullptr;
	ID3D11ShaderResourceView* RutTex42 = nullptr;
	ID3D11ShaderResourceView* HSbox = nullptr;



};