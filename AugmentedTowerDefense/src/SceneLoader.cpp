#include "StdAfx.h"
#include "SceneLoader.h"
#include "DebugStuff.h"

SceneLoader::SceneLoader(Ogre::SceneManager* sceneMgr)
{
	mSceneMgr = sceneMgr;
	mSceneRootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("SceneRootNode");
	mSceneRootNode->setPosition(0,0,0);

	mForceHide = false;

	mWallCount = 0;
	mWallCubeScale = 15;

	mRows = 7;
	mCols = 9;

	short tmpMatrix[7][9] = {1,1,0,1,1,1,1,1,1, 
							1,0,0,1,0,0,0,0,1,
							1,0,1,1,0,1,1,0,1,
							1,0,0,1,0,1,0,0,1,
							1,1,0,1,0,1,0,1,1,
							1,1,0,0,0,1,0,1,1,
							1,1,1,1,1,1,0,1,1};

	for(int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mCols; j++)
		{
			mSceneMatrix[i][j] = tmpMatrix[i][j];
		}
	}
}

SceneLoader::~SceneLoader(void)
{
}

bool SceneLoader::init()
{
	Ogre::Vector2 topLeftPos (mWallCubeScale*mRows*-0.5f, mWallCubeScale*mCols*-0.5f);
	topLeftPos.x += mWallCubeScale*0.5f;
	topLeftPos.y += mWallCubeScale*0.5f;

	// Create all walls based on the matrix
	Ogre::Vector3 vecPos(topLeftPos.x, topLeftPos.y, mWallCubeScale*0.5f);
	for(int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mCols; j++)
		{
			if(mSceneMatrix[i][j] == 1)
			{
				createWall(vecPos, mWallCubeScale);
			}	
			vecPos.y += mWallCubeScale;
		}
		vecPos.x += mWallCubeScale;
		vecPos.y = topLeftPos.y;
	}

	createFloor(mRows*mWallCubeScale, mCols*mWallCubeScale);

	mSceneRootNode->roll(Ogre::Degree(-90));
	return true;
}

void SceneLoader::createWall( Ogre::Vector3 pos, Ogre::Real scale )
{
//	Ogre::Entity* ent = mSceneMgr->createEntity("atd_cube.mesh");
 	Ogre::Entity* ent = mSceneMgr->createEntity("UnitCubeMesh.mesh");
	ent->getMesh()->buildEdgeList();
 	ent->setMaterialName("1_Cladding_Stucco_White");
	ent->setCastShadows(true);
	Ogre::SceneNode* node = mSceneRootNode->createChildSceneNode("Wall_" + HelperClass::ToString(mWallCount));
	node->setPosition(pos);
	node->setScale(Ogre::Vector3::UNIT_SCALE*scale);
//	node->pitch(Ogre::Degree(90));
	node->attachObject(ent);
	
	mWallCount++;
}

void SceneLoader::createFloor( Ogre::Real width, Ogre::Real height )
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Z, 0);

	Ogre::MeshManager::getSingleton().createPlane("FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, width, height, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);

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