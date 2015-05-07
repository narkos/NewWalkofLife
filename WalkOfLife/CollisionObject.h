#pragma once

#include "GameObject.h"

class CollisionObject : public GameObject{
protected:
	//dessa två modifieras automatiskt av subklasserna: CollectableObject och Platform
	bool isCollectable; //används när spelaren kollar collision mot detta objekt, ifall denna är sann så är collidern en CollectableObject
	bool isDeadly; //dör man om man colliderar med detta objekt?

	BoundingBox bbox;

public:
	CollisionObject(ID3D11Buffer *b, XMFLOAT3 pos, bool isActive, bool isStatic, BoundingBox bbox) : GameObject(b, pos, isActive, isStatic){ //skicka sedan in BBOX:en i konstruktorn!
		this->bbox = bbox;
	}

	CollisionObject(){}
	~CollisionObject(){

	}

	void UpdateBBOX(){ //ifall man flyttar runt detta object så måste oxå BBOXen updateras!
		//bbox.Transform(bbox, world);
	}

	void CalculateWorld(){ //denna ska kallas innan man skickar in den i GPUn
		if (isActive == true && isStatic == false){ //kanske bättre att check isStatic i main koden istället
			world = scale * rot * pos; //stämmer detta?
			UpdateBBOX();
		}
	}
	//sedan bör vi kanske ha en funktion GetIntersects() som hämtar alla object som denne collidat med


	//	virtual bool isCollectable(){ return false; } //så behöver man bara vara specifik med att det är ett CollisionObject när man testar
	//	virtual bool isDeadly(){ return false; }

	BoundingBox GetBBOX(){ return this->bbox; }


	ID3D11Buffer *boundingBoxVertexBuffer;
	void CreateBBOXVertexBuffer(ID3D11Device* gDevice){
		BoundingBox box = bbox;
		std::vector<XMFLOAT3> boxVertPoints;

		boxVertPoints.push_back(XMFLOAT3(box.Center.x - box.Extents.x, box.Center.y - box.Extents.y, box.Center.z - box.Extents.z)); //0,0,0
		boxVertPoints.push_back(XMFLOAT3(box.Center.x + box.Extents.x, box.Center.y - box.Extents.y, box.Center.z - box.Extents.z)); //1,0,0
		boxVertPoints.push_back(XMFLOAT3(box.Center.x + box.Extents.x, box.Center.y + box.Extents.y, box.Center.z - box.Extents.z)); //1,1,0
		boxVertPoints.push_back(XMFLOAT3(box.Center.x - box.Extents.x, box.Center.y + box.Extents.y, box.Center.z - box.Extents.z)); //0,1,0
		boxVertPoints.push_back(XMFLOAT3(box.Center.x - box.Extents.x, box.Center.y - box.Extents.y, box.Center.z - box.Extents.z)); //0,0,0

		boxVertPoints.push_back(XMFLOAT3(box.Center.x - box.Extents.x, box.Center.y - box.Extents.y, box.Center.z + box.Extents.z)); //0,0,1
		boxVertPoints.push_back(XMFLOAT3(box.Center.x + box.Extents.x, box.Center.y - box.Extents.y, box.Center.z + box.Extents.z)); //1,0,1
		boxVertPoints.push_back(XMFLOAT3(box.Center.x + box.Extents.x, box.Center.y + box.Extents.y, box.Center.z + box.Extents.z)); //1,1,1
		boxVertPoints.push_back(XMFLOAT3(box.Center.x - box.Extents.x, box.Center.y + box.Extents.y, box.Center.z + box.Extents.z)); //0,1,1
		boxVertPoints.push_back(XMFLOAT3(box.Center.x - box.Extents.x, box.Center.y - box.Extents.y, box.Center.z + box.Extents.z)); //0,0,1

		boxVertPoints.push_back(XMFLOAT3(box.Center.x + box.Extents.x, box.Center.y - box.Extents.y, box.Center.z + box.Extents.z)); //1,0,1
		boxVertPoints.push_back(XMFLOAT3(box.Center.x + box.Extents.x, box.Center.y - box.Extents.y, box.Center.z - box.Extents.z)); //1,0,0
		boxVertPoints.push_back(XMFLOAT3(box.Center.x + box.Extents.x, box.Center.y + box.Extents.y, box.Center.z - box.Extents.z)); //1,1,0
		boxVertPoints.push_back(XMFLOAT3(box.Center.x + box.Extents.x, box.Center.y + box.Extents.y, box.Center.z + box.Extents.z)); //1,1,1
		boxVertPoints.push_back(XMFLOAT3(box.Center.x - box.Extents.x, box.Center.y + box.Extents.y, box.Center.z + box.Extents.z)); //0,1,1
		boxVertPoints.push_back(XMFLOAT3(box.Center.x - box.Extents.x, box.Center.y + box.Extents.y, box.Center.z - box.Extents.z)); //0,1,0


		D3D11_BUFFER_DESC bDesc;
		ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));
		bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bDesc.Usage = D3D11_USAGE_DEFAULT;
		bDesc.ByteWidth = sizeof(XMFLOAT3)* (boxVertPoints.size());

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = boxVertPoints.data();//<--------
		HRESULT VertexBufferChecker = gDevice->CreateBuffer(&bDesc, &data, &boundingBoxVertexBuffer);
	}
};