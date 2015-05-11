#pragma once
#include <iostream>
#include <vector>
#include "Platform.h"

class BinaryTree
{
public:
	BinaryTree(int, int);
	~BinaryTree();
	std::vector<std::vector<Platform>>* testPlatforms;
	std::vector<std::vector<GameObject>>* renderObjects;
	void AddPlatform(Platform);
	void AddObject(GameObject);
	int pixelsPerdivision;

protected:
	
	int divisions;
};

