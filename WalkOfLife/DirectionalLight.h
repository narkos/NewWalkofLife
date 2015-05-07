#include "Light.h"

class DirectionalLight : public Light{
protected:
	XMVECTOR dir;
	
public:
	DirectionalLight(XMVECTOR dir, float intensity, float range, XMFLOAT3 pos, bool isActive, bool isStatic) : Light(intensity, range, pos, isActive, isStatic){
		this->dir = dir;
	}
	~DirectionalLight(){}
};