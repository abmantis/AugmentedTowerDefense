#include "StdAfx.h"
#include "SceneLoader.h"

SceneLoader::SceneLoader(Ogre::SceneManager* sceneMgr)
{
	mSceneMgr = sceneMgr;

	short tmpMatrix[7][9] = {1,1,0,1,1,1,1,1,1, 
							1,0,0,1,0,0,0,0,1,
							1,0,1,1,0,1,1,0,1,
							1,0,0,1,0,1,0,0,1,
							1,1,0,1,0,1,0,1,1,
							1,1,0,0,0,1,0,1,1,
							1,1,1,1,1,1,0,1,1};
	mSceneMatrix = tmpMatrix;
	//mSceneMatrix[0] = {1,1,0,1,1,1,1,1,1}; 
	//mSceneMatrix[1] = {1,0,0,1,0,0,0,0,1}; 
	//mSceneMatrix[2] = {1,0,1,1,0,1,1,0,1}; 
	//mSceneMatrix[3] = {1,0,0,1,0,1,0,0,1}; 
	//mSceneMatrix[4] = {1,1,0,1,0,1,0,1,1}; 
	//mSceneMatrix[5] = {1,1,0,0,0,1,0,1,1}; 
	//mSceneMatrix[6] = {1,1,1,1,1,1,0,1,1};
}

SceneLoader::~SceneLoader(void)
{
}
