#include "FBXImporter.h"

void FBXImporter::ImportFBX(ID3D11Device* gDevice, char* fileName){
	ifstream fbxFile;
	fbxFile.open(fileName, ios::in | ios::binary);

	FileInfo fileInfo;
	fbxFile.read((char*)&fileInfo.nrMeshes, sizeof(int));
	fbxFile.read((char*)&fileInfo.nrLights, sizeof(int));
	fbxFile.read((char*)&fileInfo.nrMaterials, sizeof(int));

	Mesh *meshes = new Mesh[fileInfo.nrMeshes];

	for (int i = 0; i < fileInfo.nrMeshes; i++){ //loopa igenom alla meshes
		MeshInfo meshInfo;
		fbxFile.read((char*)&meshInfo.meshType, sizeof(int));
		fbxFile.read((char*)&meshInfo.nrAnimations, sizeof(int));
		fbxFile.read((char*)&meshInfo.nrBones, sizeof(int));
		fbxFile.read((char*)&meshInfo.nrPos, sizeof(int));
		fbxFile.read((char*)&meshInfo.nrNor, sizeof(int));
		fbxFile.read((char*)&meshInfo.nrUV, sizeof(int));
		fbxFile.read((char*)&meshInfo.nrTangents, sizeof(int));
		fbxFile.read((char*)&meshInfo.nrFaces, sizeof(int));
		

		//ladda in indecies kanske?
		int const meshType = meshInfo.meshType;
		int const nrOfAnimations = meshInfo.nrAnimations;
		int const nrOfBones = meshInfo.nrBones;
		int const nrOfVertexPositions = meshInfo.nrPos;
		int const nrOfVertexNormals = meshInfo.nrNor;
		int const nrOfVertexUVs = meshInfo.nrUV;
		int const nrOfVertexTangents = meshInfo.nrTangents;
		int const nrOfFaces = meshInfo.nrFaces;


		//VertexData *vertecies = new VertexData[nrOfFaces * 3];
		vector<VertexData> vertecies;
		FaceData *faces = new FaceData[nrOfFaces];

		XMFLOAT3 *verPos = new XMFLOAT3[nrOfVertexPositions];
		XMFLOAT3 *verNor = new XMFLOAT3[nrOfVertexNormals];
		XMFLOAT2 *verUV = new XMFLOAT2[nrOfVertexUVs];
		XMFLOAT3 *verTangent = new XMFLOAT3[nrOfVertexTangents];
		//Float3 *verIndecies = new Float3[nrOfFaces * 3];

		//ladda in vertexvärden för meshen
		for (int p = 0; p < nrOfVertexPositions; p++){
			fbxFile.read((char*)&verPos[p].x, sizeof(float));
			fbxFile.read((char*)&verPos[p].y, sizeof(float));
			fbxFile.read((char*)&verPos[p].z, sizeof(float));
		}

		for (int p = 0; p < nrOfVertexNormals; p++){
			fbxFile.read((char*)&verNor[p].x, sizeof(float));
			fbxFile.read((char*)&verNor[p].y, sizeof(float));
			fbxFile.read((char*)&verNor[p].z, sizeof(float));
		}

		for (int p = 0; p < nrOfVertexUVs; p++){
			fbxFile.read((char*)&verUV[p].x, sizeof(float));
			fbxFile.read((char*)&verUV[p].y, sizeof(float));
		}

		for (int p = 0; p < nrOfVertexTangents; p++){
			fbxFile.read((char*)&verTangent[p].x, sizeof(float));
			fbxFile.read((char*)&verTangent[p].y, sizeof(float));
			fbxFile.read((char*)&verTangent[p].z, sizeof(float));
		}

		//tangent här

		//ladda in indeciesen och skapa vertiserna
		XMFLOAT3 tempVerIndex;
		VertexData tempVertex;
		for (int y = 0; y < nrOfFaces; y++){
			fbxFile.read((char*)&tempVerIndex.x, sizeof(float));
			fbxFile.read((char*)&tempVerIndex.y, sizeof(float));
			fbxFile.read((char*)&tempVerIndex.z, sizeof(float));

			faces[y].indexPos[0] = tempVerIndex.x;
			faces[y].indexNor[0] = tempVerIndex.y;
			faces[y].indexUV[0] = tempVerIndex.z;

			fbxFile.read((char*)&tempVerIndex.x, sizeof(float));
			fbxFile.read((char*)&tempVerIndex.y, sizeof(float));
			fbxFile.read((char*)&tempVerIndex.z, sizeof(float));

			faces[y].indexPos[1] = tempVerIndex.x;
			faces[y].indexNor[1] = tempVerIndex.y;
			faces[y].indexUV[1] = tempVerIndex.z;

			fbxFile.read((char*)&tempVerIndex.x, sizeof(float));
			fbxFile.read((char*)&tempVerIndex.y, sizeof(float));
			fbxFile.read((char*)&tempVerIndex.z, sizeof(float));

			faces[y].indexPos[2] = tempVerIndex.x;
			faces[y].indexNor[2] = tempVerIndex.y;
			faces[y].indexUV[2] = tempVerIndex.z;

			//skapa facet (vertiserna)!
			tempVertex.vertPos = verPos[faces[y].indexPos[0]];
			tempVertex.vertNor = verNor[faces[y].indexNor[0]];
			tempVertex.vertUV = verUV[faces[y].indexUV[0]];
			tempVertex.vertTangent = verTangent[faces[y].indexTangent[0]];
			vertecies.push_back(tempVertex); //vertex 1 i triangeln

			tempVertex.vertPos = verPos[faces[y].indexPos[1]];
			tempVertex.vertNor = verNor[faces[y].indexNor[1]];
			tempVertex.vertUV = verUV[faces[y].indexUV[1]];
			tempVertex.vertTangent = verTangent[faces[y].indexTangent[1]];
			vertecies.push_back(tempVertex); //vertex 2 i triangeln

			tempVertex.vertPos = verPos[faces[y].indexPos[2]];
			tempVertex.vertNor = verNor[faces[y].indexNor[2]];
			tempVertex.vertUV = verUV[faces[y].indexUV[2]];
			tempVertex.vertTangent = verTangent[faces[y].indexTangent[2]];
			vertecies.push_back(tempVertex); //vertex 3 i triangeln
		}

		D3D11_BUFFER_DESC bDesc;
		ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));
		bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bDesc.Usage = D3D11_USAGE_DEFAULT;
		bDesc.ByteWidth = sizeof(VertexData)*(vertecies.size());

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = vertecies.data();//<--------
		HRESULT VertexBufferChecker = gDevice->CreateBuffer(&bDesc, &data, &meshes[i].gVertexBuffer);


		float centerX, centerY, extentX, extentY;

		fbxFile.read((char*)&centerX, sizeof(float));
		fbxFile.read((char*)&centerY, sizeof(float));
		fbxFile.read((char*)&extentX, sizeof(float));
		fbxFile.read((char*)&extentY, sizeof(float));
		 //adda dem på listorna
		if (meshType == 0){ //static
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 0);
			Platform tempP(false, meshes[i].gVertexBuffer, XMFLOAT3(0,0,0), true, true, bTemp);
			staticPlatforms.push_back(tempP);
		}
		else if (meshType == 1){ //nonstatic
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 0);
			Platform tempP(false, meshes[i].gVertexBuffer, XMFLOAT3(0, 0, 0), true, false, bTemp);
			dynamicPlatforms.push_back(tempP);
		}
		else if (meshType == 2){ //player
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 0);
			PlayerObject tempPlayer(meshes[i].gVertexBuffer, XMFLOAT3(0, 0, 0), true, false, bTemp, 0.1f, 0.784f); //importera speeden
			players.push_back(tempPlayer);
		}
		else if (meshType == 3){ //backgroundobj
			GameObject tempO(meshes[i].gVertexBuffer, XMFLOAT3(0, 0, 0), true, true);
			backGroundObjects.push_back(tempO);
		}
		else if (meshType == 4){ //collectable
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 0);
			//CollectableObject tempC(coinValue, timeValue, meshes[i].gVertexBuffer, XMFLOAT3(0, 0, 0), true, true, bTemp);
		}
		else if (meshType == 5){ //deadly
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 0);
			Platform tempP(true, meshes[i].gVertexBuffer, XMFLOAT3(0, 0, 0), true, true, bTemp);
			staticDeadlyObjects.push_back(tempP);
		}
		else if (meshType == 6){ //deadlymoving
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 0);
			Platform tempP(true, meshes[i].gVertexBuffer, XMFLOAT3(0, 0, 0), true, false, bTemp);
			dynamicDeadlyObjects.push_back(tempP);
		}
		else if (meshType == 7){ //collectablemoving
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 0);
			//CollectableObject tempC(coinValue, timeValue, meshes[i].gVertexBuffer, XMFLOAT3(0, 0, 0), true, false, bTemp);
		}

		//meshes[i].vertices = vertecies;

		delete verPos;
		delete verNor;
		delete verUV;
		delete verTangent;
		//delete verIndecies;

	}

	//lights

	//BBOXs


	fbxFile.close();
}
