#ifndef EnemyManager_h__
#define EnemyManager_h__

#pragma once

class Enemy
{
public:
	enum EnemyState { BORNING, ALIVE, DEFEATED, VICTORIOUS };
public:
	Enemy(Ogre::SceneManager *sceneMgr, std::vector<Ogre::Vector3> *walkPath);
	~Enemy();
	void update(Ogre::Real deltaTime);
	void setVisible(bool visible) { mEntity->setVisible(visible); }
	EnemyState getState() { return mState; }
private:
	bool nextLocation(void);

	std::vector<Ogre::Vector3> *mWalkPath;
	int mWalkToPos;
	Ogre::SceneManager *mSceneMgr;
	Ogre::SceneNode *mNode;
	Ogre::Entity *mEntity;
	Ogre::Real mSpeed;
	Ogre::Real mDistance;                  // The distance the object has left to travel
	Ogre::Vector3 mDirection;              // The direction the object is moving
	Ogre::Vector3 mDestination;            // The destination the object is moving towards
	EnemyState mState;
};

class EnemyManager
{
public:
	EnemyManager(Ogre::SceneManager *sceneMgr);
	~EnemyManager(void);

	void init(std::vector<Ogre::Vector3> walkPath);
	void update(Ogre::Real deltaTime);
	void setVisible(bool visible);

private:
	void createEnemy();

	typedef std::list<Enemy*> EnemyArray;

	Ogre::SceneManager *mSceneMgr;
	std::vector<Ogre::Vector3> mWalkPath;
	EnemyArray mEnemyArray;
	Ogre::Real mTimeSinceLastWave;
	Ogre::Real mTimeSinceLastEnemyBorn;
	bool mVisible;
	int mEnemiesBorn;

};
#endif // EnemyManager_h__
