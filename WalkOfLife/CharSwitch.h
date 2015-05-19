#pragma once

#ifndef CHARSWITCH_H
#define CHARSWITCH_H
#endif



#include <iostream>
#include <fstream>
#include <d3d11.h>
#include <vector>
#include <string>
#include "Extra DirectXLibs\Inc\DDSTextureLoader.h"
#include "Extra DirectXLibs\Inc\SpriteFont.h"
#include "Extra DirectXLibs\Inc\SpriteBatch.h"
#include "Extra DirectXLibs\Inc\SimpleMath.h"
//using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace std;
class CharSwitch
{

public:
	void switchCharState(float pastchar);
	void setCharState(int state);
	int getCharSate();


private:
	int charstate = 0;

};