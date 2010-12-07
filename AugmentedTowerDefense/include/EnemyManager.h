#ifndef EnemyManager_h__
#define EnemyManager_h__

#pragma once

class Enemy
{
public:
	Enemy(Ogre::SceneManager *sceneMgr, std::vector<Ogre::Vector3> *walkPath);
	void update(Ogre::Real deltaTime);
private:
	bool nextLocation(void);

	std::vector<Ogre::Vector3> *mWalkPath;
	int mWalkToPos;
	Ogre::SceneNode *mNode;
	Ogre::Real mSpeed;
	Ogre::Real mDistance;                  // The distance the object has left to travel
	Ogre::Vector3 mDirection;              // The direction the object is moving
	Ogre::Vector3 mDestination;            // The destination the object is moving towards
};

class EnemyManager
{
public:
	EnemyManager(Ogre::SceneManager *sceneMgr);
	~EnemyManager(void);

	void init(std::vector<Ogre::Vector3> walkPath);
	void update(Ogre::Real deltaTime);

private:
	Ogre::SceneManager *mSceneMgr;
	std::vector<Ogre::Vector3> mWalkPath;
	Ogre::Real mTimeSinceLastWave;

	Enemy *pEnemy;

};
#endif // EnemyManager_h__
