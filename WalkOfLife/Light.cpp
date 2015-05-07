#include "Light.h"



LightClass::LightClass(int type, XMFLOAT3 pos, bool isActive, bool isStatic) : Entity(pos, isActive, isStatic)
{

	this->lightObject.Type = type;

	if (type == l_Directional)
		lightObject.Direction = XMFLOAT4(pos.x, pos.y, pos.z, 1.0f);
	else
		lightObject.Position = XMFLOAT4(pos.x, pos.y, pos.z, 1.0f);
	
	if (isActive)
		lightObject.Active = 1;
	else
		lightObject.Active = 0;
}

void LightClass::ToggleActive()
{
	if (lightObject.Active != 0)
		lightObject.Active = 0;
	else
		lightObject.Active = 1;

	return;
}
void LightClass::CreateLightBuffer(ID3D11Device* dev)
{

	D3D11_BUFFER_DESC lbuffDesc;
	ZeroMemory(&lbuffDesc, sizeof(lbuffDesc));
	lbuffDesc.Usage = D3D11_USAGE_DEFAULT;
	lbuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lbuffDesc.CPUAccessFlags = 0;
	lbuffDesc.MiscFlags = 0;
	lbuffDesc.ByteWidth = sizeof(Light);
	
	HRESULT hr = dev->CreateBuffer(&lbuffDesc, NULL, &lightBuffer);
	
}


void LightClass::Render(ID3D11Device* dev, ID3D11DeviceContext* devcon)
{

	//if (lightType == 1)
	//	devcon->UpdateSubresource(lightBuffer, 0, NULL, &dLightData, 0, 0);
	//else if (lightType == 2)
	//	devcon->UpdateSubresource(lightBuffer, 0, NULL, &dLightData, 0, 0);
	//else if (lightType == 3)
	//	devcon->UpdateSubresource(lightBuffer, 0, NULL, &dLightData, 0, 0);
	//else
	//	return;

	//if (lightType != 0)
	//	devcon->PSSetConstantBuffers(0, 1, &lightBuffer);

}


LightClass::~LightClass()
{
	//lightBuffer->Release();
}

//void Light::UpdateBuffer(ID3D11Device* dev)
//{ 
//	if (lightType == 1)
//	{
//
//	}
// }