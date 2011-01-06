#include "StdAfx.h"
#include "SceneLoader.h"
#include "SharedStuff.h"

SceneLoader::SceneLoader(Ogre::SceneManager* sceneMgr, Ogre::SceneNode *baseSceneNode)
{
	mSceneMgr = sceneMgr;
	mSceneRootNode = baseSceneNode->createChildSceneNode("SceneRootNode");
	mSceneRootNode->setPosition(0,0,0);
	mSceneRootNode->setDirection(0,0,-1);

	mForceHide = false;

	mWallCount = 0;
	mWallCubeScale = 15;

	mMatrixSize.x = 9;
	mMatrixSize.y= 7;

 	short tmpMatrix[9][7] = {	1, 1, 1, 1, 1, 1, 1,
 								1, 0, 0, 0, 1, 1, 1,
 								1, 0, 1, 0, 0, 0, 0,
 								1, 0, 1, 1, 1, 1, 1,
 								1, 0, 0, 0, 0, 0, 1,
 								1, 1, 1, 1, 1, 0, 1,
 								0, 0, 1, 0, 0, 0, 1,
 								1, 0, 0, 0, 1, 1, 1,
 								1, 1, 1, 1, 1, 1, 1};
 								
	//short tmpMatrix[9][7] = {	1, 1, 1, 1, 1, 1, 1,
	//							1, 0, 0, 0, 0, 0, 1,
	//							1, 0, 1, 1, 1, 0, 1,
	//							1, 0, 1, 0, 1, 0, 1,
	//							1, 0, 1, 0, 1, 0, 1,
	//							1, 0, 0, 0, 1, 0, 1,
	//							1, 1, 1, 1, 1, 0, 1,
	//							0, 0, 0, 0, 0, 0, 1,
	//							1, 1, 1, 1, 1, 1, 1};

	mStartPos.x = 6; mStartPos.y = 0;

	for(int i = 0; i < mMatrixSize.x; i++)
	{
		for (int j = 0; j < mMatrixSize.y; j++)
		{
			mSceneMatrix[i][j] = tmpMatrix[i][j];
		}
	}

	mTopLeft = getTopLeft();

// 	for(int i = 0; i < mMatrixSize.x; i++)
// 	{
// 		for (int j = 0; j < mMatrixSize.y; j++)
// 		{
// 			std::cout << mSceneMatrix[i][j] << ", ";
// 		}
// 		std::cout << std::endl;
// 	}
}

SceneLoader::~SceneLoader(void)
{
}

bool SceneLoader::init()
{
	// Create all walls based on the matrix
	Ogre::Vector3 vecPos;
	for(int i = 0; i < mMatrixSize.x; i++)
	{
		for (int j = 0; j < mMatrixSize.y; j++)
		{
			if(mSceneMatrix[i][j] == 1)
			{
				vecPos = matrixCoordToSceneCoord(Ogre::Vector2(i*1.0f,j*1.0f));
				createWall(vecPos, mWallCubeScale);
			}	
		}
	}

	createFloor(mMatrixSize.x*mWallCubeScale, mMatrixSize.y*mWallCubeScale);

	createWalkArray();

//	mSceneRootNode->roll(Ogre::Degree(-90));

	return true;
}

void SceneLoader::createWall( Ogre::Vector3 pos, Ogre::Real scale )
{
	Ogre::Entity* ent = mSceneMgr->createEntity("atd_cube.mesh");
//  	Ogre::MeshPtr mesh = ent->getMesh();
//  	mesh->freeEdgeList();
//  	mesh->buildEdgeList();

	ent->setCastShadows(true);
	ent->setQueryFlags(AugmentedTowerDefense::MASK_WALLS);
	Ogre::SceneNode* node = mSceneRootNode->createChildSceneNode("Wall_" + AugmentedTowerDefense::HelperClass::ToString(mWallCount));
	node->setPosition(pos);
	node->setScale(Ogre::Vector3::UNIT_SCALE*scale);
	node->attachObject(ent);
	
	mWallCount++;
}

void SceneLoader::createFloor( Ogre::Real width, Ogre::Real height )
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Z, 0);

	Ogre::MeshManager::getSingleton().createPlane("FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, width, height, 20, 20, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);

	Ogre::Entity* entFloor = mSceneMgr->createEntity("FloorEntity", "FloorPlane");
	entFloor->setMaterialName("Examples/GrassFloor");
	entFloor->setCastShadows(false);

	mSceneRootNode->attachObject(entFloor);

// 	Ogre::Entity* ent = mSceneMgr->createEntity("atd_cube.mesh");
// 	Ogre::SceneNode* node = mSceneRootNode->createChildSceneNode("Floor");
// 	node->setScale(Ogre::Vector3::UNIT_SCALE*mWallCubeScale*Ogre::Vector3(mRows,mCols,1));
// 	node->attachObject(ent);
}

void SceneLoader::togleVisibility()
{	
	mForceHide = !mForceHide;
	if(mForceHide)
	{
		mSceneRootNode->setVisible(false);
	}
}

void SceneLoader::createWalkArray()
{	
	Ogre::Real zPos = 0; //mWallCubeScale*0.5f;
	Ogre::Vector2 curPos, lastPos, nextPos;
	lastPos = curPos = mStartPos;

	mWalkVector.push_back(matrixCoordToSceneCoord(curPos, zPos));
	while(findNextPos(curPos, lastPos, nextPos))
	{
		lastPos = curPos;
		curPos = nextPos;
		mWalkVector.push_back(matrixCoordToSceneCoord(curPos, zPos));
	}
}

bool SceneLoader::findNextPos( Ogre::Vector2 curPos, Ogre::Vector2 lastPos, Ogre::Vector2 &nextPos )
{
	nextPos = curPos;
	nextPos.x -= 1;			// -1  0
	if(canBeNextPos(nextPos, lastPos)) return true;
	nextPos = curPos;
	nextPos.x += 1;			//  1  0
	if(canBeNextPos(nextPos, lastPos)) return true;
	nextPos = curPos;
	nextPos.y -= 1;			//  0 -1
	if(canBeNextPos(nextPos, lastPos)) return true;
	nextPos = curPos;
	nextPos.y += 1;			//  0  1
	if(canBeNextPos(nextPos, lastPos)) return true;
	
 	
// 	std::cout << "-------------------------------" << std::endl;
// 
// 	nextPos = curPos;
// 	nextPos.x -= 1;			// -1  0
// 	std::cout << nextPos.x << " " << nextPos.y << std::endl;
// 	nextPos.y -= 1;			// -1 -1
// 	std::cout << nextPos.x << " " << nextPos.y << std::endl;
// 	nextPos.x += 1;			//  0 -1
// 	std::cout << nextPos.x << " " << nextPos.y << std::endl;
// 	nextPos.x += 1;			//  1 -1
// 	std::cout << nextPos.x << " " << nextPos.y << std::endl;
// 	nextPos.y += 1;			//  1  0
// 	std::cout << nextPos.x << " " << nextPos.y << std::endl;
// 	nextPos.y += 1;			//  1  1
// 	std::cout << nextPos.x << " " << nextPos.y << std::endl;
// 	nextPos.x -= 1;			//  0  1
// 	std::cout << nextPos.x << " " << nextPos.y << std::endl;
// 	nextPos.x -= 1;			//  -1  1
// 	std::cout << nextPos.x << " " << nextPos.y << std::endl;
// 
// 	std::cout << "-------------------------------" << std::endl;


	return false;
}


bool SceneLoader::canBeNextPos( Ogre::Vector2 nextPos, Ogre::Vector2 lastPos )
{
	if(nextPos != lastPos && isPosFloor(nextPos)) return true;
	return false;
}

bool SceneLoader::isPosValid( Ogre::Vector2 pos )
{
	if(pos.x < 0 || pos.y < 0) return false;
	if(pos.x >= mMatrixSize.x || pos.y >= mMatrixSize.y) return false;

	return true;
}

bool SceneLoader::isPosWall( Ogre::Vector2 pos )
{
	if(isPosValid(pos))
	{
		if(mSceneMatrix[(int)pos.x][(int)pos.y] == 1) return true;
	}
	return false;
}

bool SceneLoader::isPosFloor( Ogre::Vector2 pos )
{
	if(isPosValid(pos))
	{
		if(mSceneMatrix[(int)pos.x][(int)pos.y] == 0) return true;
	}
	return false;
}

Ogre::Vector3 SceneLoader::getTopLeft()
{
	Ogre::Vector3 topleft (mWallCubeScale*(mMatrixSize.x+1)*-0.5f, mWallCubeScale*(mMatrixSize.y+1)*-0.5f, mWallCubeScale*0.5f);
	return topleft;
}

Ogre::Vector3 SceneLoader::matrixCoordToSceneCoord( Ogre::Vector2 matrixCoord )
{
	Ogre::Vector3 scenecoord = mTopLeft;
	matrixCoord.x += 1;
	matrixCoord.y += 1;
	matrixCoord *= mWallCubeScale;

	scenecoord.x += matrixCoord.x;
	scenecoord.y += matrixCoord.y;
	
	return scenecoord;
}

Ogre::Vector3 SceneLoader::matrixCoordToSceneCoord( Ogre::Vector2 matrixCoord, Ogre::Real zPos )
{
	Ogre::Vector3 vec = matrixCoordToSceneCoord(matrixCoord);
	vec.z = zPos;
	return vec;
}
