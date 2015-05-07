#pragma once

#ifndef PLATFORM_H
#define PLATFORM_H
#endif

#include "CollisionObject.h"

class Platform : public CollisionObject{

protected:
	void CreateTriangles(){
		for (int i = 0; i < vertices.size(); i++){
			Triangle tempT;
			XMVECTOR tempV;

			//N�gon m�ste kolla p� detta, v�ldigt os�ker, �r vertiserna i denna ordningen som jag skapat dem?? vettigt s�tt att loopa p�?
			tempV = XMVectorSet(vertices[i].x, vertices[i].y, vertices[i].z, 1); //1a f�r att det �r en point
			tempT.x = tempV;

			i++;
			tempV = XMVectorSet(vertices[i].x, vertices[i].y, vertices[i].z, 1);
			tempT.y = tempV;

			i++;
			tempV = XMVectorSet(vertices[i].x, vertices[i].y, vertices[i].z, 1);
			tempT.z = tempV;
		}
	}
	vector<XMFLOAT3> vertices; //vertex array som anv�nds vid collision test, endast positionerna
	vector<Triangle> triangles; //skapa dessa med verices!

public:
	Platform(bool isDeadly, vector<XMFLOAT3> verts, ID3D11Buffer *b, XMFLOAT3 pos, bool isActive, bool isStatic, BoundingBox bbox) : CollisionObject(b, pos, isActive, isStatic, bbox){ //och eventuellt ha med tex isBouncy
		this->isDeadly = isDeadly;
		vertices = verts; //vertiserna m�ste komma in i r�tt ordning f�rh�llande till hur CreateTriangleVector funktionen l�ser dem
		CreateTriangles();
	}
	Platform(){}
	~Platform(){}

	/*bool isCollectable(){ return false; }
	bool isDeadly(){ return isDeadly; }	*/

	vector<Triangle> GetTriangles(){
		return triangles;
	}
};