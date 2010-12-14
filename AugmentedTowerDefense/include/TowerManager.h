#ifndef TowerManager_h__
#define TowerManager_h__

class Tower
{
public:
	Tower(Ogre::SceneManager *sceneMgr, Ogre::Vector3 posi);
	~Tower();

	void update(Ogre::Real deltaTime, std::vector<Ogre::Vector3>* enemyPosVec);
private:

	Ogre::SceneManager *mSceneMgr;
	Ogre::SceneNode *mBodyNode;
	Ogre::SceneNode *mHeadNode;
	Ogre::SceneNode *mGunsNode;
	Ogre::Entity *mBodyEntity;
	Ogre::Entity *mHeadEntity;
	Ogre::Entity *mGunsEntity;
	Ogre::Real mMaxShootSquaredDistance;
};

class TowerManager
{
public:
	TowerManager(Ogre::SceneManager *sceneMgr);
	~TowerManager(void);

	void init();
	void update(Ogre::Real deltaTime, std::vector<Ogre::Vector3> *enemyPos);
	void addTower(Ogre::Vector3 pos);
private:

// 	Tower *t;
// 	Tower *t2;
	Ogre::SceneManager *mSceneMgr;
	std::vector<Tower*> mTowerVec;
};
#endif // TowerManager_h__