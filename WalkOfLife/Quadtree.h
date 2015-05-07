#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "GameObject.h"
#include "Platform.h"

class Quadtree;
class Object;

class Quadtree {
public:
	Quadtree(float x, float z, float width, float height, int level, int maxLevel);

	~Quadtree();

	void					AddObject(Platform *object);
	vector<Object*>				GetObjectsAt(float x, float y);
	void					Clear();
	void				 objectsInSpace(float x, float y, vector<Platform*>*);
	//bool					frustumAABB(Frustum*);
	//bool					seeNode;
	DirectX::XMFLOAT2		BL;
	DirectX::XMFLOAT2		TR;
	DirectX::XMFLOAT2		TL;
	DirectX::XMFLOAT2		BR;
	vector<Platform*>				returnObjects;
	bool isInside(float x, float y);
private:
	float					x;
	float					z;
	float					widthx;
	float					heighty;
	int					level;
	int					maxLevel;
	vector<Platform*>				objects;
	
	
	DirectX::XMFLOAT2		center;
	Quadtree *				parent;
	Quadtree *				NW;
	Quadtree *				NE;
	Quadtree *				SW;
	Quadtree *				SE;

	bool					contains(Quadtree *child, GameObject *object);

};

#endif