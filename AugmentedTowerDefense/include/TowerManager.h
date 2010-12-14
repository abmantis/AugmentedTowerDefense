#ifndef TowerManager_h__
#define TowerManager_h__

class Tower
{
public:
	Tower(Ogre::SceneManager *sceneMgr, Ogre::Vector3 posi);
	~Tower();

	void update(Ogre::Real deltaTime, std::vector<Ogre::Vector3>* enemyPos);
private:

	Ogre::SceneManager *mSceneMgr;
	Ogre::SceneNode *mBodyNode;
	Ogre::SceneNode *mHeadNode;
	Ogre::SceneNode *mGunsNode;
	Ogre::Entity *mBodyEntity;
	Ogre::Entity *mHeadEntity;
	Ogre::Entity *mGunsEntity;
};

class TowerManager
{
public:
	TowerManager(Ogre::SceneManager *sceneMgr);
	~TowerManager(void);

	void init();
	void update(Ogre::Real deltaTime, std::vector<Ogre::Vector3> *enemyPos);
private:

	Tower *t;
	Ogre::SceneManager *mSceneMgr;
};
#endif // TowerManager_h__