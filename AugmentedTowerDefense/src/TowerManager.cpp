#include "StdAfx.h"
#include "TowerManager.h"
#include "SharedStuff.h"

//////////////////////////////////////////////////////////////////////////
// TowerManager class
//////////////////////////////////////////////////////////////////////////
TowerManager::TowerManager( Ogre::SceneManager *sceneMgr, MOC::CollisionTools* colisionTools, Ogre::SceneNode *sceneRootNode )
: mSceneMgr(sceneMgr), mColisionTools(colisionTools), mSceneRootNode(sceneRootNode)
{
 	mVisible = false;
	mScoresMgr = NULL;
	mPlacementTower = NULL;
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
	mScoresMgr = ScoresManager::getSingletonPtr();

	// set mVisible = true so that setVisible will process and change mVisible to false :)
	mVisible = true;
	setVisible(false);
	
	// lets initialize the tower's entities. Otherwise there  
	// will be a delay when the player adds the first tower
	//Ogre::Entity *ent = mSceneMgr->createEntity("ATD_tower_body.mesh");
	//mSceneMgr->destroyEntity(ent);
	//ent = mSceneMgr->createEntity("ATD_tower_head.mesh");
	//mSceneMgr->destroyEntity(ent);
	//ent = mSceneMgr->createEntity("ATD_tower_guns.mesh");
	//mSceneMgr->destroyEntity(ent);

	mPlacementTower = new Tower(mSceneMgr, mSceneMgr->getRootSceneNode(), Ogre::Vector3::ZERO, 0);
	mPlacementTower->setVisible(false);
}

// @param deltaTime: time since last call
// @param enemyIDPos: vector of Enemy::IDPosPair representing the current enemies
// 
// @return: vector with the IDs of shooted enemies
std::vector<int> TowerManager::update( Ogre::Real deltaTime, std::vector<Enemy::IDPosPair> *enemyIDPos )
{
	std::vector<int> shootedEnemies;
	int enemyID;
	for(unsigned int i = 0; i < mTowerVec.size(); i++)
	{
		if((enemyID = mTowerVec[i]->update(deltaTime, enemyIDPos)) > -1)
		{
			shootedEnemies.push_back(enemyID);
		}
	}	

	return shootedEnemies;
}

bool TowerManager::addTower( Ogre::Vector3 pos )
{
	int towerPrice = mScoresMgr->GetTowerPrice();
	if(towerPrice <= mScoresMgr->GetPoints())
	{
		Ogre::Real shotTimeInterval = 2; //initially the towers fire every 2 seconds
		shotTimeInterval -= (mScoresMgr->GetTowerLevel()-1)*0.1f;
		mScoresMgr->ChangePoints(-towerPrice);
		mTowerVec.push_back(new Tower(mSceneMgr, mSceneRootNode, pos, shotTimeInterval));
		return true;
	}
	else
	{
		return false;
	}
}

bool TowerManager::addTowerToWall( Ogre::Entity *pWall, bool checkOnly )
{
	if(pWall->isVisible() == false) return false;
	if(mScoresMgr->GetTowerPrice() > mScoresMgr->GetPoints()) return false;
	
	float closestDist;
	Ogre::Vector3 result;
	Ogre::SceneNode *pWallNode = pWall->getParentSceneNode();
	Ogre::Vector3 wallPos = pWallNode->getPosition();	
	Ogre::Quaternion sceneOri = mSceneRootNode->_getDerivedOrientation();	
	Ogre::Entity *pTowerTarget = NULL;
	Ogre::Vector3 pointAboveWallCenter = mSceneRootNode->getPosition() + sceneOri * Ogre::Vector3(wallPos.x, wallPos.y, 99999);
	Ogre::Vector3 rayDirection = sceneOri * Ogre::Vector3::NEGATIVE_UNIT_Z;

	//////////////////////////////////////////////////////////////////////////
	// Make sure there's not a tower already in this wall 
	if( mColisionTools->raycastFromPoint(
		pointAboveWallCenter, rayDirection, result, pTowerTarget, 
		closestDist, AugmentedTowerDefense::MASK_TOWER) == false)
	{

		wallPos.z += pWall->getBoundingBox().getSize().z * pWallNode->getScale().z * 0.5f;
		if(checkOnly) return true;
		else return addTower(wallPos);
	}	
	return false;
}

void TowerManager::setVisible( bool visible )
{
	if(visible != mVisible)
	{
		mVisible = visible;
		int towerCount = mTowerVec.size();
		for(int i = 0; i< towerCount; i++)
		{
			mTowerVec[i]->setVisible(mVisible);
		}
	}
}



//////////////////////////////////////////////////////////////////////////
// Tower class
//////////////////////////////////////////////////////////////////////////
Tower::Tower(Ogre::SceneManager *sceneMgr, Ogre::SceneNode *sceneRootNode, Ogre::Vector3 posi, Ogre::Real shotTimeInterval)
: mSceneMgr(sceneMgr), mSceneRootNode(sceneRootNode)
{
	mShot = NULL;
	mTimeSinceLastShot = 0;
	mShotTimeInterval = shotTimeInterval;
	Ogre::int32 mask = AugmentedTowerDefense::MASK_TOWER;
	mMaxShootSquaredDistance = 1700;

	// Body
	mBodyEntity = mSceneMgr->createEntity("ATD_tower_body.mesh");
	mBodyEntity->setQueryFlags(mask);
	mBodyNode =sceneRootNode->createChildSceneNode();
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
	Ogre::Vector3 scale(3,3,2);
	mBodyNode->setScale(scale);

	mRightGunWorldPos.x = mGunsEntity->getBoundingBox().getMaximum().x * scale.x;
	mRightGunWorldPos.x -= mRightGunWorldPos.x*0.1f;
	mRightGunWorldPos.y = mGunsEntity->getBoundingBox().getMinimum().y * scale.y;
//	mRightGunWorldPos.y -= mRightGunWorldPos.y*0.1f;
	mRightGunWorldPos.z = 0;	

	mLeftGunWorldPos = mRightGunWorldPos;
	mLeftGunWorldPos.x *= -1;
	
	mLastShotFromLeft = false;
}

Tower::~Tower()
{

}

// @param deltaTime: time since last call
// @param enemyIDPos: vector of Enemy::IDPosPair representing the current enemies
// 
// @return: ID of shooted enemy (-1 if no enemy was shooted)
int Tower::update( Ogre::Real deltaTime, std::vector<Enemy::IDPosPair> *enemyIDAndPos)
{	
	Ogre::Vector3 towerPos = mBodyNode->getPosition();
	int enemyCount = enemyIDAndPos->size();
	Ogre::Real squaredDistance;
	Ogre::Vector3 enemyPos;
	int enemyID;
	bool bHasEnemyToShoot = false;

	mTimeSinceLastShot += deltaTime;


	for(int i = 0; i < enemyCount; i++)
	{
		Enemy::IDPosPair pair = (*enemyIDAndPos)[i];		
		enemyID = pair.first;
		enemyPos = pair.second;
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
		Ogre::Quaternion headDerivedOri = mBodyNode->getOrientation() * mHeadNode->getOrientation();
		Ogre::Vector3 headDerivedPos = mBodyNode->getPosition() + mBodyNode->getOrientation() * (mBodyNode->getScale() * mHeadNode->getPosition());
		Ogre::Vector3 headDerivedScale = mBodyNode->getScale() * mHeadNode->getScale();
		Ogre::Quaternion gunsDerivedOri = headDerivedOri * mGunsNode->getOrientation();
		Ogre::Vector3 gunsDerivedPos = headDerivedPos + headDerivedOri * (headDerivedScale * mGunsNode->getPosition());

		// Rotate head
		Ogre::Vector3 direction = enemyPos - headDerivedPos;
		Ogre::Vector3 src = headDerivedOri * Ogre::Vector3::NEGATIVE_UNIT_Y;
		direction.z = 0;	// Strip out Z component (we only want to rotate arround X & Y)
		src.z = 0;
		direction.normalise();
		src.normalise();
		AugmentedTowerDefense::HelperClass::DoSafeRotation(mHeadNode, src, direction);

		// Tilt gun
		direction = enemyPos - gunsDerivedPos;
		Ogre::Real height = gunsDerivedPos.z - enemyPos.z;
		Ogre::Real distance = direction.length();
		Ogre::Radian pitchAngle = Ogre::Degree(90) - Ogre::Math::ACos(height / distance);
		Ogre::Quaternion quat;
		quat.FromAngleAxis(pitchAngle, Ogre::Vector3::UNIT_X);
		mGunsNode->setOrientation(quat);

		if(mTimeSinceLastShot > mShotTimeInterval)
		{
			mTimeSinceLastShot = 0;
			if(mShot)
			{
				delete mShot;
				mShot = NULL;
			}
			Ogre::Vector3 gunPos;
			mLastShotFromLeft = !mLastShotFromLeft;
			if(mLastShotFromLeft) gunPos = mLeftGunWorldPos;
			else gunPos = mRightGunWorldPos;

			gunPos = gunsDerivedPos + gunsDerivedOri * gunPos;
			mShot = new Shot(mSceneMgr, mSceneRootNode, gunPos, enemyPos, enemyID);
		}
	}

	if(mShot)
	{
		if(mShot->update(deltaTime))
		{
			int enemyID = mShot->getEnemyID();
			delete mShot;
			mShot = NULL;
			return enemyID;
		}
	}
	return -1;
}

void Tower::setVisible( bool visible )
{
	mBodyNode->setVisible(visible);	
}



//////////////////////////////////////////////////////////////////////////
// Tower::Shot class
//////////////////////////////////////////////////////////////////////////
Tower::Shot::Shot(Ogre::SceneManager* sceneMgr, Ogre::SceneNode *sceneRootNode, Ogre::Vector3 startPos, Ogre::Vector3 enemyPos, int enemyID)
{
	mSpeed = 150;
	mSceneMgr = sceneMgr;
	mDirection = enemyPos - startPos;
	mDistance = mDirection.normalise();
	mEnemyID = enemyID;


	mEntity = mSceneMgr->createEntity("Cylinderbullet.mesh");
	mEntity->setCastShadows(false);
	mNode = sceneRootNode->createChildSceneNode(startPos);
	mNode->attachObject(mEntity);
	mNode->setScale(2,2,1);
	mNode->setDirection(mDirection);
	

	mDistance -= mEntity->getBoundingBox().getHalfSize().z;

}

Tower::Shot::~Shot()
{
	AugmentedTowerDefense::HelperClass::DestroyAllAttachedMovableObjects(mNode);
	mNode->removeAndDestroyAllChildren();
	mSceneMgr->destroySceneNode(mNode);
}

bool Tower::Shot::update( Ogre::Real deltaTime )
{
	Ogre::Real move = mSpeed * deltaTime;
	mDistance -= move;

	if(mDistance < 0) return true;

	mNode->translate(mDirection * move);

	return false;
}
