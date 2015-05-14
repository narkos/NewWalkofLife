#include "BinaryTree.h"


BinaryTree::BinaryTree(int divisions, int pixelsPerDivision)
{
	this->divisions = divisions;
	this->pixelsPerdivision = pixelsPerDivision;
	
	this->testPlatforms = new std::vector<std::vector<Platform>>(100);
	this->renderObjects = new std::vector<std::vector<GameObject>>(100);
}


BinaryTree::~BinaryTree()
{
}

void BinaryTree::AddPlatform(Platform thePlatform)
{
	if (thePlatform.GetBBOX().Center.x > 49 * pixelsPerdivision)
	{
		if (thePlatform.GetBBOX().Center.x > 74 * pixelsPerdivision)
		{
				for (int i = 0; i < 25; i++)
				{
					if ((thePlatform.GetBBOX().Center.x >= 75 * pixelsPerdivision + i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < 75 * pixelsPerdivision + (i*pixelsPerdivision) + pixelsPerdivision))
					{
						testPlatforms->at(i).push_back(thePlatform);
						break;
					}
				}
		}

		else
		{
			for (int i = 0; i < 25; i++)
			{
				if ((thePlatform.GetBBOX().Center.x >= 50 * pixelsPerdivision + i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < 50 * pixelsPerdivision + (i*pixelsPerdivision) + pixelsPerdivision))
				{
					testPlatforms->at(i).push_back(thePlatform);
					break;
				}
			}
		}
	}

	else
	{
		if (thePlatform.GetBBOX().Center.x > 24 * pixelsPerdivision)
		{
			for (int i = 0; i < 25; i++)
			{
				if ((thePlatform.GetBBOX().Center.x >= 25 * pixelsPerdivision + i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < 25 * pixelsPerdivision + (i*pixelsPerdivision) + pixelsPerdivision))
				{
					testPlatforms->at(i).push_back(thePlatform);
					break;
				}
			}
		}

		else
		{
			for (int i = 0; i < 25; i++)
			{
				if ((thePlatform.GetBBOX().Center.x >= i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
				{
					testPlatforms->at(i).push_back(thePlatform);
					break;
				}
			}
		}
	}
	
}

void BinaryTree::AddObject(GameObject theObject)
{
	if (theObject.xPos > 49 * pixelsPerdivision)
	{
		if (theObject.xPos > 74 * pixelsPerdivision)
		{
			for (int i = 0; i < 25; i++)
			{
				if ((theObject.xPos >= 75 * pixelsPerdivision + i*pixelsPerdivision) && (theObject.xPos < 75 * pixelsPerdivision + (i*pixelsPerdivision) + pixelsPerdivision))
				{
					renderObjects->at(i).push_back(theObject);
					break;
				}
			}
		}

		else
		{
			for (int i = 0; i < 25; i++)
			{
				if ((theObject.xPos >= 50 * pixelsPerdivision + i*pixelsPerdivision) && (theObject.xPos < 50 * pixelsPerdivision + (i*pixelsPerdivision) + pixelsPerdivision))
				{
					renderObjects->at(i).push_back(theObject);
					break;
				}
			}
		}
	}

	else
	{
		if (theObject.xPos > 24 * pixelsPerdivision)
		{
			for (int i = 0; i < 25; i++)
			{
				if ((theObject.xPos >= 25 * pixelsPerdivision + i*pixelsPerdivision) && (theObject.xPos < 25 * pixelsPerdivision + (i*pixelsPerdivision) + pixelsPerdivision))
				{
					renderObjects->at(i).push_back(theObject);
					break;
				}
			}
		}

		else
		{
			for (int i = 0; i < 25; i++)
			{
				if ((theObject.xPos >= i*pixelsPerdivision) && (theObject.xPos < (i*pixelsPerdivision) + pixelsPerdivision))
				{
					renderObjects->at(i).push_back(theObject);
					break;
				}
			}
		}
	}
}

