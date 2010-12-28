#ifndef TowerManager_h__
#define TowerManager_h__
#include "EnemyManager.h"

class Tower
{	
public:
	Tower(Ogre::SceneManager *sceneMgr, Ogre::Vector3 posi);
	~Tower();

	int update(Ogre::Real deltaTime, std::vector<Enemy::IDPosPair> *enemyIDPos);
	void setVisible(bool visible);
private:

	class Shot;
	Ogre::SceneManager *mSceneMgr;
	Ogre::SceneNode *mBodyNode;
	Ogre::SceneNode *mHeadNode;
	Ogre::SceneNode *mGunsNode;
	Ogre::Entity *mBodyEntity;
	Ogre::Entity *mHeadEntity;
	Ogre::Entity *mGunsEntity;
	Ogre::Real mMaxShootSquaredDistance;
	Ogre::Real mTimeSinceLastShot;
	Ogre::Real mShotRateTime;
	Shot* mShot;

	class Shot
	{
	public:
		Shot(Ogre::SceneManager* sceneMgr, Ogre::Vector3 startPos, Ogre::Vector3 enemyPos, int enemyID);
		~Shot();
		bool update(Ogre::Real deltaTime);
		int getEnemyID() { return mEnemyID; }
	private:

		Ogre::SceneManager* mSceneMgr;
		Ogre::Entity *mEntity;
		Ogre::SceneNode *mNode;
		Ogre::Real mSpeed;
		Ogre::Vector3 mDirection;
		Ogre::Real mDistance;
		int mEnemyID;
	};
};

class TowerManager
{
public:
	TowerManager(Ogre::SceneManager *sceneMgr);
	~TowerManager(void);

	void init();
	std::vector<int> update(Ogre::Real deltaTime, std::vector<Enemy::IDPosPair> *enemyIDPos);
	void addTower(Ogre::Vector3 pos);
	void setVisible(bool visible);
	void show() { setVisible(true); }
	void hide() { setVisible(false); }
private:

	Ogre::SceneManager *mSceneMgr;
	std::vector<Tower*> mTowerVec;
	bool mVisible;
};
#endif // TowerManager_h__