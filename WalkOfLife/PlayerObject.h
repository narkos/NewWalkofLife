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
	float rayRangeDown = 0.4f;
	float rayRangeSides = 0.5f;
	XMVECTOR originLow, originLowRight, originLowLeft;
	XMVECTOR originHigh, originMiddle, originMiddle2, originMiddle3, originMiddle4;
	vector<XMVECTOR> originSides;
	vector<float> originsYValues;

	float lowValue = -2.1f, middleValue = -1.8f, highValue = 0.8f, extraXValue = 0.6f;
	
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
	void Dash();

	float dashTimer;
	bool dashDisabling;
	float jumpMomentumX;
	bool jumpMomentumState;
	float jumpTimer = 0;
	float momentum;
	bool jumping;
	int bajs = 0;
	float dashAvailable = true;

	XMFLOAT3 startPlatformPos;
	XMFLOAT3 currPlatformPos;
	XMFLOAT3 lastFrameCurrPlatformPos;

	PlayerObject(ID3D11Buffer *b, XMFLOAT3 pos, bool isActive, bool isStatic, BoundingBox bbox, float xInter, float yInter, float xSpeed, float ySpeed) : CollisionObject(b, pos, isActive, isStatic, bbox, xInter, yInter, xSpeed, ySpeed){
		this->speed = xSpeed;
		this->division = 0;
		this->jumpHeight = ySpeed;
		this->jumpMomentumState = false;
		this->jumpMomentumX = 0;
		this->dashDisabling = true;


		originLow = XMVectorSet(pos.x, pos.y + lowValue, pos.z, 1); 
		originLowRight = XMVectorSet(pos.x + extraXValue, pos.y + lowValue, pos.z, 1); //groundchecks
		originLowLeft = XMVectorSet(pos.x - extraXValue, pos.y + lowValue, pos.z, 1);
		/*originMiddle = XMVectorSet(pos.x, pos.y + middleValue, pos.z, 1);
		originMiddle2 = XMVectorSet(pos.x, pos.y + middleValue2, pos.z, 1);
		originMiddle3 = XMVectorSet(pos.x, pos.y + middleValue3, pos.z, 1);
		originMiddle4 = XMVectorSet(pos.x, pos.y + middleValue4, pos.z, 1);*/
		for (int i = 0; i < 10; i++){
			AddOriginSides(-2 + i/2);
		}

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
		lastFrameCurrPlatformPos = XMFLOAT3(0, 0, 0);
		startPlatformPos = XMFLOAT3(100000, 1000000, 100000);
	}

	PlayerObject(){}
	~PlayerObject(){}

	void SetDivision(int div){
		this->division = div;
	}
	void setJumpMomentum(bool);
	void Move(bool right);
	void Jump();
	float getSpeed();
	float getJumpHeight();
	void UpdatePosition(bool canGoRight, bool canGoLeft);

	void SetRayOrigins(float low, float middle, float high){
		lowValue = low, middleValue = middle, highValue = high;
	}


	bool TestIntersect(Platform pObj){
		if (pObj.GetActive() == true){
			if (bbox.Intersects(pObj.GetBBOX()) == true) //denna bör vi nog använda, fast använd rays istället
				return true;
			else if (bbox.Contains(pObj.GetBBOX()) == true)
				return true;
			else return false;
		}
		else return false;
	}

	bool TestIntersect(CollisionObject pObj){
		if (pObj.GetActive() == true){
			if (bbox.Intersects(pObj.GetBBOX()) == true) //denna bör vi nog använda, fast använd rays istället
				return true;
			else if (bbox.Contains(pObj.GetBBOX()) == true)
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

	bool TestDownMovingPlatform(Platform& pObj, bool isGrounded){
		//if (SameXMFLOAT3(startPlatformPos, pObj.playerStartIntervalPosition) == false){ //kalla detta en gång vid impact istället, spelar ingen roll om det är samma object
		if (isGrounded == false){ //denna blir kallad flera gånger även om den inte ska det!
			startPlatformPos = pObj.GetCurrIntervalPos();
			pObj.playerStartIntervalPosition = startPlatformPos;
		}
		currPlatformPos = SubXMFLOAT3(pObj.GetCurrIntervalPos(), startPlatformPos);
		return true;
				
	}

	
	bool TestDown(Platform& pObj, bool isGrounded){
		if (pObj.GetActive() == true){

			if (pObj.GetBBOX().Intersects(originLow, down, rayLength) == true){
				if (rayLength < rayRangeDown){
					if (pObj.GetStatic() == false){
						TestDownMovingPlatform(pObj, isGrounded);
					}
					else{
						currPlatformPos = XMFLOAT3(0, 0, 0);
						lastFrameCurrPlatformPos = XMFLOAT3(0, 0, 0);
						//startPlatformPos = XMFLOAT3(0, 0, 0);
					}
					return true;
					
				}			
			}
			if (pObj.GetBBOX().Intersects(originLowRight, down, rayLength) == true){
				if (rayLength < rayRangeDown){
					if (pObj.GetStatic() == false){
						TestDownMovingPlatform(pObj, isGrounded);
					}
					else{
						currPlatformPos = XMFLOAT3(0, 0, 0);
						lastFrameCurrPlatformPos = XMFLOAT3(0, 0, 0);
						//startPlatformPos = XMFLOAT3(0, 0, 0);
					}
					return true;

				}				

			}
			if (pObj.GetBBOX().Intersects(originLowLeft, down, rayLength) == true){
				if (rayLength < rayRangeDown){
					if (pObj.GetStatic() == false){
						TestDownMovingPlatform(pObj, isGrounded);
					}
					else{
						currPlatformPos = XMFLOAT3(0, 0, 0);
						lastFrameCurrPlatformPos = XMFLOAT3(0, 0, 0);
						//startPlatformPos = XMFLOAT3(0, 0, 0);
					}
					return true;

				}
			}
			else{
				currPlatformPos = XMFLOAT3(0, 0, 0);
				lastFrameCurrPlatformPos = XMFLOAT3(0, 0, 0);
				return false;
			}
		}
		else return false;
	}

	bool TestRight(Platform pObj){
		if (pObj.GetActive() == true){
			for (int i = 0; i < originSides.size() - 1; i++){
				if (pObj.GetBBOX().Intersects(originSides[i], right, rayLength) == true){
					if (rayLength < rayRangeSides){
						return true;
					}
				}
			}

			if (pObj.GetBBOX().Intersects(originHigh, right, rayLength) == true){
				if (rayLength < rayRangeSides){
					return true;
				}
			}

			if (pObj.GetBBOX().Intersects(originLow, right, rayLength) == true){
				if (rayLength < rayRangeSides){
					return true;
				}
			}

			else return false;
		}
		return false;
	
	}

	bool TestLeft(Platform pObj){
		if (pObj.GetActive() == true){
			for (int i = 0; i < originSides.size() - 1; i++){
				if (pObj.GetBBOX().Intersects(originSides[i], left, rayLength) == true){
					if (rayLength < rayRangeSides){
						return true;
					}
				}
			}

			if (pObj.GetBBOX().Intersects(originHigh, left, rayLength) == true){
				if (rayLength < rayRangeSides){
					return true;
				}
			}

			if (pObj.GetBBOX().Intersects(originLow, left, rayLength) == true){
				if (rayLength < rayRangeSides){
					return true;
				}
			}
			else return false;


		}
		return false;
	}

	void AddOriginSides(float yValue){
		XMVECTOR temp = XMVectorSet(xPos, yPos + yValue, 0, 1);
		originsYValues.push_back(yValue);
		originSides.push_back(temp);
	}

	void Translate(float x, float y, float z){
		pos = XMMatrixTranslation(x, y, 0);

		originLow = XMVectorSet(x, y + lowValue, 0, 1);
		originLowRight = XMVectorSet(x + extraXValue, y + lowValue, 0, 1); //markkontroll
		originLowLeft = XMVectorSet(x - extraXValue, y + lowValue, 0, 1);

		originHigh = XMVectorSet(x, y + highValue, 0, 1);

		for (int i = 0; i < originSides.size() - 1; i++){
			originSides[i] = XMVectorSet(x, y + originsYValues[i], 0, 1);
		}


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