#include "BinaryTree.h"


BinaryTree::BinaryTree(int divisions, int pixelsPerDivision)
{
	this->divisions = divisions;
	this->pixelsPerdivision = pixelsPerDivision;
	
	this->testPlatforms = new std::vector<std::vector<Platform>>(100);
	this->renderObjects = new std::vector<std::vector<GameObject>>(100);
	this->collectables = new std::vector<std::vector<CollectableObject>>(100);
	this->deadly = new std::vector<std::vector<Platform>>(100);
}


BinaryTree::~BinaryTree()
{
}

void BinaryTree::AddPlatform(Platform thePlatform)
{
	if (thePlatform.GetBBOX().Center.x < 49 * 0)
	{
		testPlatforms->at(0).push_back(thePlatform);
	}

	else
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

void BinaryTree::addCollectables(CollectableObject theObject)
{
	if (theObject.GetBBOX().Center.x > 49 * pixelsPerdivision)
	{
		if (theObject.GetBBOX().Center.x > 74 * pixelsPerdivision)
		{
			for (int i = 0; i < 25; i++)
			{
				if ((theObject.GetBBOX().Center.x >= 75 * pixelsPerdivision + i*pixelsPerdivision) && (theObject.GetBBOX().Center.x < 75 * pixelsPerdivision + (i*pixelsPerdivision) + pixelsPerdivision))
				{
					collectables->at(i).push_back(theObject);
					break;
				}
			}
		}

		else
		{
			for (int i = 0; i < 25; i++)
			{
				if ((theObject.GetBBOX().Center.x >= 50 * pixelsPerdivision + i*pixelsPerdivision) && (theObject.GetBBOX().Center.x < 50 * pixelsPerdivision + (i*pixelsPerdivision) + pixelsPerdivision))
				{
					collectables->at(i).push_back(theObject);
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
				if ((theObject.GetBBOX().Center.x >= 25 * pixelsPerdivision + i*pixelsPerdivision) && (theObject.GetBBOX().Center.x < 25 * pixelsPerdivision + (i*pixelsPerdivision) + pixelsPerdivision))
				{
					collectables->at(i).push_back(theObject);
					break;
				}
			}
		}

		else
		{
			for (int i = 0; i < 25; i++)
			{
				if ((theObject.GetBBOX().Center.x >= i*pixelsPerdivision) && (theObject.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
				{
					collectables->at(i).push_back(theObject);
					break;
				}
			}
		}
	}
}

void BinaryTree::addMovingCollectable(CollectableObject theObject)
{
	if (theObject.xPos > 49 * pixelsPerdivision)
	{
		if (theObject.xPos > 74 * pixelsPerdivision)
		{
			for (int i = 0; i < 25; i++)
			{
				if ((theObject.xPos >= 75 * pixelsPerdivision + i*pixelsPerdivision) && (theObject.xPos < 75 * pixelsPerdivision + (i*pixelsPerdivision) + pixelsPerdivision))
				{
					collectableMoving->at(i).push_back(theObject);
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
					collectableMoving->at(i).push_back(theObject);
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
					collectableMoving->at(i).push_back(theObject);
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
					collectableMoving->at(i).push_back(theObject);
					break;
				}
			}
		}
	}
}

void BinaryTree::addMovingPlatforms(Platform thePlatform)
{
	if (thePlatform.GetBBOX().Center.x > 49 * pixelsPerdivision)
	{
		if (thePlatform.GetBBOX().Center.x > 74 * pixelsPerdivision)
		{
			for (int i = 0; i < 25; i++)
			{
				if ((thePlatform.GetBBOX().Center.x >= 75 * pixelsPerdivision + i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < 75 * pixelsPerdivision + (i*pixelsPerdivision) + pixelsPerdivision))
				{
					platformsMoving->at(i).push_back(thePlatform);
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
					platformsMoving->at(i).push_back(thePlatform);
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
					platformsMoving->at(i).push_back(thePlatform);
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
					platformsMoving->at(i).push_back(thePlatform);
					break;
				}
			}
		}
	}

}


void BinaryTree::addDeadlyMovingPlatform(Platform thePlatform)
{
	if (thePlatform.GetBBOX().Center.x > 49 * pixelsPerdivision)
	{
		if (thePlatform.GetBBOX().Center.x > 74 * pixelsPerdivision)
		{
			for (int i = 0; i < 25; i++)
			{
				if ((thePlatform.GetBBOX().Center.x >= 75 * pixelsPerdivision + i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < 75 * pixelsPerdivision + (i*pixelsPerdivision) + pixelsPerdivision))
				{
					deadlyMoving->at(i).push_back(thePlatform);
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
					deadlyMoving->at(i).push_back(thePlatform);
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
					deadlyMoving->at(i).push_back(thePlatform);
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
					deadlyMoving->at(i).push_back(thePlatform);
					break;
				}
			}
		}
	}

}

void BinaryTree::addDeadlyPlatforms(Platform thePlatform)
{
	if (thePlatform.GetBBOX().Center.x > 49 * pixelsPerdivision)
	{
		if (thePlatform.GetBBOX().Center.x > 74 * pixelsPerdivision)
		{
			for (int i = 0; i < 25; i++)
			{
				if ((thePlatform.GetBBOX().Center.x >= 75 * pixelsPerdivision + i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < 75 * pixelsPerdivision + (i*pixelsPerdivision) + pixelsPerdivision))
				{
					deadly->at(i).push_back(thePlatform);
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
					deadly->at(i).push_back(thePlatform);
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
					deadly->at(i).push_back(thePlatform);
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
					deadly->at(i).push_back(thePlatform);
					break;
				}
			}
		}
	}

}