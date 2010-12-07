#include "StdAfx.h"
#include "EnemyManager.h"

Enemy::Enemy( Ogre::SceneManager *sceneMgr, std::vector<Ogre::Vector3> *walkPath )
{
	mWalkPath = walkPath;
	mWalkToPos = 0;
	mSpeed = 15;

	Ogre::Entity *ent = sceneMgr->createEntity("atd_cube.mesh");
	mNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
	mNode->attachObject(ent);
	mNode->setScale(6,6,6);
	mNode->setPosition((*mWalkPath)[0]);
//	mNode->setDirection((*mWalkPath)[1], Ogre::SceneNode::TS_LOCAL, Ogre::Vector3::UNIT_Y);
	nextLocation();
}

void Enemy::update( Ogre::Real deltaTime )
{
	Ogre::Real move = mSpeed * deltaTime;
	mDistance -= move;

	if (mDistance <= 0.0f)
	{                 
		mNode->setPosition(mDestination);
		mDirection = Ogre::Vector3::ZERO;				
		// Set animation based on if the robot has another point to walk to. 
		if (!nextLocation())
		{
// 			// Set Idle animation                     
// 			mAnimationState = mEntity->getAnimationState("Idle");
// 			mAnimationState->setLoop(true);
// 			mAnimationState->setEnabled(true);
		}
		else
		{
			Ogre::Vector3 src = mNode->getOrientation() * Ogre::Vector3::UNIT_X;
			if ((1.0f + src.dotProduct(mDirection)) < 0.0001f) 
			{
				mNode->yaw(Ogre::Degree(180));						
			}
			else
			{
				Ogre::Quaternion quat = src.getRotationTo(mDirection);
				mNode->rotate(quat);
			}
		}
	}
	else
	{
		mNode->translate(mDirection * move);
	} 

// 
// 	mNode->setDirection(vecToGo, Ogre::SceneNode::TS_WORLD, Ogre::Vector3::UNIT_Y);
// 	mNode->translate(Ogre::Vector3(0, mSpeed*deltaTime, 0), Ogre::SceneNode::TS_LOCAL);
	
}

bool Enemy::nextLocation(void){
	if (mWalkPath->size() == mWalkToPos) return false;

	mDestination = (*mWalkPath)[mWalkToPos];
	mDirection = mDestination - mNode->getPosition();
	mDistance = mDirection.normalise();

	mWalkToPos++;

	return true;
}


EnemyManager::EnemyManager( Ogre::SceneManager *sceneMgr )
{
	mSceneMgr = sceneMgr;
	mTimeSinceLastWave = 0;	
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::init( std::vector<Ogre::Vector3> walkPath )
{
	mWalkPath = walkPath;
	pEnemy = NULL;
}

void EnemyManager::update( Ogre::Real deltaTime )
{
	if(mTimeSinceLastWave < 3.0f)
	{
		mTimeSinceLastWave += deltaTime;
	}
	else
	{
		if(pEnemy)
		{
			pEnemy->update(deltaTime);
		}
		else
		{
			pEnemy = new Enemy(mSceneMgr, &mWalkPath);
		}
	}	

}

