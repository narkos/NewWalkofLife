#include "BinaryTree.h"


BinaryTree::BinaryTree(int divisions, int pixelsPerDivision)
{
	this->divisions = divisions;
	this->pixelsPerdivision = pixelsPerDivision;
	
	this->testPlatforms = new std::vector<std::vector<Platform>>(20);
	this->renderObjects = new std::vector<std::vector<GameObject>>(20);
	this->collectables = new std::vector<std::vector<CollectableObject>>(20);
	this->deadly = new std::vector<std::vector<Platform>>(20);
	this->collectableMoving = new std::vector<std::vector<CollectableObject>>(20);
	this->platformsMoving = new std::vector<std::vector<Platform>>(20);
	this->deadlyMoving = new std::vector<std::vector<Platform>>(20);

}


BinaryTree::~BinaryTree()
{
	this->testPlatforms->clear();
	this->renderObjects->clear();
	this->collectables->clear();
	this->deadly->clear();
	this->collectableMoving->clear();
	this->platformsMoving->clear();
	this->deadlyMoving->clear();
}

void BinaryTree::AddPlatform(Platform thePlatform)
{
	if (thePlatform.GetBBOX().Center.x < 9 * 0)
	{
		testPlatforms->at(0).push_back(thePlatform);
	}

	else
	{
		if (thePlatform.GetBBOX().Center.x > 9 * pixelsPerdivision)
			{
				if (thePlatform.GetBBOX().Center.x > 14 * pixelsPerdivision)
				{
						for (int i = 14; i < 20; i++)
						{
							if ((thePlatform.GetBBOX().Center.x >= i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
							{
								testPlatforms->at(i).push_back(thePlatform);
								break;
							}
						}
				}

				else
				{
					for (int i = 9; i < 14; i++)
					{
						if ((thePlatform.GetBBOX().Center.x >= i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
						{
							testPlatforms->at(i).push_back(thePlatform);
							break;
						}
					}
				}
			}

			else
			{
				if (thePlatform.GetBBOX().Center.x > 4 * pixelsPerdivision)
				{
					for (int i = 4; i < 9; i++)
					{
						if ((thePlatform.GetBBOX().Center.x >= i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
						{
							testPlatforms->at(i).push_back(thePlatform);
							break;
						}
					}
				}

				else
				{
					for (int i = 0; i < 4; i++)
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
	if (theObject.xPos < 9 * 0)
	{
		renderObjects->at(0).push_back(theObject);
	}

	else
	{
		if (theObject.xPos > 9 * pixelsPerdivision)
		{
			if (theObject.xPos > 14 * pixelsPerdivision)
			{
				for (int i = 14; i < 20; i++)
				{
					if ((theObject.xPos >= i*pixelsPerdivision) && (theObject.xPos < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						renderObjects->at(i).push_back(theObject);
						break;
					}
				}
			}

			else
			{
				for (int i = 9; i < 14; i++)
				{
					if ((theObject.xPos >= i*pixelsPerdivision) && (theObject.xPos < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						renderObjects->at(i).push_back(theObject);
						break;
					}
				}
			}
		}

		else
		{
			if (theObject.xPos > 4 * pixelsPerdivision)
			{
				for (int i = 4; i < 9; i++)
				{
					if ((theObject.xPos  >= i*pixelsPerdivision) && (theObject.xPos < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						renderObjects->at(i).push_back(theObject);
						break;
					}
				}
			}

			else
			{
				for (int i = 0; i < 4; i++)
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

}

void BinaryTree::addCollectables(CollectableObject theObject)
{
	if (theObject.GetBBOX().Center.x < 9 * 0)
	{
		collectables->at(0).push_back(theObject);
	}

	else
	{
		if (theObject.GetBBOX().Center.x > 9 * pixelsPerdivision)
		{
			if (theObject.GetBBOX().Center.x > 14 * pixelsPerdivision)
			{
				for (int i = 14; i < 20; i++)
				{
					if ((theObject.GetBBOX().Center.x >= i*pixelsPerdivision) && (theObject.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						collectables->at(i).push_back(theObject);
						break;
					}
				}
			}

			else
			{
				for (int i = 9; i < 14; i++)
				{
					if ((theObject.GetBBOX().Center.x >= i*pixelsPerdivision) && (theObject.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						collectables->at(i).push_back(theObject);
						break;
					}
				}
			}
		}

		else
		{
			if (theObject.GetBBOX().Center.x > 4 * pixelsPerdivision)
			{
				for (int i = 4; i < 9; i++)
				{
					if ((theObject.GetBBOX().Center.x >= i*pixelsPerdivision) && (theObject.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						collectables->at(i).push_back(theObject);
						break;
					}
				}
			}

			else
			{
				for (int i = 0; i < 4; i++)
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

}

void BinaryTree::addMovingCollectable(CollectableObject theObject)
{
	if (theObject.GetBBOX().Center.x < 9 * 0)
	{
		collectableMoving->at(0).push_back(theObject);
	}

	else
	{
		if (theObject.GetBBOX().Center.x > 9 * pixelsPerdivision)
		{
			if (theObject.GetBBOX().Center.x > 14 * pixelsPerdivision)
			{
				for (int i = 14; i < 20; i++)
				{
					if ((theObject.GetBBOX().Center.x >= i*pixelsPerdivision) && (theObject.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						collectableMoving->at(i).push_back(theObject);
						break;
					}
				}
			}

			else
			{
				for (int i = 9; i < 14; i++)
				{
					if ((theObject.GetBBOX().Center.x >= i*pixelsPerdivision) && (theObject.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						collectableMoving->at(i).push_back(theObject);
						break;
					}
				}
			}
		}

		else
		{
			if (theObject.GetBBOX().Center.x > 4 * pixelsPerdivision)
			{
				for (int i = 4; i < 9; i++)
				{
					if ((theObject.GetBBOX().Center.x >= i*pixelsPerdivision) && (theObject.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						collectableMoving->at(i).push_back(theObject);
						break;
					}
				}
			}

			else
			{
				for (int i = 0; i < 4; i++)
				{
					if ((theObject.GetBBOX().Center.x >= i*pixelsPerdivision) && (theObject.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						collectableMoving->at(i).push_back(theObject);
						break;
					}
				}
			}
		}

	}

}

void BinaryTree::addMovingPlatforms(Platform thePlatform)
{
	if (thePlatform.GetBBOX().Center.x < 9 * 0)
	{
		platformsMoving->at(0).push_back(thePlatform);
	}

	else
	{
		if (thePlatform.GetBBOX().Center.x > 9 * pixelsPerdivision)
		{
			if (thePlatform.GetBBOX().Center.x > 14 * pixelsPerdivision)
			{
				for (int i = 14; i < 20; i++)
				{
					if ((thePlatform.GetBBOX().Center.x >= i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						platformsMoving->at(i).push_back(thePlatform);
						break;
					}
				}
			}

			else
			{
				for (int i = 9; i < 14; i++)
				{
					if ((thePlatform.GetBBOX().Center.x >= i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						platformsMoving->at(i).push_back(thePlatform);
						break;
					}
				}
			}
		}

		else
		{
			if (thePlatform.GetBBOX().Center.x > 4 * pixelsPerdivision)
			{
				for (int i = 4; i < 9; i++)
				{
					if ((thePlatform.GetBBOX().Center.x >= i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						platformsMoving->at(i).push_back(thePlatform);
						break;
					}
				}
			}

			else
			{
				for (int i = 0; i < 4; i++)
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

}


void BinaryTree::addDeadlyMovingPlatform(Platform thePlatform)
{
	if (thePlatform.GetBBOX().Center.x < 9 * 0)
	{
		deadlyMoving->at(0).push_back(thePlatform);
	}

	else
	{
		if (thePlatform.GetBBOX().Center.x > 9 * pixelsPerdivision)
		{
			if (thePlatform.GetBBOX().Center.x > 14 * pixelsPerdivision)
			{
				for (int i = 14; i < 20; i++)
				{
					if ((thePlatform.GetBBOX().Center.x >= i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						deadlyMoving->at(i).push_back(thePlatform);
						break;
					}
				}
			}

			else
			{
				for (int i = 9; i < 14; i++)
				{
					if ((thePlatform.GetBBOX().Center.x >= i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						deadlyMoving->at(i).push_back(thePlatform);
						break;
					}
				}
			}
		}

		else
		{
			if (thePlatform.GetBBOX().Center.x > 4 * pixelsPerdivision)
			{
				for (int i = 4; i < 9; i++)
				{
					if ((thePlatform.GetBBOX().Center.x >= i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						deadlyMoving->at(i).push_back(thePlatform);
						break;
					}
				}
			}

			else
			{
				for (int i = 0; i < 4; i++)
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

}

void BinaryTree::addDeadlyPlatforms(Platform thePlatform)
{
	if (thePlatform.GetBBOX().Center.x < 9 * 0)
	{
		deadly->at(0).push_back(thePlatform);
	}

	else
	{
		if (thePlatform.GetBBOX().Center.x > 9 * pixelsPerdivision)
		{
			if (thePlatform.GetBBOX().Center.x > 14 * pixelsPerdivision)
			{
				for (int i = 14; i < 20; i++)
				{
					if ((thePlatform.GetBBOX().Center.x >= i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						deadly->at(i).push_back(thePlatform);
						break;
					}
				}
			}

			else
			{
				for (int i = 9; i < 14; i++)
				{
					if ((thePlatform.GetBBOX().Center.x >= i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						deadly->at(i).push_back(thePlatform);
						break;
					}
				}
			}
		}

		else
		{
			if (thePlatform.GetBBOX().Center.x > 4 * pixelsPerdivision)
			{
				for (int i = 4; i < 9; i++)
				{
					if ((thePlatform.GetBBOX().Center.x >= i*pixelsPerdivision) && (thePlatform.GetBBOX().Center.x < (i*pixelsPerdivision) + pixelsPerdivision))
					{
						deadly->at(i).push_back(thePlatform);
						break;
					}
				}
			}

			else
			{
				for (int i = 0; i < 4; i++)
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

}