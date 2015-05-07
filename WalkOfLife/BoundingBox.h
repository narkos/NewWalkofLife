#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>

class BoundingBox{
private:
	XMFLOAT3 center;
	XMFLOAT3 rotationVectors[3];
	float lengths[3];

public:
	BoundingBox(XMFLOAT3 c, XMFLOAT3 vecX, XMFLOAT3 vecY, XMFLOAT3 vecZ, float lngtX, float lngtY, float lngtZ){
		this->center = c;
		this->rotationVectors[0] = vecX;
		this->rotationVectors[1] = vecY;
		this->rotationVectors[2] = vecZ;

		this->lengths[0] = lngtX;
		this->lengths[1] = lngtY;
		this->lengths[2] = lngtZ;
	}

	bool Intersects(BoundingBox b){

	}

	void Transform(XMMATRIX w){

	}
};