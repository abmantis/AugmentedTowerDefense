#include "StdAfx.h"
#include "ScoresManager.h"

template<> ScoresManager* Ogre::Singleton<ScoresManager>::ms_Singleton = 0;
ScoresManager::ScoresManager()
{
	mPlayerEnergy = 5;
	mTowerPrice = 5;
	mPoints = 10;
	mTowerLevel = 1;
	mLevel = 1;
}

ScoresManager::~ScoresManager(void)
{
}

ScoresManager& ScoresManager::getSingleton( void )
{
	assert( ms_Singleton ); return ( *ms_Singleton );
}

ScoresManager* ScoresManager::getSingletonPtr( void )
{
	return ms_Singleton;
}


