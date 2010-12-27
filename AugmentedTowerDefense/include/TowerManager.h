#ifndef TowerManager_h__
#define TowerManager_h__

class Tower
{	
public:
	Tower(Ogre::SceneManager *sceneMgr, Ogre::Vector3 posi);
	~Tower();

	void update(Ogre::Real deltaTime, std::vector<Ogre::Vector3>* enemyPosVec);
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
		Shot(Ogre::SceneManager* sceneMgr, Ogre::Vector3 startPos, Ogre::Vector3 enemyPos);
		~Shot();
		bool update(Ogre::Real deltaTime);
	private:

		Ogre::SceneManager* mSceneMgr;
		Ogre::Entity *mEntity;
		Ogre::SceneNode *mNode;
		Ogre::Real mSpeed;
		Ogre::Vector3 mDirection;
		Ogre::Real mDistance;
	};
};

class TowerManager
{
public:
	TowerManager(Ogre::SceneManager *sceneMgr);
	~TowerManager(void);

	void init();
	void update(Ogre::Real deltaTime, std::vector<Ogre::Vector3> *enemyPos);
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