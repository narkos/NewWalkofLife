#pragma once

#ifndef BILLDBOARDTEXTUREEFFECT_H
#define BILLDBOARDTEXTUREEFFECT_H
#endif

#include "GameTimer.h"

#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>
#include <DirectXMathVector.inl>
#include <d3d11.h>
#include <string>
#include <sstream>

#include "Extra DirectXLibs\Inc\DDSTextureLoader.h"
#include "Extra DirectXLibs\Inc\WICTextureLoader.h"

//create string compare
#include <AtlBase.h>
#include <atlconv.h>

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
	int currIndex;

	float timer = 0.0f;
	float showTime = 1.0f;
	float totalTime = 0.0f;

	//std::string *texturePaths;
	vector<std::string> texturePaths;
	std::string billboardBaseName;
	std::string fileType;


	struct Vertex{
		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
	};
public:
	bool playing;

	BillboardTextureEffect(ID3D11Device *gDev, int nrImages, float spriteWidth, float spriteHeight, float showTime, std::string billboardBaseName, std::string fileType){
		gDevice = gDev;
		this->nrImages = nrImages;
		this->showTime = showTime;
		this->billboardBaseName = billboardBaseName;
		this->fileType = fileType;
		playing = false;

		CreateResourceViews();
		CreateVertexBuffer(spriteWidth, spriteHeight);
	}
	BillboardTextureEffect(){}
	~BillboardTextureEffect(){
		//delete sResourceViews;
	}

	void CreateResourceViews(){
		sResourceViews = new ID3D11ShaderResourceView*[nrImages];
		//texturePaths = new std::string[nrImages];
		
		for (int i = 0; i < nrImages; i++){
			std::string tempString;
			std::ostringstream oss;
			oss << i;
			tempString = billboardBaseName + oss.str() + fileType;
			texturePaths.push_back(tempString);
		}

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

	void CreateVertexBuffer(float width, float height){
		vector<Vertex> vertecies;

		Vertex temp;
		temp.pos = XMFLOAT3(0, 0, 0);
		temp.texCoord = XMFLOAT2(0, 0);
		vertecies.push_back(temp);
		temp.pos = XMFLOAT3(0, height, 0);
		temp.texCoord = XMFLOAT2(0, 1);
		vertecies.push_back(temp);
		temp.pos = XMFLOAT3(width, height, 0);
		temp.texCoord = XMFLOAT2(1, 1);
		vertecies.push_back(temp);
		temp.pos = XMFLOAT3(width, 0, 0);
		temp.texCoord = XMFLOAT2(1, 0);
		vertecies.push_back(temp);


		D3D11_BUFFER_DESC bDesc;
		ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));
		bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bDesc.Usage = D3D11_USAGE_DEFAULT;
		bDesc.ByteWidth = sizeof(Vertex)*(vertecies.size());

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = vertecies.data();//<--------
		HRESULT VertexBufferChecker = gDevice->CreateBuffer(&bDesc, &data, &billboardVertexBuffer);
	}

	
	void Play(){
		currIndex = 0;
		playing = true;
		
	}

	void PlayBillboard(float time){ //position oxå?
		if (currIndex >= nrImages)
			currIndex = 0;
			//playing = false;
		if (playing == true){
			if (time > timer){
				//loopa igenom resourceviewsen med tiden
				currResourceView = sResourceViews[currIndex];
				timer = time + showTime;
			}
		}
		
	}

	ID3D11ShaderResourceView** GetCurrRSV(){
		return &currResourceView;
	}

	ID3D11Buffer** GetVertexBuffer(){
		return &billboardVertexBuffer;
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