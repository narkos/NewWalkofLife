#include "FBXImporter.h"
#include "string"
void FBXImporter::ImportFBX(ID3D11Device* gDevice, char* fileName){
	ifstream fbxFile;
	fbxFile.open(fileName, ifstream::binary);

	FileInfo fileInfo;
	fbxFile.read((char*)&fileInfo.nrMeshes, sizeof(int));
	fbxFile.read((char*)&fileInfo.nrLights, sizeof(int));
	//fbxFile.read((char*)&fileInfo.nrMaterials, sizeof(int));

	int mesnNumberInt=0;
	for (int i = 0; i < fileInfo.nrMeshes; i++){ //loopa igenom alla meshes
		MeshInfo meshInfo;
		
		fbxFile.read((char*)&meshInfo.nrTex, sizeof(int));
		
		fbxFile.read((char*)meshInfo.name.c_str(), meshInfo.nrTex);
		std::string qs = std::to_string(meshInfo.nrTex);
	testTexNameArray.push_back(meshInfo.name.c_str());

		
		if (i>335)
		{
			int bajs = 2;
		}

	
		
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
		int meshType = meshInfo.meshType;
		float xInterval = meshInfo.xInterval;
		float yInterval = meshInfo.yInterval;
		float xSpeed = meshInfo.xSpeed;
		float ySpeed = meshInfo.ySpeed;
		int coinValue = meshInfo.coinValue;
		int timeValue = meshInfo.timeValue;


		// Why? /Joakim
		if (xInterval < -1000)
			xInterval = 0;
		if (yInterval < -1000)
			yInterval = 0;
		if (xSpeed < -1000)
			xSpeed = 0;
		if (ySpeed < -1000)
			ySpeed = 0;

		int nrOfAnimations = meshInfo.nrAnimations;
		int nrOfBones = meshInfo.nrBones;
		int nrOfVertexPositions = meshInfo.nrPos;
		int nrOfVertexNormals = meshInfo.nrNor;
		int nrOfVertexUVs = meshInfo.nrUV;
		int nrOfVertexTangents = meshInfo.nrTangents;
		int nrOfFaces = meshInfo.nrFaces;

		ID3D11Buffer* meshVertexBuffer;

		vector<VertexData> vertecies;
		/*FaceData *faces = new FaceData[nrOfFaces];

		XMFLOAT3 *verPos = new XMFLOAT3[nrOfVertexPositions];
		XMFLOAT3 *verNor = new XMFLOAT3[nrOfVertexNormals];
		XMFLOAT2 *verUV = new XMFLOAT2[nrOfVertexUVs];
		XMFLOAT3 *verTangent = new XMFLOAT3[nrOfVertexTangents];
		*/
		vector<FaceData> faces;
		vector<XMFLOAT3> verPos;
		vector<XMFLOAT3> verNor;
		vector<XMFLOAT2> verUV;
		vector<XMFLOAT3> verTangent;

		XMFLOAT3 temp3;
		XMFLOAT2 temp2;

		//ladda in vertexv�rden f�r meshen
		for (int p = 0; p < nrOfVertexPositions; p++){
			XMFLOAT3 temp3;
		
			fbxFile.read((char*)&temp3.x, sizeof(float));
			fbxFile.read((char*)&temp3.y, sizeof(float));
			fbxFile.read((char*)&temp3.z, sizeof(float));
			temp3.z = temp3.z*-1;
			verPos.push_back(temp3);
		}
		
		for (int p = 0; p < nrOfVertexNormals; p++){
			XMFLOAT3 temp3;
		
			fbxFile.read((char*)&temp3.x, sizeof(float));
			fbxFile.read((char*)&temp3.y, sizeof(float));
			fbxFile.read((char*)&temp3.z, sizeof(float));
			temp2.y = temp2.y*-1;
			verNor.push_back(temp3);
		}

		for (int p = 0; p < nrOfVertexUVs; p++){
			XMFLOAT2 temp2;

			fbxFile.read((char*)&temp2.x, sizeof(float));
			fbxFile.read((char*)&temp2.y, sizeof(float));
			temp2.y = temp2.y*-1;
			verUV.push_back(temp2);
		}

		for (int p = 0; p < nrOfVertexTangents; p++){
			XMFLOAT3 temp3;
		
			fbxFile.read((char*)&temp3.x, sizeof(float));
			fbxFile.read((char*)&temp3.y, sizeof(float));
			fbxFile.read((char*)&temp3.z, sizeof(float));
			verTangent.push_back(temp3);
		}

		//tangent h�r

		//ladda in indeciesen och skapa vertiserna
		Int4 tempVerIndex;
		VertexData tempVertex;
		FaceData tempFaceData;
		for (int y = 0; y < nrOfFaces; y++){
			fbxFile.read((char*)&tempVerIndex.x, sizeof(int));
			fbxFile.read((char*)&tempVerIndex.y, sizeof(int));
			fbxFile.read((char*)&tempVerIndex.z, sizeof(int));
			
			tempFaceData.indexPos[0] = tempVerIndex.x;
			tempFaceData.indexUV[0] = tempVerIndex.y;
			tempFaceData.indexNor[0] = tempVerIndex.z;
			
			//tempFaceData.indexTangent[0] = tempVerIndex.w;
			
			fbxFile.read((char*)&tempVerIndex.x, sizeof(int));
			fbxFile.read((char*)&tempVerIndex.y, sizeof(int));
			fbxFile.read((char*)&tempVerIndex.z, sizeof(int));
			

			tempFaceData.indexPos[1] = tempVerIndex.x;
			tempFaceData.indexUV[1] = tempVerIndex.y;
			tempFaceData.indexNor[1] = tempVerIndex.z;
			
			fbxFile.read((char*)&tempVerIndex.x, sizeof(int));
			fbxFile.read((char*)&tempVerIndex.y, sizeof(int));
			fbxFile.read((char*)&tempVerIndex.z, sizeof(int));
			

			tempFaceData.indexPos[2] = tempVerIndex.x;
			tempFaceData.indexUV[2] = tempVerIndex.y;
			tempFaceData.indexNor[2] = tempVerIndex.z;
			
			faces.push_back(tempFaceData);

			//skapa facet (vertiserna)!
			tempVertex.vertPos = verPos[faces[y].indexPos[2]-1];
			tempVertex.vertUV = verUV[faces[y].indexUV[2]-1];
			tempVertex.vertNor = verNor[faces[y].indexNor[2] - 1];
			//tempVertex.vertTangent = verTangent[0];
			vertecies.push_back(tempVertex); //vertex 1 i triangeln

			tempVertex.vertPos = verPos[faces[y].indexPos[1]-1];			
			tempVertex.vertUV = verUV[faces[y].indexUV[1]-1];
			tempVertex.vertNor = verNor[faces[y].indexNor[1] - 1];
			//tempVertex.vertTangent = verTangent[0];
			vertecies.push_back(tempVertex); //vertex 2 i triangeln

			tempVertex.vertPos = verPos[faces[y].indexPos[0]-1];
			tempVertex.vertUV = verUV[faces[y].indexUV[0]-1];
			tempVertex.vertNor = verNor[faces[y].indexNor[0] - 1];
			//tempVertex.vertTangent = verTangent[0];
			vertecies.push_back(tempVertex); //vertex 3 i triangeln
		}

		D3D11_BUFFER_DESC bDesc;
		ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));
		bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bDesc.Usage = D3D11_USAGE_DEFAULT;
		bDesc.ByteWidth = (UINT)(sizeof(VertexData)*(vertecies.size()));

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = vertecies.data();//<--------
		HRESULT VertexBufferChecker = gDevice->CreateBuffer(&bDesc, &data, &meshVertexBuffer);
		
		//delete verPos;
		//delete verNor;
		//delete verUV;
		//delete verTangent;
		//delete faces;

		verPos.clear();
		verNor.clear();
		verUV.clear();
		verTangent.clear();
		faces.clear();
		
		float centerX, centerY, extentX, extentY;

		fbxFile.read((char*)&centerX, sizeof(float));
		fbxFile.read((char*)&centerY, sizeof(float));
		fbxFile.read((char*)&extentX, sizeof(float));
		fbxFile.read((char*)&extentY, sizeof(float));

		//MATERIAL!!!!!!
		XMFLOAT4 KD;
		XMFLOAT4 KA;
		XMFLOAT4 KS;
		float specularPowah, trans;

		fbxFile.read((char*)&KD.x, sizeof(float));
		fbxFile.read((char*)&KD.y, sizeof(float));
		fbxFile.read((char*)&KD.z, sizeof(float));
		KD.w = 1.0f;

		fbxFile.read((char*)&KA.x, sizeof(float));
		fbxFile.read((char*)&KA.y, sizeof(float));
		fbxFile.read((char*)&KA.z, sizeof(float));
		KA.w = 1.0f;

		fbxFile.read((char*)&KS.x, sizeof(float));
		fbxFile.read((char*)&KS.y, sizeof(float));
		fbxFile.read((char*)&KS.z, sizeof(float));
		KS.w = 1.0f;

		fbxFile.read((char*)&specularPowah, sizeof(float));
		fbxFile.read((char*)&trans, sizeof(float));

		//MatInfo material(KA, KD, KS, specularPowah);


		 //adda dem p� listorna
		if (meshType == 0 || meshType < 0){ //static
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0.0f);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 1000.0f);
			Platform tempP(false, meshVertexBuffer, XMFLOAT3(0.0f, 0.0f, 0.0f), true, true, bTemp, xInterval, yInterval, xSpeed, ySpeed);
			tempP.material.Diffuse = KD;
			tempP.material.Ambient = KA;
			tempP.material.Specular = KS;
			tempP.material.SpecPow = specularPowah;
			tempP.nrElements = nrOfFaces;
			tempP.indexT = mesnNumberInt;
			mesnNumberInt++;
			staticPlatforms.push_back(tempP);
		}
		else if (meshType == 1){ //nonstatic
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0.0f);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 10.0f);
			Platform tempP(false, meshVertexBuffer, XMFLOAT3(0.0f, 0.0f, 0.0f), true, false, bTemp, xInterval, yInterval, xSpeed, ySpeed);
			tempP.material.Diffuse = KD;
			tempP.material.Ambient = KA;
			tempP.material.Specular = KS;
			tempP.material.SpecPow = specularPowah;
			tempP.nrElements = nrOfFaces;
			tempP.indexT = mesnNumberInt;
			mesnNumberInt++;
			dynamicPlatforms.push_back(tempP);
		}
		else if (meshType == 2){ //player
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0.0f);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 10.0f);
			PlayerObject tempPlayer(meshVertexBuffer, XMFLOAT3(0.0f, 0.0f, 0.0f), true, false, bTemp, xInterval, yInterval, xSpeed, ySpeed); //importera speeden
			tempPlayer.material.Diffuse = KD;
			tempPlayer.material.Ambient = KA;
			tempPlayer.material.Specular = KS;
			tempPlayer.material.SpecPow = specularPowah;
			tempPlayer.nrElements = nrOfFaces;
			tempPlayer.indexT = mesnNumberInt;
			mesnNumberInt++;
			players.push_back(tempPlayer);
		}
		else if (meshType == 3){ //backgroundobj
			GameObject tempO(meshVertexBuffer, XMFLOAT3(0.0f, 0.0f, 0.0f), true, true, xInterval, yInterval, xSpeed, ySpeed);
			tempO.material.Diffuse = KD;
			tempO.material.Ambient = KA;
			tempO.material.Specular = KS;
			tempO.material.SpecPow = specularPowah;
			tempO.xPos = centerX;
			tempO.nrElements = nrOfFaces;
			tempO.indexT = mesnNumberInt;

			mesnNumberInt++;
			backGroundObjects.push_back(tempO);
		}
		else if (meshType == 4){ //collectable
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0.0f);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 10.0f);
			CollectableObject tempC(coinValue, timeValue, meshVertexBuffer, XMFLOAT3(0.0f, 0.0f, 0.0f), true, true, bTemp, xInterval, yInterval, xSpeed, ySpeed);
			tempC.material.Diffuse = KD;
			tempC.material.Ambient = KA;
			tempC.material.Specular = KS;
			tempC.material.SpecPow = specularPowah;
			tempC.nrElements = nrOfFaces;
			tempC.indexT = mesnNumberInt;

			mesnNumberInt++;
			staticCollectableObjects.push_back(tempC);
		}
		else if (meshType == 5){ //deadly
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0.0f);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 10.0f);
			Platform tempP(true, meshVertexBuffer, XMFLOAT3(0.0f, 0.0f, 0.0f), true, true, bTemp, xInterval, yInterval, xSpeed, ySpeed);
			tempP.material.Diffuse = KD;
			tempP.material.Ambient = KA;
			tempP.material.Specular = KS;
			tempP.material.SpecPow = specularPowah;
			tempP.nrElements = nrOfFaces;
			tempP.indexT = mesnNumberInt;

			mesnNumberInt++;
			staticDeadlyObjects.push_back(tempP);
		}
		else if (meshType == 6){ //deadlymoving
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0.0f);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 10.0f);
			Platform tempP(true, meshVertexBuffer, XMFLOAT3(0.0f, 0.0f, 0.0f), true, false, bTemp, xInterval, yInterval, xSpeed, ySpeed);
			tempP.material.Diffuse = KD;
			tempP.material.Ambient = KA;
			tempP.material.Specular = KS;
			tempP.material.SpecPow = specularPowah;
			tempP.slamDirection = coinValue;
			tempP.slamSpeedMultiplier = ySpeed;
			tempP.slamWaitTime = xSpeed;
			tempP.slamTimeOffset = xInterval;
			tempP.nrElements = nrOfFaces;
			tempP.indexT = mesnNumberInt;
			mesnNumberInt++;
			dynamicDeadlyObjects.push_back(tempP);
		}
		else if (meshType == 7){ //collectablemoving
			BoundingBox bTemp;
			bTemp.Center = XMFLOAT3(centerX, centerY, 0.0f);
			bTemp.Extents = XMFLOAT3(extentX, extentY, 10.0f);
			CollectableObject tempC(coinValue, timeValue, meshVertexBuffer, XMFLOAT3(0.0f, 0.0f, 0.0f), true, false, bTemp, xInterval, yInterval, xSpeed, ySpeed);
			tempC.material.Diffuse = KD;
			tempC.material.Ambient = KA;
			tempC.material.Specular = KS;
			tempC.material.SpecPow = specularPowah;
			tempC.nrElements = nrOfFaces;		
			tempC.indexT = mesnNumberInt;

			mesnNumberInt++;
			dynamicCollectableObjects.push_back(tempC);
		}
		//delete meshVertexBuffer

		//for (int i = 0; i < nrOfAnimations; i++){

		//}

		//for (int i = 0; i < nrOfBones; i++){

		//}
		


	}

	//lights

	
	fbxFile.close();

	int cp = 0;
		for (int f = 0; f < testTexNameArray.size(); f++){
			
			const char* cmpString3 =testTexNameArray[f].c_str();
			if (f == 0){
				std::string finalTexPath =cmpString3;

				texNameArray.push_back(finalTexPath.c_str());
			}
			else if (f > 0){
				std::string finalTexPath1;
				for (int f2 = 0; f2 < texNameArray.size(); f2++){
					
					int t2 = 0;
					if (cmpString3 != texNameArray[f2]){
				
						t2 = 1;
						finalTexPath1 = cmpString3;
						for (int f3 = 0; f3 < texNameArray.size(); f3++){
						if (cmpString3 == texNameArray[f3])
							t2=0;
						
						
					}
					if (f2 == texNameArray.size() - 1 && t2 == 1){
						finalTexPath1 =  cmpString3;
						texNameArray.push_back(finalTexPath1.c_str());
						cp++;
					}
					}
			}
			}
		}
		for (int f = 0; f < testTexNameArray.size(); f++){
			const char* cmpString3 = testTexNameArray[f].c_str();
			for (int f5 = 0; f5 < texNameArray.size(); f5++){			
				if (cmpString3 == texNameArray[f5]){
					indexArray.push_back(f5);
				}

			}
		}

		int qbajs = 0;

}
