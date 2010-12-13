#include "StdAfx.h"
#include "EnemyManager.h"
#include "HelperClass.h"

Enemy::Enemy( Ogre::SceneManager *sceneMgr, std::vector<Ogre::Vector3> *walkPath )
:mSceneMgr(sceneMgr), mWalkPath(walkPath)
{
	mWalkToPos = 0;
	mSpeed = 25;
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
}

Enemy::~Enemy()
{
	HelperClass::DestroyAllAttachedMovableObjects(mNode);
	mNode->removeAndDestroyAllChildren();
	mSceneMgr->destroySceneNode(mNode);
}

void Enemy::update( Ogre::Real deltaTime )
{
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



EnemyManager::EnemyManager( Ogre::SceneManager *sceneMgr )
{
	mSceneMgr = sceneMgr;
	mTimeSinceLastWave = 0;	
	mTimeSinceLastEnemyBorn = 0;
	mVisible = false;
	mEnemiesBorn = 0;
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::init( std::vector<Ogre::Vector3> walkPath )
{
	mWalkPath = walkPath;
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
		if(mTimeSinceLastEnemyBorn > 1.0f && mEnemiesBorn < 5)
		{
			createEnemy();
		}		
	}	

}

void EnemyManager::createEnemy()
{
	mEnemyArray.push_back(new Enemy(mSceneMgr, &mWalkPath));
	mEnemiesBorn++;
	mTimeSinceLastEnemyBorn = 0;
}

void EnemyManager::setVisible( bool visible )
{
	mVisible = visible;
	std::list<Enemy*>::iterator it;
	for(it=mEnemyArray.begin(); it != mEnemyArray.end(); it++)
	{
		(*it)->setVisible(visible);
	}
}


