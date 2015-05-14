#include "FBXImporter.h"

void FBXImporter::ImportFBX(ID3D11Device* gDevice, char* fileName){
	ifstream fbxFile;
	fbxFile.open(fileName, ios::in | ios::binary);

	FileInfo fileInfo;
	fbxFile.read((char*)&fileInfo.nrMeshes, sizeof(int));
	fbxFile.read((char*)&fileInfo.nrLights, sizeof(int));
	fbxFile.read((char*)&fileInfo.nrMaterials, sizeof(int));


	for (int i = 0; i < fileInfo.nrMeshes; i++){ //loopa igenom alla meshes
		MeshInfo meshInfo;
		fbxFile.read((char*)&meshInfo.meshType, sizeof(int));
		fbxFile.read((char*)&meshInfo.xInterval, sizeof(float));
		fbxFile.read((char*)&meshInfo.yInterval, sizeof(float));
		fbxFile.read((char*)&meshInfo.xSpeed, sizeof(float));
		fbxFile.read((char*)&meshInfo.ySpeed, sizeof(float));
		fbxFile.read((char*)&meshInfo.coinValue, sizeof(int));
		fbxFile.read((char*)&meshInfo.timeValue, sizeof(int));

		fbxFile.read((char*)&meshInfo.nrAnimations, sizeof(int));
		fbxFile.read((char*)&meshInfo.nrBones, sizeof(int));
		fbxFile.read((char*)&meshInfo.nrPos, sizeof(int));
		fbxFile.read((char*)&meshInfo.nrNor, sizeof(int));
		fbxFile.read((char*)&meshInfo.nrUV, sizeof(int));
		fbxFile.read((char*)&meshInfo.nrTangents, sizeof(int));
		fbxFile.read((char*)&meshInfo.nrFaces, sizeof(int));
		

		//ladda in indecies kanske?
		int const meshType = meshInfo.meshType;
		float const xInterval = meshInfo.xInterval;
		float const yInterval = meshInfo.yInterval;
		float const xSpeed = meshInfo.xSpeed;
		float const ySpeed = meshInfo.ySpeed;
		int const coinValue = meshInfo.coinValue;
		int const timeValue = meshInfo.timeValue;

		int const nrOfAnimations = meshInfo.nrAnimations;
		int const nrOfBones = meshInfo.nrBones;
		int const nrOfVertexPositions = meshInfo.nrPos;
		int const nrOfVertexNormals = meshInfo.nrNor;
		int const nrOfVertexUVs = meshInfo.nrUV;
		int const nrOfVertexTangents = meshInfo.nrTangents;
		int const nrOfFaces = meshInfo.nrFaces;

		ID3D11Buffer* meshVertexBuffer;


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
		HRESULT VertexBufferChecker = gDevice->CreateBuffer(&bDesc, &data, &meshVertexBuffer);
		
		delete verPos;
		delete verNor;
		delete verUV;
		delete verTangent;
		delete faces;
		
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
			Platform tempP(false, meshVertexBuffer, XMFLOAT3(0, 0, 0), true, true, bTemp, xInterval, yInterval, xSpeed, ySpeed);
			staticPlatforms.push_back(tempP);
		}
		else if (meshType == 1){ //nonstatic
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 0);
			Platform tempP(false, meshVertexBuffer, XMFLOAT3(0, 0, 0), true, false, bTemp, xInterval, yInterval, xSpeed, ySpeed);
			dynamicPlatforms.push_back(tempP);
		}
		else if (meshType == 2){ //player
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 0);
			PlayerObject tempPlayer(meshVertexBuffer, XMFLOAT3(0, 0, 0), true, false, bTemp, xInterval, yInterval, xSpeed, ySpeed); //importera speeden
			players.push_back(tempPlayer);
		}
		else if (meshType == 3){ //backgroundobj
			GameObject tempO(meshVertexBuffer, XMFLOAT3(0, 0, 0), true, true, xInterval, yInterval, xSpeed, ySpeed);
			backGroundObjects.push_back(tempO);
		}
		else if (meshType == 4){ //collectable
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 0);
			CollectableObject tempC(coinValue, timeValue, meshVertexBuffer, XMFLOAT3(0, 0, 0), true, true, bTemp, xInterval, yInterval, xSpeed, ySpeed);
			staticCollectableObjects.push_back(tempC);
		}
		else if (meshType == 5){ //deadly
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 0);
			Platform tempP(true, meshVertexBuffer, XMFLOAT3(0, 0, 0), true, true, bTemp, xInterval, yInterval, xSpeed, ySpeed);
			staticDeadlyObjects.push_back(tempP);
		}
		else if (meshType == 6){ //deadlymoving
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 0);
			Platform tempP(true, meshVertexBuffer, XMFLOAT3(0, 0, 0), true, false, bTemp, xInterval, yInterval, xSpeed, ySpeed);
			dynamicDeadlyObjects.push_back(tempP);
		}
		else if (meshType == 7){ //collectablemoving
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 0);
			CollectableObject tempC(coinValue, timeValue, meshVertexBuffer, XMFLOAT3(0, 0, 0), true, false, bTemp, xInterval, yInterval, xSpeed, ySpeed);
			dynamicCollectableObjects.push_back(tempC);
		}
		//delete meshVertexBuffer

		for (int i = 0; i < nrOfAnimations; i++){

		}

		for (int i = 0; i < nrOfBones; i++){

		}
		
		//MATERIAL
		XMFLOAT4 KD;
		XMFLOAT4 KA;
		XMFLOAT4 KS;
		float specularPowah, trans;

		fbxFile.read((char*)&KD.x, sizeof(float));
		fbxFile.read((char*)&KD.y, sizeof(float));
		fbxFile.read((char*)&KD.z, sizeof(float));
		KD.w = 0;

		fbxFile.read((char*)&KA.x, sizeof(float));
		fbxFile.read((char*)&KA.y, sizeof(float));
		fbxFile.read((char*)&KA.z, sizeof(float));
		KA.w = 0;

		fbxFile.read((char*)&KS.x, sizeof(float));
		fbxFile.read((char*)&KS.y, sizeof(float));
		fbxFile.read((char*)&KS.z, sizeof(float));
		KS.w = 0;

		fbxFile.read((char*)&specularPowah, sizeof(float));
		fbxFile.read((char*)&trans, sizeof(float));

		MatInfo material(KA, KD, KS, specularPowah);
		

	}

	for (int i = 0; i < fileInfo.nrMeshes; i++){

	}

	//lights

	//BBOXs


	fbxFile.close();
}
