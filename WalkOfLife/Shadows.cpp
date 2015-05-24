#include "Shadows.h"

Shadows::Shadows()
{}

Shadows::Shadows(int winWidth, int winHeight, ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext)
{
	WINDOW_WIDTH = winWidth;
	WINDOW_HEIGHT = winHeight;

	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;
}

void Shadows::createShadowMap()
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));		//Initialize the render target texture description.

	//Setup the texture description.
	texDesc.Width = WINDOW_WIDTH;	//Or shadowMapWidth for square, and 1.0 ratio
	texDesc.Height = WINDOW_HEIGHT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	HRESULT hr1 = gDevice->CreateTexture2D(&texDesc, NULL, &sRenderTargetTexture);	//Create the render target texture.

	//Setup the description of the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0;
	DSVDesc.Flags = 0;
	HRESULT hr2 = gDevice->CreateDepthStencilView(sRenderTargetTexture, &DSVDesc, &sDepthStencilView);		//Create the depth stencil view.

	//Setup the description of the shader resource view.
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;
	HRESULT hr3 = gDevice->CreateShaderResourceView(sRenderTargetTexture, &SRVDesc, &sShaderResourceView);	//Create the shader resource view.
}

//void Shadows::setShadowShaderParameters(XMMATRIX worldMatrix)
//{
//	//D3D11_MAPPED_SUBRESOURCE mappedResource;
//	//unsigned int bufferNr;
//	//cbPerObj.hasShadowMap = 1;	//Don't Cull when mapping the shadow
//
//	//Transpose the matrices (WVP To lights POV) to prepare them for the shader, For Shadow mapping in this case
//	shadowWVP = worldMatrix * lightViewMatrix * lightProjectionMatrix;
//
//	//cbWorld will be set to these:
//	shadowWorld = XMMatrixTranspose(worldMatrix);
//	shadowWVP = XMMatrixTranspose(shadowWVP);
//	lightView = XMMatrixTranspose(lightViewMatrix);
//	lightProjection = XMMatrixTranspose(lightProjectionMatrix);
//
//	//gDeviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
//	//gDeviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
//}

void Shadows::renderSceneToShadowMap(XMMATRIX worldMatrix, XMFLOAT4 lightPosition, float lookAtX)
{
	gDeviceContext->OMSetRenderTargets(0, nullptr, sDepthStencilView);		//Bind the render target view and depth stencil buffer to the output render pipeline.
	gDeviceContext->ClearDepthStencilView(sDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);		//Clear the depth buffer

	///////////PERSPECTIVE SHADOW PROJECTION
	//lightProjectionMatrix = XMMatrixPerspectiveFovLH(3.14*0.45, 1200/900, 0.5f, 20.0f);
	///////////ORTHOGRAPHIC SHADOW PROJECTION
	lightProjectionMatrix = XMMatrixOrthographicLH(60, 30, 0.5f, 100.0f);	//Check

	lightLookAt = XMVectorSet(lookAtX, 0.0f, 3.0f, 1.0f);
	lightViewUp = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);

	//Create the lights view matrix for shadow mapping
	XMVECTOR lightPosToVector = XMVectorSet(lightPosition.x, lightPosition.y, lightPosition.z, 1.0f);
	//XMVECTOR lightPosToVector = XMVectorSet(0.0f, 0.0f, -3.0, 1.0f);	//Custom position
	lightViewMatrix = XMMatrixLookAtLH(lightPosToVector, lightLookAt, lightViewUp);

	//setShadowShaderParameters();	//Set the lights view and projection matrices
	lightWVP = worldMatrix * lightViewMatrix * lightProjectionMatrix;

	//cbWorld will be set to these:
	shadowWorld = worldMatrix;
	lightWVP = lightWVP;
	lightView = lightViewMatrix;	//Gets FUCKED UP when transposed
	lightProjection = lightProjectionMatrix;

	//gDeviceContext->IASetInputLayout(gVertexLayout);	//Set the vertex input layout.
	//gDeviceContext->VSSetShader(gVertexShader, NULL, 0);	//Set the vertex and pixel shaders that will be used to render

	//RENDER THE SHADOW MAP
	//drawScene();
}

void Shadows::setShaderResource()
{
	gDeviceContext->PSSetShaderResources(1, 1, &sShaderResourceView);	//Set (SHADOW MAP) shader texture resource in the pixel shader.
}