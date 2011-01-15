#ifndef TowerManager_h__
#define TowerManager_h__
#include "EnemyManager.h"
#include "CollisionTools.h"

class Tower
{	
public:
	Tower(Ogre::SceneManager *sceneMgr, Ogre::SceneNode *sceneRootNode, Ogre::Vector3 posi, Ogre::Real shotRateTime);
	~Tower();

	int update(Ogre::Real deltaTime, std::vector<Enemy::IDPosPair> *enemyIDPos);
	void setVisible(bool visible);
	Ogre::Vector3 getPosition() { return mBodyNode->getPosition(); }
	void setPosition(Ogre::Vector3 pos) { mBodyNode->setPosition(pos); }
	void setOrientation(Ogre::Quaternion ori) { mBodyNode->setOrientation(ori); }
private:

	class Shot;
	Ogre::SceneManager *mSceneMgr;
	Ogre::SceneNode *mSceneRootNode;
	Ogre::SceneNode *mBodyNode;
	Ogre::SceneNode *mHeadNode;
	Ogre::SceneNode *mGunsNode;
	Ogre::Entity *mBodyEntity;
	Ogre::Entity *mHeadEntity;
	Ogre::Entity *mGunsEntity;
	Ogre::Real mMaxShootSquaredDistance;
	Ogre::Real mTimeSinceLastShot;
	Ogre::Real mShotTimeInterval;
	bool mLastShotFromLeft;
	Ogre::Vector3 mLeftGunWorldPos;
	Ogre::Vector3 mRightGunWorldPos;

	Shot* mShot;

	class Shot
	{
	public:
		Shot(Ogre::SceneManager* sceneMgr, Ogre::SceneNode *sceneRootNode, Ogre::Vector3 startPos, Ogre::Vector3 enemyPos, int enemyID);
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
	TowerManager(Ogre::SceneManager *sceneMgr, MOC::CollisionTools* colisionTools, Ogre::SceneNode *sceneRootNode);
	~TowerManager(void);

	void init();
	std::vector<int> update(Ogre::Real deltaTime, std::vector<Enemy::IDPosPair> *enemyIDPos);
	bool addTowerToWall(Ogre::Entity *pWall, bool checkOnly);
	bool addTower(Ogre::Vector3 pos);
	void setVisible(bool visible);
	void show() { setVisible(true); }
	void hide() { setVisible(false); }
	
	Tower *mPlacementTower;

private:

	Ogre::SceneManager *mSceneMgr;
	Ogre::SceneNode *mSceneRootNode;
	ScoresManager* mScoresMgr;
	MOC::CollisionTools* mColisionTools;
	
	std::vector<Tower*> mTowerVec;
	bool mVisible;
};
#endif // TowerManager_h__