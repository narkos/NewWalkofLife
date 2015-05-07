#pragma once

#include "GameObject.h"

class CollisionObject : public GameObject{
protected:
	//dessa tv� modifieras automatiskt av subklasserna: CollectableObject och Platform
	bool isCollectable; //anv�nds n�r spelaren kollar collision mot detta objekt, ifall denna �r sann s� �r collidern en CollectableObject
	bool isDeadly; //d�r man om man colliderar med detta objekt?

	BoundingBox bbox;

public:
	CollisionObject(ID3D11Buffer *b, XMFLOAT3 pos, bool isActive, bool isStatic, BoundingBox bbox) : GameObject(b, pos, isActive, isStatic){ //skicka sedan in BBOX:en i konstruktorn!
		this->bbox = bbox;
	}

	CollisionObject(){}
	~CollisionObject(){

	}

	void UpdateBBOX(){ //ifall man flyttar runt detta object s� m�ste ox� BBOXen updateras!
		//bbox.Transform(bbox, world);
	}

	void CalculateWorld(){ //denna ska kallas innan man skickar in den i GPUn
		if (isActive == true && isStatic == false){ //kanske b�ttre att check isStatic i main koden ist�llet
			world = scale * rot * pos; //st�mmer detta?
			UpdateBBOX();
		}
	}
	//sedan b�r vi kanske ha en funktion GetIntersects() som h�mtar alla object som denne collidat med


	//	virtual bool isCollectable(){ return false; } //s� beh�ver man bara vara specifik med att det �r ett CollisionObject n�r man testar
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