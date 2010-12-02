#ifndef SceneLoader_h__
#define SceneLoader_h__
#pragma once

class SceneLoader
{
public:
	SceneLoader(Ogre::SceneManager* sceneMgr);
	~SceneLoader(void);

	bool init();

	void show() { if(!mForceHide) mSceneRootNode->setVisible(true); }
	void hide() { mSceneRootNode->setVisible(false); }
	void togleVisibility();

private:
	void createWall(Ogre::Vector3 pos, Ogre::Real scale);
	void createFloor(Ogre::Real width, Ogre::Real height);

	Ogre::SceneManager* mSceneMgr;
	Ogre::SceneNode* mSceneRootNode;
	short mSceneMatrix[7][9];
	int mRows;
	int mCols;
	int mWallCount;
	Ogre::Real mWallCubeScale;
	bool mForceHide;
};

#endif // SceneLoader_h__