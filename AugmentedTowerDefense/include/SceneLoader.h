#ifndef SceneLoader_h__
#define SceneLoader_h__
#pragma once

class SceneLoader
{
public:
	SceneLoader(Ogre::SceneManager* sceneMgr);
	~SceneLoader(void);

	bool init();

private:
	void createWall(Ogre::Vector3 pos, Ogre::Real scale);

	Ogre::SceneManager* mSceneMgr;
	Ogre::SceneNode* mSceneRootNode;
	short mSceneMatrix[7][9];
	int mRows;
	int mCols;
	int mWallCount;
	Ogre::Real mWallCubeScale;
};

#endif // SceneLoader_h__