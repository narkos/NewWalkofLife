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
	void AddPlatform(Platform);
	int pixelsPerdivision;

protected:
	
	int divisions;
};

