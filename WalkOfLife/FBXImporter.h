#include <iostream>
#include <fstream>
#include <d3d11.h>
#include <vector>

#include "Entity.h"
#include "CollectableObject.h"
#include "CollisionObject.h"
#include "GameObject.h"
#include "Platform.h"
#include "PlayerObject.h"


using namespace std;

//infos

class FBXImporter{
private:
	vector<Platform> staticPlatforms;
	vector<Platform> dynamicPlatforms;
	vector<PlayerObject> players;
	vector<GameObject> backGroundObjects;
	vector<CollectableObject> staticCollectableObjects;
	vector<Platform> staticDeadlyObjects; //använder denna som platformen ändå!
	vector<Platform> dynamicDeadlyObjects;
	vector<CollectableObject> dynamicCollectableObjects;


	struct FileInfo{
		int nrMeshes;
		int nrLights;
		int nrMaterials;
	};

	struct MeshInfo{
		int meshType;
		float xInterval;
		float yInterval;
		float xSpeed;
		float ySpeed;
		int coinValue;
		int timeValue;
		int nrAnimations;
		int nrBones;
		int nrPos;
		int nrNor;
		int nrUV;
		int nrTangents;
		int nrFaces;

	};

	struct VertexData{
		XMFLOAT3 vertPos;
		XMFLOAT3 vertNor;
		XMFLOAT2 vertUV;
		//XMFLOAT3 vertTangent;
	};

	struct FaceData{
		int indexPos[3];
		int indexNor[3];
		int indexUV[3];
		int indexTangent[3];
	};

	struct MaterialData{
		float kd[3];
		float ka[3];
		float ks[3];
		float trans;
	};



	struct Material{
		int byteSizeOfName;
		string name;

		XMFLOAT3 ka, kd, ks;
	};

	struct Int4{
		int x, y, z, w;
	};

	struct Float4{
		float x, y, z, w;
	};

public:
	FBXImporter(){}
	~FBXImporter(){}

	void ImportFBX(ID3D11Device* gDevice, char* fileName);

	vector<Platform> GetStaticPlatforms(){
		return staticPlatforms;
	}
	vector<Platform> GetDynamicPlatforms(){
		return dynamicPlatforms;
	}
	vector<PlayerObject> GetPlayers(){
		return players;
	}
	vector<GameObject> GetBackGroundObjects(){
		return backGroundObjects;
	}
	vector<CollectableObject> GetStaticCollectableObjects(){
		return staticCollectableObjects;
	}
	vector<Platform> GetStaticDeadlyObjects(){
		return staticDeadlyObjects;
	}
	vector<Platform> GetDynamicDeadlyObjects(){
		return dynamicDeadlyObjects;
	}
	vector<CollectableObject> GetDynamicCollectableObject(){
		return dynamicCollectableObjects;
	}
};