#include "StdAfx.h"
#include "ScoresManager.h"

template<> ScoresManager* Ogre::Singleton<ScoresManager>::ms_Singleton = 0;
ScoresManager::ScoresManager(Ogre::RenderTarget *window)
{
	mWindow = window;
	mDebugOverlay = Ogre::OverlayManager::getSingleton().getByName("ScorsOverlay");

	mPlayerEnergy = 5;
	mTowerPrice = 5;
	mPoints = 10;
	mTowerLevel = 1;
	mLevel = 1;

	mDebugOverlay->show();
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

void ScoresManager::update()
{
	static Ogre::String energy = "Energy: ";
	static Ogre::String level = "Level: ";
	static Ogre::String points = "Points: ";
	static Ogre::String towerPrice = "Tower Price: ";
	static Ogre::String towerLevel = "Tower Level: ";


	Ogre::OverlayElement* guiEnergy = Ogre::OverlayManager::getSingleton().getOverlayElement("Energy");
	Ogre::OverlayElement* guiLevel = Ogre::OverlayManager::getSingleton().getOverlayElement("Level");
	Ogre::OverlayElement* guiPoints = Ogre::OverlayManager::getSingleton().getOverlayElement("Points");
	Ogre::OverlayElement* guiTowerPrice = Ogre::OverlayManager::getSingleton().getOverlayElement("TowerPrice");
	Ogre::OverlayElement* guiTowerLevel = Ogre::OverlayManager::getSingleton().getOverlayElement("TowerLevel");

	guiEnergy->setCaption(energy + Ogre::StringConverter::toString(mPlayerEnergy));
	guiLevel->setCaption(level + Ogre::StringConverter::toString(mLevel));
	guiPoints->setCaption(points + Ogre::StringConverter::toString(mPoints));
	guiTowerPrice->setCaption(towerPrice + Ogre::StringConverter::toString(mTowerPrice));
	guiTowerLevel->setCaption(towerLevel + Ogre::StringConverter::toString(mTowerLevel));

	

	
}


