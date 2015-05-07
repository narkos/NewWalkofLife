#include "Light.h"

class PointLight : public Light{
protected:
	float radius;
public:
	PointLight(float radius, float intensity, float range, XMFLOAT3 pos, bool isActive, bool isStatic) : Light(intensity, range, pos, isActive, isStatic){
		this->radius = radius;
	}
	~PointLight(){}
};