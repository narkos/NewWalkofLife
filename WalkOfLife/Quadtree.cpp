#include "Quadtree.h"
#include "GameObject.h"

using namespace std;

Quadtree::Quadtree(float x, float z, float widthx, float heighty, int level, int maxLevel)
{
	this->heighty = heighty;
	this->widthx = widthx;
	this->x = x;
	this->z = z;
	this->level = level;
	this->maxLevel = maxLevel;

	this->TR = DirectX::XMFLOAT2(x + widthx, z + heighty);  //Calculating corners of Quadtree
	this->BL = DirectX::XMFLOAT2(x - widthx, z - heighty);
	this->BR = DirectX::XMFLOAT2(x + widthx, z - heighty);
	this->TL = DirectX::XMFLOAT2(x - widthx, z + heighty);
	this->center = DirectX::XMFLOAT2(x, z);

	if (level == maxLevel)
		return;

	NW = new Quadtree(x - widthx/2, z + heighty/2, widthx / 2.0f, heighty / 2.0f, level + 1, maxLevel);
	NE = new Quadtree(x + widthx/2, z + heighty/2, widthx / 2.0f, heighty / 2.0f, level + 1, maxLevel);
	SW = new Quadtree(x - widthx/2, z - heighty/2, widthx / 2.0f, heighty / 2.0f, level + 1, maxLevel);
	SE = new Quadtree(x + widthx /2, z - heighty/2, widthx / 2.0f, heighty / 2.0f, level + 1, maxLevel);

	
}

Quadtree::~Quadtree()
{
	if (level == maxLevel)
		return;

	delete NW;
	delete NE;
	delete SW;
	delete SE;
}

void Quadtree::AddObject(Platform *object) {
	if (level == maxLevel) {
		objects.push_back(object);
		return;
	}

	if (NW->isInside(object->GetBBOX().Center.x, object->GetBBOX().Center.y)) {
		NW->AddObject(object);
	}
	if (NE->isInside(object->GetBBOX().Center.x, object->GetBBOX().Center.y)) {
		NE->AddObject(object);
	}
	if (SW->isInside(object->GetBBOX().Center.x, object->GetBBOX().Center.y)) {
		SW->AddObject(object);
	}
	if (SE->isInside(object->GetBBOX().Center.x, object->GetBBOX().Center.y)) {
		SE->AddObject(object);
	}
}

void Quadtree::objectsInSpace(float x, float y, vector<Platform*> *objects)
{
	if (level == maxLevel)
	{
		for (int i = 0; i < this->objects.size(); i++)
		{
			objects->push_back(this->objects[i]);
		}
		
	}
	else
	{

		if (SW->isInside(x, y))
		{
			SW->objectsInSpace(x, y, objects);
		}
		if (SE->isInside(x, y))
		{
			SE->objectsInSpace(x, y, objects);
		}

		if (NE->isInside(x, y))
		{
			NE->objectsInSpace(x, y, objects);
		}

		if (NW->isInside(x, y))
		{
			NW->objectsInSpace(x, y, objects);
		}
	}
	
}

//bool Quadtree::frustumAABB(Frustum *theFrustum)
//{
//	DirectX::XMVECTOR TRv, BLv, TLv, BRv, pNear, pFar, pLeft, pRight, center;
//	DirectX::XMLoadFloat2(&theFrustum->nearPlane.normal);
//	TRv = DirectX::XMLoadFloat2(&this->TR);
//	BLv = DirectX::XMLoadFloat2(&this->BL);
//	TLv = DirectX::XMLoadFloat2(&this->TL);
//	BRv = DirectX::XMLoadFloat2(&this->BR);
//	center = DirectX::XMLoadFloat2(&this->center);
//
//	pNear = DirectX::XMLoadFloat2(&theFrustum->nearPlane.normal);
//	pFar = DirectX::XMLoadFloat2(&theFrustum->farPlane.normal);
//	pLeft = DirectX::XMLoadFloat2(&theFrustum->leftPlane.normal);
//	pRight = DirectX::XMLoadFloat2(&theFrustum->rightPlane.normal);
//	int out = 0;
//	int in = 0;
//	
//		
//		// is the corner outside or inside
//		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pNear, TRv)) + theFrustum->nearPlane.w < 0)
//			out++;
//		else
//			in++;
//
//		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pNear, BLv)) + theFrustum->nearPlane.w < 0)
//			out++;
//		else
//			in++;
//
//		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pNear, BRv)) + theFrustum->nearPlane.w < 0)
//			out++;
//		else
//			in++;
//
//		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pNear, TLv)) + theFrustum->nearPlane.w < 0)
//			out++;
//		else
//			in++;
//
//
//		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pNear, center)) + theFrustum->nearPlane.w < 0)
//			out++;
//		else
//			in++;
//
//	
//	//if all corners are out
//	if (in == 0)
//		return false;
//	
//
//	in = 0;
//	float d = DirectX::XMVectorGetX(DirectX::XMVector3Dot(pFar, TRv));
//	DirectX::XMVECTOR asd = DirectX::XMVector3Dot(pFar, BLv);
//		// is the corner outside or inside
//		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pFar, TRv)) + theFrustum->farPlane.w < 0)
//			out++;
//		else
//			in++;
//
//		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pFar, BLv)) + theFrustum->farPlane.w < 0)
//			out++;
//		else
//			in++;
//
//		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pFar, TLv)) + theFrustum->farPlane.w < 0)
//			out++;
//		else
//			in++;
//
//		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pFar, BRv)) + theFrustum->farPlane.w < 0)
//			out++;
//		else
//			in++;
//
//		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pFar, center)) + theFrustum->farPlane.w < 0)
//			out++;
//		else
//			in++;
//	
//	//if all corners are out
//	if (in == 0)
//		return false;
//
//	in = 0;
//	d = DirectX::XMVectorGetX(DirectX::XMVector3Dot(pLeft, TRv));
//	if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pLeft, TRv)) + theFrustum->leftPlane.w < 0)
//		out++;
//	else
//		in++;
//
//	if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pLeft, BLv)) + theFrustum->leftPlane.w < 0)
//		out++;
//	else
//		in++;
//
//	if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pLeft, BRv)) + theFrustum->leftPlane.w < 0)
//		out++;
//	else
//		in++;
//
//	if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pLeft, TLv)) + theFrustum->leftPlane.w < 0)
//		out++;
//	else
//		in++;
//
//	if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pLeft, center)) + theFrustum->leftPlane.w < 0)
//		out++;
//	else
//		in++;
//
//	//if all corners are out
//	if (in == 0)
//		return false;
//
//	in = 0;
//	if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pRight, TRv)) + theFrustum->rightPlane.w < 0)
//		out++;
//	else
//		in++;
//
//	if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pRight, BLv)) + theFrustum->rightPlane.w < 0)
//		out++;
//	else
//		in++;
//
//	if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pRight, BRv)) + theFrustum->rightPlane.w < 0)
//		out++;
//	else
//		in++;
//
//	if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pRight, TLv)) + theFrustum->rightPlane.w < 0)
//		out++;
//	else
//		in++;
//
//	if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(pRight, center)) + theFrustum->rightPlane.w < 0)
//		out++;
//	else
//		in++;
//
//	//if all corners are out
//	if (in == 0)
//		return false;
//
//	return true;
//
//}

//vector<Object*> Quadtree::GetObjectsAt(float _x, float _y) {
//	if (level == maxLevel)
//		return objects;
//
//	vector<Object*> returnObjects, childReturnObjects;
//	if (!objects.empty()) {
//		returnObjects = objects;
//	}
//	if (_x > this->x + width / 2.0f && _x < this->x + width) {
//		if (_y > y + height / 2.0f && _y < y + height) {
//			childReturnObjects = SE->GetObjectsAt(_x, _y);
//			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
//			return returnObjects;
//		}
//		else if (_y > this->y && _y <= this->y + height / 2.0f) {
//			childReturnObjects = NE->GetObjectsAt(_x, _y);
//			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
//			return returnObjects;
//		}
//	}
//	else if (_x > x && _x <= x + width / 2.0f) {
//		if (_y > y + height / 2.0f && _y < y + height) {
//			childReturnObjects = SW->GetObjectsAt(_x, _y);
//			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
//			return returnObjects;
//		}
//		else if (_y > y && _y <= y + height / 2.0f) {
//			childReturnObjects = NW->GetObjectsAt(_x, _y);
//			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
//			return returnObjects;
//		}
//	}
//
//	return returnObjects;
//}

void Quadtree::Clear() {
	if (level == maxLevel) {
		objects.clear();
		return;
	}
	else {
		NW->Clear();
		NE->Clear();
		SW->Clear();
		SE->Clear();
	}

	if (!objects.empty()) {
		objects.clear();
	}
}

//vector<Object*>

//bool Quadtree::contains(Quadtree *child, Object *object) {
//	//if all corners of the object are to the left or to the right of the quadtree
//	if ((object->TR.x > child->center.x + child->widthx && object->BL.x > child->center.x + child->widthx && object->BR.x > child->center.x + child->widthx && object->TL.x > child->center.x + child->widthx) || (object->TR.x < child->center.x - child->widthx && object->BL.x < child->center.x - child->widthx && object->BR.x < child->center.x - child->widthx && object->TL.x < child->center.x - child->widthx))
//		return false;
//
//	//if all corners of the object are above or below the quadtree
//	if ((object->TR.y > child->center.y + child->heightz && object->BL.y > child->center.y + child->heightz && object->BR.y > child->center.y + child->heightz && object->TL.y > child->center.y + child->heightz) || (object->TR.y < child->center.y - child->heightz && object->BL.y < child->center.y - child->heightz && object->BR.y < child->center.y - child->heightz && object->TL.y < child->center.y - child->heightz))
//		return false;
//
//
//	return true;
//}

bool Quadtree::isInside(float x, float y)
{
	if ((x < this->center.x - this->widthx) || (x > this->center.x + this->widthx) || (y < this->center.y - this->heighty) || (y > this->center.y + this->heighty))
	{
		return false;
	}

	else
		return true;

}