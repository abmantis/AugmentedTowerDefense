#ifndef EnemyManager_h__
#define EnemyManager_h__

#pragma once
#include "ScoresManager.h"

class Enemy
{
public:
	typedef std::pair<int, Ogre::Vector3> IDPosPair;
	enum EnemyState { BORNING, ALIVE, DEFEATED, VICTORIOUS };
	enum EnemyType { NORMAL, ENERGY, UPGRADE };
public:
	Enemy(int ID, EnemyType type, int energy, Ogre::SceneManager *sceneMgr, Ogre::SceneNode *sceneRootNode, std::vector<Ogre::Vector3> *walkPath);
	~Enemy();
	void update(Ogre::Real deltaTime);
	void setVisible(bool visible) { mEntity->setVisible(visible); }
	EnemyState getState() { return mState; }
	Ogre::Vector3 getPosition();
	int getID() {return mID;}
	void addShot();
	EnemyType getType() { return mType; }
	Ogre::Real getHeight() { return mHeight; }

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
	Ogre::Real mScale;
	EnemyState mState;
	int mID;
	int mEnergy;
	EnemyType mType;
	Ogre::Real mHeight;
	
};

class EnemyManager
{
public:
	EnemyManager(Ogre::SceneManager *sceneMgr, Ogre::SceneNode *sceneRootNode);
	~EnemyManager(void);

	void init(std::vector<Ogre::Vector3> walkPath);
	void update(Ogre::Real deltaTime);
	void setVisible(bool visible);
	void show() { setVisible(true); }
	void hide() { setVisible(false); }

	std::vector<Enemy::IDPosPair> getEnemyPos();
	void addShotsToEnemies(std::vector<int> enemyIdVec);

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
	int mLastEnemyID;
	int mCurrentEnemyEnergy;
	ScoresManager* mScoresMgr;
	Ogre::SceneNode *mSceneRootNode;

};
#endif // EnemyManager_h__
