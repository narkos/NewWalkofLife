#pragma once
#ifndef SHADOWS_H
#define SHADOWS_H

#include <DirectXMath.h>
#include <d3d11.h>

using namespace DirectX;

class Shadows{

private:
	ID3D11Device* gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;

	int WINDOW_WIDTH;		//The game window width.
	int WINDOW_HEIGHT;		//The game window height.

	XMMATRIX lightViewMatrix;
	XMMATRIX lightProjectionMatrix;
	HRESULT result;

	XMVECTOR lightLookAt;
	XMVECTOR lightViewUp;

	XMMATRIX shadowWorld;
	XMMATRIX lightWVP;
	XMMATRIX lightView;
	XMMATRIX lightProjection;


	ID3D11Texture2D* sRenderTargetTexture = nullptr;
	//ID3D11RenderTargetView* sRenderTargetView = nullptr;		//NEEDED?
	ID3D11ShaderResourceView* sShaderResourceView = nullptr;	//DepthMapTexture
	ID3D11DepthStencilView* sDepthStencilView = nullptr;		//NEW
public:
	Shadows();

	Shadows(int winWidth, int winHeight, ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);

	void createShadowMap();

	//void setShadowShaderParameters(XMMATRIX worldMatrix);

	void renderSceneToShadowMap(XMMATRIX worldMatrix, XMFLOAT4 lightPosition, float lookAtX, XMFLOAT4 direction);

	XMMATRIX getShadowWorld(){
		return shadowWorld;
	}
	XMMATRIX getLightWVP(){
		return lightWVP;
	}
	XMMATRIX getLightView(){
		return lightView;
	}
	XMMATRIX getLightProjection(){
		return lightProjection;
	}

	void Shadows::setShaderResource();
};

#endif