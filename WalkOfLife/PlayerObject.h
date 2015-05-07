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
	XMVECTOR origin;
	XMVECTOR up, down, right, left;

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
	float momentum;
	bool jumping;
	int bajs = 0;
	PlayerObject(ID3D11Buffer *b, XMFLOAT3 pos, bool isActive, bool isStatic, BoundingBox bbox) : CollisionObject(b, pos, isActive, isStatic, bbox){
		this->speed = 0.05;

		origin = XMVectorSet(pos.x, pos.y, pos.z, 1); //0 f�r att det �r en vektor
		up = XMVectorSet(0, 1, 0, 0);
		down = XMVectorSet(0, -1, 0, 0);
		right = XMVectorSet(1, 0, 0, 0);
		left = XMVectorSet(-1, 0, 0, 0);
	}

	PlayerObject(){}
	~PlayerObject(){}

	void Move(bool right);
	void Jump();
	float getSpeed();
	float getJumpHeight();

	//void TestIntersect(CollectableObject cObj, int &currCoins, int &currTime){ //testar ifall man tr�ffar ett specifikt CollectableObject och is�fall f� dess v�rde
	//	//ha n�n if-sats som kollar vilka object som ligger i n�rheten s� man inte beh�ver skicka in alla CollectableObjectsd
	//	if (cObj.GetActive() == true){ //testa bara collision ifall detta objektet �r aktivt
	//		if (cObj.isCollectable() == true){
	//			if (bbox.Intersects(cObj.GetBBOX()) == true){ //denna b�r vi nog anv�nda, fast anv�nd rays ist�llet
	//				currCoins += cObj.GetCoinValue();
	//				currTime += cObj.GetTimeValue();
	//			}
	//		}
	//	}
	//}

	bool TestIntersect(Platform pObj){
		if (pObj.GetActive() == true){
			if (bbox.Contains(pObj.GetBBOX()) == 1)
				return true;
			else if (bbox.Intersects(pObj.GetBBOX()) == true) //denna b�r vi nog anv�nda, fast anv�nd rays ist�llet
				return true;

			else return false;
		}
		else return false;
	}

	bool TestUp(Platform pObj, float &rayLength){
		if (pObj.GetActive() == true){
			if (pObj.GetBBOX().Intersects(origin, up, rayLength) == true){
				return true;
			}
			else return false;
		}
		else return false;
	}

	bool TestDown(Platform pObj, float &rayLength){
		if (pObj.GetActive() == true){

			if (pObj.GetBBOX().Intersects(origin, down, rayLength) == true){
				bajs++;
				return true;

			}
			else return false;
		}
		else return false;
	}

	bool TestRight(Platform pObj, float &rayLength){
		if (pObj.GetActive() == true){
			if (pObj.GetBBOX().Intersects(origin, right, rayLength) == true){
				return true;
			}
			else return false;
		}
		else return false;
	}

	bool TestLeft(Platform pObj, float &rayLength){
		if (pObj.GetActive() == true){
			if (pObj.GetBBOX().Intersects(origin, left, rayLength) == true){
				return true;
			}
			else return false;
		}
		else return false;
	}

	void Translate(float x, float y, float z){
		pos = XMMatrixTranslation(this->xPos + x, this->yPos + y, 0);
		origin = XMVectorSet(xPos + x, yPos + y, 0, 0);

		/*left = XMVectorSet((XMVectorGetX(left) + xPos), (XMVectorGetY(left) + yPos), 0, 0);
		right = XMVectorSet((XMVectorGetX(right) + xPos), (XMVectorGetY(right) + yPos), 0, 0);
		up = XMVectorSet((XMVectorGetX(up) + xPos), (XMVectorGetY(up) + yPos), 0, 0);
		down = XMVectorSet((XMVectorGetX(down) + xPos), (XMVectorGetY(down) + yPos), 0, 0);*/
	}

	float GetYValueOnMesh(Platform pObj, XMVECTOR ray, float rayLength){ //k�r f�rst funktionen TestDown tex. H�mtar det Y-v�rde som spelaren ska st� p�
		if (TestDown(pObj, rayLength)){
			vector<Triangle> tris = pObj.GetTriangles();
			for each (Triangle t in tris)
			{
				if (DirectX::TriangleTests::Intersects(origin, ray, t.x, t.y, t.z, rayLength) == true){ //st�r spelaren p� denna triangeln?
					//ta reda p� vart p� triangeln spelaren st�r, genom att anv�nda worldpos mojs
					//hur hittar vi positionen mellan tv� punkter i worldspace?
					Vec e1, e2, s;
					Vec or(XMVectorGetX(origin), XMVectorGetY(origin), XMVectorGetZ(origin));
					Vec rayDirection(0, -1, 0);

					Vec tx(XMVectorGetX(t.x), XMVectorGetY(t.x), XMVectorGetZ(t.x));
					Vec ty(XMVectorGetX(t.y), XMVectorGetY(t.y), XMVectorGetZ(t.y));
					Vec tz(XMVectorGetX(t.z), XMVectorGetY(t.z), XMVectorGetZ(t.z));
					e1 = ty.Sub(tx);
					e2 = tz.Sub(tx);
					s = or.Sub(tx);
					//Vec tempVec = (r.d, e1, e2);
					Vec vecTemp = Vec(Det(s, e1, e2), Det(rayDirection.VectorMultFloat(-1.0), s, e2), Det(rayDirection.VectorMultFloat(-1.0), e1, s));

					Vec inversen = vecTemp.VectorMultFloat((1 / Det((rayDirection.VectorMultFloat(-1.0)), e1, e2)));
					float t1 = inversen.x;
					return t1;
					break;
				}
			}
			return 0;
		}
		else return 0; //�ndra denna eventuellt

		//}//RayTestTriangleInMesh



	}

	////dessa tv� �r bara h�r f�r att virtuell mojset ska st�mma �verens
	bool isCollectable(){ return false; }
	bool isDeadly(){ return false; }
};