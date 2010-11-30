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
	Ogre::SceneManager* mSceneMgr;
	short mSceneMatrix[7][9];
};

#endif // SceneLoader_h__