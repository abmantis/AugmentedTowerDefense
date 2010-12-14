#include "StdAfx.h"
#include "TowerManager.h"
#include "HelperClass.h"


//////////////////////////////////////////////////////////////////////////
// Tower class
//////////////////////////////////////////////////////////////////////////
Tower::Tower(Ogre::SceneManager *sceneMgr, Ogre::Vector3 posi)
: mSceneMgr(sceneMgr)
{
	// Body
	mBodyEntity = mSceneMgr->createEntity("ATD_tower_body.mesh");
	mBodyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mBodyNode->attachObject(mBodyEntity);

	// Head
	mHeadEntity = mSceneMgr->createEntity("ATD_tower_head.mesh");
	mHeadNode = mBodyNode->createChildSceneNode();
	mHeadNode->attachObject(mHeadEntity);

	// Guns
	// Find head center to set guns center
	Ogre::Vector3 headcenter = mHeadEntity->getBoundingBox().getCenter();
	mGunsEntity = mSceneMgr->createEntity("ATD_tower_guns.mesh");
	mGunsNode = mHeadNode->createChildSceneNode();
	mGunsNode->attachObject(mGunsEntity);
	mGunsNode->setPosition(headcenter);

	mBodyNode->setPosition(posi);
	mBodyNode->setScale(3,3,3);

// 	mHeadNode->roll(Ogre::Degree(25));
// 	mGunsNode->pitch(Ogre::Degree(25));
// 	
}

Tower::~Tower()
{

}

void Tower::update( Ogre::Real deltaTime, std::vector<Ogre::Vector3>* enemyPos )
{
	if(enemyPos->size()>0)
	{
		Ogre::Vector3 enemy = (*enemyPos)[0];
		// The tower is originally facing -Y

		Ogre::Vector3 dest = mHeadNode->getPosition()-enemy;
		Ogre::Vector3 unityZ = Ogre::Vector3::UNIT_Z;
		mHeadNode->roll(unityZ.angleBetween(dest));

		//mHeadNode->setOrientation(Ogre::Vector3::NEGATIVE_UNIT_Y.getRotationTo(enemy));

	}
}

//////////////////////////////////////////////////////////////////////////
// TowerManager class
//////////////////////////////////////////////////////////////////////////
TowerManager::TowerManager( Ogre::SceneManager *sceneMgr )
: mSceneMgr(sceneMgr)
{
	t = new Tower(mSceneMgr, Ogre::Vector3(9, 0, 15));
}

void TowerManager::init()
{

}

void TowerManager::update( Ogre::Real deltaTime, std::vector<Ogre::Vector3> *enemyPos )
{
	t->update(deltaTime, enemyPos);
}

TowerManager::~TowerManager( void )
{

}
