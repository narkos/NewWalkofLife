#include "Light.h"

class SpotLight : public Light{
protected:
	XMVECTOR dir;
	float coneRadius;
public:
	SpotLight(XMVECTOR dir, float coneRadius, float intensity, float range, XMFLOAT3 pos, bool isActive, bool isStatic) : Light(intensity, range, pos, isActive, isStatic){
		this->dir = dir;
		this->coneRadius = coneRadius;
	}
	~SpotLight(){}
};