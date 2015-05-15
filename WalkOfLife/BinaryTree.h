#pragma once
#include <iostream>
#include <vector>
#include "Platform.h"
#include "CollectableObject.h"

class BinaryTree
{
public:
	BinaryTree(int, int);
	~BinaryTree();
	std::vector<std::vector<Platform>>* testPlatforms;
	std::vector<std::vector<GameObject>>* renderObjects;
	std::vector<std::vector<CollectableObject>>* collectables;
	std::vector<std::vector<Platform>>* deadly;
	std::vector<std::vector<Platform>>* platformsMoving;
	std::vector<std::vector<Platform>>* deadlyMoving;
	std::vector<std::vector<CollectableObject>>* collectableMoving;
	void AddPlatform(Platform);
	void AddObject(GameObject);
	int pixelsPerdivision;

protected:
	
	int divisions;
};

