#include "StdAfx.h"
#include "TowerManager.h"
#include "HelperClass.h"


//////////////////////////////////////////////////////////////////////////
// Tower class
//////////////////////////////////////////////////////////////////////////
Tower::Tower(Ogre::SceneManager *sceneMgr, Ogre::Vector3 posi)
: mSceneMgr(sceneMgr)
{
	Ogre::int32 mask = 1<<3;
	mMaxShootSquaredDistance = 40000;

	// Body
	mBodyEntity = mSceneMgr->createEntity("ATD_tower_body.mesh");
	mBodyEntity->setQueryFlags(mask);
	mBodyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mBodyNode->attachObject(mBodyEntity);

	// Head
	mHeadEntity = mSceneMgr->createEntity("ATD_tower_head.mesh");
	mHeadEntity->setQueryFlags(mask);
	mHeadNode = mBodyNode->createChildSceneNode();
	mHeadNode->attachObject(mHeadEntity);

	// Guns
	// Find head center to set guns center
	Ogre::Vector3 headcenter = mHeadEntity->getBoundingBox().getCenter();
	mGunsEntity = mSceneMgr->createEntity("ATD_tower_guns.mesh");
	mGunsEntity->setQueryFlags(mask);
	mGunsNode = mHeadNode->createChildSceneNode(headcenter);
	mGunsNode->attachObject(mGunsEntity);

	mBodyNode->setPosition(posi);
	mBodyNode->setScale(3,3,3);


// 	mHeadNode->roll(Ogre::Degree(25));
// 	mGunsNode->pitch(Ogre::Degree(25));
// 	
}

Tower::~Tower()
{

}

void Tower::update( Ogre::Real deltaTime, std::vector<Ogre::Vector3>* enemyPosVec )
{	
	Ogre::Vector3 towerPos = mBodyNode->getPosition();
	int enemyCount = enemyPosVec->size();
	Ogre::Real squaredDistance;
	Ogre::Vector3 enemyPos;
	bool bHasEnemyToShoot = false;

	for(int i = 0; i < enemyCount; i++)
	{
		enemyPos = (*enemyPosVec)[i];
		squaredDistance = towerPos.squaredDistance(enemyPos);
		if(squaredDistance <= mMaxShootSquaredDistance)
		{			
			bHasEnemyToShoot = true;
			break;
		}		
	}

	if(bHasEnemyToShoot)
	{
		//////////////////////////////////////
		// The tower is originally facing -Y

		// Rotate head
		Ogre::Vector3 direction = enemyPos - mHeadNode->_getDerivedPosition();//getPosition();
		Ogre::Vector3 src = mHeadNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Y;
		direction.z = 0;	// Strip out Z component (we only want to rotate arround X & Y)
		src.z = 0;
 		direction.normalise();
 		src.normalise();
		HelperClass::DoSafeRotation(mHeadNode, src, direction);

		// Tilt gun
		Ogre::Vector3 gunWorldPos = mGunsNode->_getDerivedPosition();
		direction = enemyPos - gunWorldPos;
		Ogre::Real height = gunWorldPos.z - enemyPos.z;
		Ogre::Real distance = direction.length();
		Ogre::Radian pitchAngle = Ogre::Degree(90) - Ogre::Math::ACos(height / distance);
		Ogre::Quaternion quat;
		quat.FromAngleAxis(pitchAngle, Ogre::Vector3::UNIT_X);
		mGunsNode->setOrientation(quat);
	}
}

//////////////////////////////////////////////////////////////////////////
// TowerManager class
//////////////////////////////////////////////////////////////////////////
TowerManager::TowerManager( Ogre::SceneManager *sceneMgr )
: mSceneMgr(sceneMgr)
{
// 	mTowerVec.push_back(new Tower(mSceneMgr, Ogre::Vector3(15, -15, 15)));
// 	mTowerVec.push_back(new Tower(mSceneMgr, Ogre::Vector3(15*3, 15*3, 15)));
}

TowerManager::~TowerManager( void )
{
	for(unsigned int i = 0; i < mTowerVec.size(); i++)
	{
		if(mTowerVec[i]) delete mTowerVec[i];
		mTowerVec[i] = NULL;
	}
}

void TowerManager::init()
{

}

void TowerManager::update( Ogre::Real deltaTime, std::vector<Ogre::Vector3> *enemyPos )
{
	for(unsigned int i = 0; i < mTowerVec.size(); i++)
	{
		mTowerVec[i]->update(deltaTime, enemyPos);
	}
}

void TowerManager::addTower( Ogre::Vector3 pos )
{
	mTowerVec.push_back(new Tower(mSceneMgr, pos));
}

