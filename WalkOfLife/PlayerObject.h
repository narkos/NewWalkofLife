#pragma once

#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H
#endif

#include "CollisionObject.h"
#include "CollectableObject.h"
#include "Platform.h"

class PlayerObject : public CollisionObject{
protected:
	float speed;
	float jumpHeight;
	int division;

	float rayLength; //storar hur lång ray hiten blev
	float rayRangeUp = 0.2f;
	float rayRangeDown = 1;
	float rayRangeSides = 0.2f;
	XMVECTOR originLow, originHigh, originMiddle;
	float lowValue = 0, middleValue = 0.2f, highValue = 1;
	XMVECTOR up, down, right, left;
	BoundingBox footBox, originalFootBox;

	struct Vec
	{
		float	x, y, z;

		Vec() {};
		Vec(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}

		float Dot(Vec& v2)
		{
			return (x * v2.x) + (y * v2.y) + (z * v2.z);
		}

		float Length()
		{
			return sqrt(x*x + y*y + z*z);
		}

		void Normalize()
		{
			float l = Length();
			x /= l;
			y /= l;
			z /= l;
		}

		Vec Sub(Vec& v){

			return Vec(x - v.x, y - v.y, z - v.z);
		}

		Vec Add(Vec& v){
			return Vec(x + v.x, y + v.y, z + v.z);
		}


		Vec VectorMultFloat(float number){

			return Vec((x * number), (y * number), (z * number));
		}


	};
	float Det(Vec v, Vec u, Vec w){
		return (((v.x * u.y * w.z) + (v.y * u.z * w.x) + (v.z * u.x * w.y)) - ((v.x * u.z * w.y) + (v.y * u.x * w.z) + (v.z * u.y * w.x)));
	}
public:
	int getDivision();
	void setDivision(int);
	void UpdateDivision(int);

	float momentum;
	bool jumping;
	int bajs = 0;

	XMFLOAT3 startPlatformPos;
	XMFLOAT3 currPlatformPos;

	PlayerObject(ID3D11Buffer *b, XMFLOAT3 pos, bool isActive, bool isStatic, BoundingBox bbox) : CollisionObject(b, pos, isActive, isStatic, bbox){
		this->speed = 0.1;
		this->division = 0;

		originLow = XMVectorSet(pos.x, pos.y + lowValue, pos.z, 1); //0 för att det är en vektor
		originMiddle = XMVectorSet(pos.x, pos.y + middleValue, pos.z, 1);
		originHigh = XMVectorSet(pos.x, pos.y + highValue, pos.z, 1);
		up = XMVectorSet(0, 1, 0, 0);
		down = XMVectorSet(0, -1, 0, 0);
		right = XMVectorSet(1, 0, 0, 0);
		left = XMVectorSet(-1, 0, 0, 0);

		footBox.Center = XMFLOAT3(XMVectorGetX(originLow), XMVectorGetY(originLow), XMVectorGetZ(originLow));
		footBox.Extents = bbox.Extents;
		footBox.Extents.y = 10;
		originalFootBox = footBox; //används vid transformation av bbox

		currPlatformPos = XMFLOAT3(0, 0, 0);
		startPlatformPos = XMFLOAT3(100000, 1000000, 100000);
	}

	PlayerObject(){}
	~PlayerObject(){}

	void Move(bool right);
	void Jump();
	float getSpeed();
	float getJumpHeight();
	void UpdatePosition();

	//void TestIntersect(CollectableObject cObj, int &currCoins, int &currTime){ //testar ifall man träffar ett specifikt CollectableObject och isåfall få dess värde
	//	//ha nån if-sats som kollar vilka object som ligger i närheten så man inte behöver skicka in alla CollectableObjectsd
	//	if (cObj.GetActive() == true){ //testa bara collision ifall detta objektet är aktivt
	//		if (cObj.isCollectable() == true){
	//			if (bbox.Intersects(cObj.GetBBOX()) == true){ //denna bör vi nog använda, fast använd rays istället
	//				currCoins += cObj.GetCoinValue();
	//				currTime += cObj.GetTimeValue();
	//			}
	//		}
	//	}
	//}

	void SetRayOrigins(float low, float middle, float high){
		lowValue = low, middleValue = middle, highValue = high;
	}


	bool TestIntersect(Platform pObj){
		if (pObj.GetActive() == true){
			if (bbox.Contains(pObj.GetBBOX()) == 1)
				return true;
			else if (bbox.Intersects(pObj.GetBBOX()) == true) //denna bör vi nog använda, fast använd rays istället
				return true;

			else return false;
		}
		else return false;
	}

	bool TestUp(Platform pObj){
		if (pObj.GetActive() == true){
			if (pObj.GetBBOX().Intersects(originHigh, up, rayLength) == true){
				if (rayLength < rayRangeUp)
					return true;
				else return false;
			}
			else return false;
		}
		else return false;
	}

	bool TestDown(Platform& pObj){
		if (pObj.GetActive() == true){

			if (pObj.GetBBOX().Intersects(originLow, down, rayLength) == true){
				if (rayLength < rayRangeDown){
					if (SameXMFLOAT3(startPlatformPos, pObj.playerStartIntervalPosition) == false){ 
						startPlatformPos = pObj.GetCurrIntervalPos();
						pObj.playerStartIntervalPosition = startPlatformPos;
					}
					currPlatformPos = SubXMFLOAT3(pObj.GetCurrIntervalPos(), startPlatformPos);
					return true;
				}
				else return false;
			}
			//if (footBox.Intersects(pObj.GetBBOX()) == true){
			//	if (SameXMFLOAT3(startPlatformPos, pObj.playerStartIntervalPosition) == false){
			//		startPlatformPos = pObj.GetCurrIntervalPos();
			//		pObj.playerStartIntervalPosition = startPlatformPos;
			//	}
			//	currPlatformPos = SubXMFLOAT3(pObj.GetCurrIntervalPos(), startPlatformPos);
			//	return true;
			//}
			else return false;
		}
		else return false;
	}

	bool TestRight(Platform pObj){
		if (pObj.GetActive() == true){
			if (pObj.GetBBOX().Intersects(originMiddle, right, rayLength) == true){
				if (rayLength < rayRangeSides){
					if (pObj.GetBBOX().Intersects(originHigh, right, rayLength) == true){
						if (rayLength < rayRangeSides){
							if (pObj.GetBBOX().Intersects(originLow, right, rayLength) == true){
								if (rayLength < rayRangeSides){
									return true;
								}
								else return false;
							}
							else return false;
						}
						else return false;
					}
					else return false;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}

	bool TestLeft(Platform pObj){
		if (pObj.GetActive() == true){
			if (pObj.GetBBOX().Intersects(originMiddle, left, rayLength) == true){
				if (rayLength < rayRangeSides){
					if (pObj.GetBBOX().Intersects(originHigh, left, rayLength) == true){
						if (rayLength < rayRangeSides){
							if (pObj.GetBBOX().Intersects(originLow, left, rayLength) == true){
								if (rayLength < rayRangeSides){
									return true;
								}
								else return false;
							}
							else return false;
						}
						else return false;
					}
					else return false;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}

	void Translate(float x, float y, float z){
		pos = XMMatrixTranslation(this->xPos + x, this->yPos + y, 0);

		originLow = XMVectorSet(xPos + x, yPos + y + lowValue, 0, 1);
		originMiddle = XMVectorSet(xPos + x, yPos + y + middleValue, 0, 1);
		originHigh = XMVectorSet(xPos + x, yPos + y + highValue, 0, 1);


		BoundingBox tempB;
		tempB = originalBox;
		tempB.Transform(tempB, 1.0f, XMVectorSet(0, 0, 0, 1), XMVectorSet(currIntervalPosition.x + xPos, currIntervalPosition.y + yPos, 0, 1));
		bbox = tempB;

		tempB = originalFootBox;
		tempB.Transform(tempB, 1.0f, XMVectorSet(0, 0, 0, 1), XMVectorSet(currIntervalPosition.x + xPos, currIntervalPosition.y + yPos, 0, 1));
		footBox = tempB;



		/*left = XMVectorSet((XMVectorGetX(left) + xPos), (XMVectorGetY(left) + yPos), 0, 0);
		right = XMVectorSet((XMVectorGetX(right) + xPos), (XMVectorGetY(right) + yPos), 0, 0);
		up = XMVectorSet((XMVectorGetX(up) + xPos), (XMVectorGetY(up) + yPos), 0, 0);
		down = XMVectorSet((XMVectorGetX(down) + xPos), (XMVectorGetY(down) + yPos), 0, 0);*/
	}

	////dessa två är bara här för att virtuell mojset ska stämma överens
	bool isCollectable(){ return false; }
	bool isDeadly(){ return false; }
};