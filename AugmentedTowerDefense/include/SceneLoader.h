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

	std::vector<Ogre::Vector3> getWalkVector() const { return mWalkVector; }	

private:
	void createWall(Ogre::Vector3 pos, Ogre::Real scale);
	void createFloor(Ogre::Real width, Ogre::Real height);
	Ogre::Vector3 getTopLeft();
	void createWalkArray();
	bool findNextPos(Ogre::Vector2 curPos, Ogre::Vector2 lastPos, Ogre::Vector2 &nextPos);
	bool canBeNextPos(Ogre::Vector2 nextPos, Ogre::Vector2 lastPos);
	bool isPosValid(Ogre::Vector2 pos);
	bool isPosWall(Ogre::Vector2 pos);
	bool isPosFloor(Ogre::Vector2 pos);
	Ogre::Vector3 matrixCoordToSceneCoord(Ogre::Vector2 matrixCoord);
	Ogre::Vector3 matrixCoordToSceneCoord(Ogre::Vector2 matrixCoord, Ogre::Real zPos);

	Ogre::SceneManager* mSceneMgr;
	Ogre::SceneNode* mSceneRootNode;
	short mSceneMatrix[9][7];
	Ogre::Vector2 mMatrixSize;
	Ogre::Vector2 mStartPos;
	Ogre::Vector3 mTopLeft;
	Ogre::Real mWallCubeScale;
	int mWallCount;
	bool mForceHide;
	std::vector<Ogre::Vector3> mWalkVector;
	
};

#endif // SceneLoader_h__