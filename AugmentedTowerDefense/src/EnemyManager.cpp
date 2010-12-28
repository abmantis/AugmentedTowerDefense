#include "StdAfx.h"
#include "EnemyManager.h"
#include "SharedStuff.h"

//////////////////////////////////////////////////////////////////////////
// Enemy class
//////////////////////////////////////////////////////////////////////////
Enemy::Enemy(int ID, Ogre::SceneManager *sceneMgr, std::vector<Ogre::Vector3> *walkPath )
:mSceneMgr(sceneMgr), mWalkPath(walkPath)
{
	mID = ID;
	mWalkToPos = 0;
	mSpeed = 10;
	mScale = 6;
	
	mEntity = mSceneMgr->createEntity("atd_cube.mesh");
	mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mNode->attachObject(mEntity);
	mNode->setScale(mScale, mScale, mScale);
	Ogre::Vector3 posi = (*mWalkPath)[0];
	posi.z = mEntity->getBoundingBox().getSize().z * mScale * 0.5f;
	mNode->setPosition(posi);
	nextLocation();
	mState = BORNING;

	mEnergy = 5;
}

Enemy::~Enemy()
{
	AugmentedTowerDefense::HelperClass::DestroyAllAttachedMovableObjects(mNode);
	mNode->removeAndDestroyAllChildren();
	mSceneMgr->destroySceneNode(mNode);
}

void Enemy::update( Ogre::Real deltaTime )
{
	if(mState == DEFEATED) return;

	Ogre::Real move = mSpeed * deltaTime;
	mDistance -= move;

	if (mDistance <= 0.0f)
	{                 
		mNode->setPosition(mDestination);
		mDirection = Ogre::Vector3::ZERO;				

		if (!nextLocation())
		{
			mState = VICTORIOUS;

		}
		else
		{
// 			Ogre::Vector3 src = mNode->getOrientation() * Ogre::Vector3::UNIT_X;
// 			if ((1.0f + src.dotProduct(mDirection)) < 0.0001f) 
// 			{
// 				mNode->yaw(Ogre::Degree(180));
// 			}
// 			else
// 			{
// 				Ogre::Quaternion quat = src.getRotationTo(mDirection);
// 				mNode->rotate(quat);
// 			}
		}
	}
	else
	{
		mNode->translate(mDirection * move);
	} 

//	mNode->yaw(Ogre::Degree(deltaTime * 200));
	mNode->roll(Ogre::Degree(deltaTime * 400));
//	mNode->pitch(Ogre::Degree(deltaTime * 500));
}

bool Enemy::nextLocation(void){
	if (mWalkPath->size() == mWalkToPos) return false;

	mDestination = (*mWalkPath)[mWalkToPos];
	mDestination.z = mEntity->getBoundingBox().getSize().z * mScale * 0.5f;
	mDirection = mDestination - mNode->getPosition();
	mDistance = mDirection.normalise();

	mWalkToPos++;

	return true;
}

Ogre::Vector3 Enemy::getPosition()
{
	return mNode->getPosition();
}

void Enemy::addShot()
{
	mEnergy--; 
	if(mEnergy <= 0) mState = DEFEATED;
}

//////////////////////////////////////////////////////////////////////////
// EnemyManager class
//////////////////////////////////////////////////////////////////////////
EnemyManager::EnemyManager( Ogre::SceneManager *sceneMgr )
: mSceneMgr(sceneMgr)
{	
	mTimeSinceLastWave = 0;	
	mTimeSinceLastEnemyBorn = 0;
	mEnemiesBorn = 0;
	mVisible = false;
	mLastEnemyID = 0;
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::init( std::vector<Ogre::Vector3> walkPath )
{
	mWalkPath = walkPath;

	// set mVisible = true so that setVisible will process and change mVisible to false :)
	mVisible = true;
	setVisible(false);
}

void EnemyManager::update( Ogre::Real deltaTime )
{
	if(mTimeSinceLastWave < 5.0f)
	{
		mTimeSinceLastWave += deltaTime;
	}
	else
	{
		EnemyArray::iterator it;
		EnemyArray::iterator itBegin = mEnemyArray.begin();
		EnemyArray::iterator itEnd = mEnemyArray.end();

		if(itBegin != itEnd)
		{
			// Update and delete enemies
			std::queue<EnemyArray::iterator> itersToDelete;
			for(it = itBegin; it != itEnd; it++)
			{
				Enemy* pEnemy = (*it);
				pEnemy->update(deltaTime);

				switch (pEnemy->getState())
				{
				case Enemy::VICTORIOUS:
				case Enemy::DEFEATED:
					delete pEnemy;
					pEnemy = NULL;
					itersToDelete.push(it);	// add iterator to delete list. it will be deleted later
					break;
				case Enemy::BORNING:
				case Enemy::ALIVE:					
					break;
				}
			}

			// delete enemies
			while(!itersToDelete.empty())
			{
				mEnemyArray.erase(itersToDelete.front());
				itersToDelete.pop();
			}
		}
		else // The array is empty
		{   
			if(mEnemiesBorn >= 5)
			{
				// The array is empty and all enemies have already born? Start next round!
				mTimeSinceLastWave = 0;
				mEnemiesBorn = 0;
				mTimeSinceLastEnemyBorn = 1.1f;
				return;
			}
		}

		mTimeSinceLastEnemyBorn += deltaTime;
		if(mTimeSinceLastEnemyBorn > 2.0f && mEnemiesBorn < 5)
		{
			createEnemy();
		}		
	}	

}

void EnemyManager::createEnemy()
{
	Enemy* pEnemy = new Enemy(mLastEnemyID, mSceneMgr, &mWalkPath);
	pEnemy->setVisible(mVisible);
	mEnemyArray.push_back(pEnemy);
	mEnemiesBorn++;
	mTimeSinceLastEnemyBorn = 0;

	mLastEnemyID++;
	if(mLastEnemyID > 9999) mLastEnemyID = 0;
}

void EnemyManager::setVisible( bool visible )
{
	if(visible != mVisible)
	{
		mVisible = visible;
		std::list<Enemy*>::iterator it;
		for(it=mEnemyArray.begin(); it != mEnemyArray.end(); it++)
		{
			(*it)->setVisible(visible);
		}
	}	
}

std::vector<Enemy::IDPosPair> EnemyManager::getEnemyPos()
{
	std::vector<Enemy::IDPosPair> enemyIDPosVec;

	EnemyArray::iterator it;
	EnemyArray::iterator itBegin = mEnemyArray.begin();
	EnemyArray::iterator itEnd = mEnemyArray.end();

	for(it = itBegin; it != itEnd; it++)
	{
		Enemy* pEnemy = (*it);
		Enemy::IDPosPair pair;
		pair.first = pEnemy->getID();
		pair.second = pEnemy->getPosition();
		enemyIDPosVec.push_back(pair);
	}
	return enemyIDPosVec;
}

void EnemyManager::addShotsToEnemies( std::vector<int> enemyIdVec )
{
	int iShootedEnemies = enemyIdVec.size();
	EnemyArray::iterator it;
	EnemyArray::iterator itBegin = mEnemyArray.begin();
	EnemyArray::iterator itEnd = mEnemyArray.end();

	for (int i = 0; i < iShootedEnemies; i++)
	{
		int iID = enemyIdVec[i];		
		
		for(it = itBegin; it != itEnd; it++)
		{
			Enemy* pEnemy = (*it);
			if(pEnemy->getID() == iID) pEnemy->addShot();
		}
	}
}


