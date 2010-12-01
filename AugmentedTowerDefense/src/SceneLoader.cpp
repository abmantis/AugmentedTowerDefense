#include "StdAfx.h"
#include "SceneLoader.h"
#include "DebugStuff.h"

SceneLoader::SceneLoader(Ogre::SceneManager* sceneMgr)
{
	mSceneMgr = sceneMgr;
	mSceneRootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("SceneRootNode");
	mSceneRootNode->setPosition(0,0,0);

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
	Ogre::Vector3 vecPos(topLeftPos.x, topLeftPos.y, 0);
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

	mSceneRootNode->roll(Ogre::Degree(-90));
	return true;
}

void SceneLoader::createWall( Ogre::Vector3 pos, Ogre::Real scale )
{
	Ogre::Entity* ent = mSceneMgr->createEntity("atd_cube.mesh");
	HelperClass::Print(ent->getBoundingBox().getSize());
	Ogre::SceneNode* node = mSceneRootNode->createChildSceneNode("Wall_" + HelperClass::ToString(mWallCount));
	node->setPosition(pos);
	node->setScale(Ogre::Vector3::UNIT_SCALE*scale);
	node->attachObject(ent);
	
	mWallCount++;
}
