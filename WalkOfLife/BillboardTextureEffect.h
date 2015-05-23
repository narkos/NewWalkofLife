#pragma once

#ifndef BILLDBOARDTEXTUREEFFECT_H
#define BILLDBOARDTEXTUREEFFECT_H
#endif

#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>
#include <DirectXMathVector.inl>
#include <d3d11.h>
#include <string>

#include "Extra DirectXLibs\Inc\DDSTextureLoader.h"
#include "Extra DirectXLibs\Inc\WICTextureLoader.h"

using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")


class BillboardTextureEffect{
private:
	ID3D11Device *gDevice;
	ID3D11Buffer* billboardVertexBuffer;

	int nrImages;
	ID3D11ShaderResourceView **sResourceViews;
	ID3D11ShaderResourceView *currResourceView;

	float timer = 0.0f;
	float showTime = 1.0f;

	std::string *texturePaths;
public:
	BillboardTextureEffect(ID3D11Device *gDev, int nrImages, float showTime){
		gDevice = gDev;
		this->nrImages = nrImages;
		this->showTime = showTime;
		CreateResourceViews();
	}
	~BillboardTextureEffect(){
		//delete sResourceViews;
	}

	void CreateResourceViews(){
		sResourceViews = new ID3D11ShaderResourceView*[nrImages];
		texturePaths = new std::string[nrImages];

		std::wstring path = L"Textures/";

		for (int i = 0; i < nrImages; i++){

			std::wstring completePath = path + string2wString(texturePaths[i]);;
			const wchar_t* thePath = completePath.c_str();
			HRESULT texCheck = CreateDDSTextureFromFile(gDevice, thePath, nullptr, &sResourceViews[i]);
			if (texCheck == E_FAIL){
				CreateWICTextureFromFile(gDevice, thePath, nullptr, &sResourceViews[i]);
			}
		}
	}

	void PlayBillboard(float time){
		if (time > timer){
			//loopa igenom resourceviewsen med tiden
			timer = time + showTime;
		}
		
	}


	std::wstring string2wString(const std::string& s){
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
};