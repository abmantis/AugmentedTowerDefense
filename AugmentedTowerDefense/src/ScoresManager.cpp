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
	mLevel = 0;

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
	static Ogre::String currFps = "Current FPS: ";
	static Ogre::String avgFps = "Average FPS: ";
	static Ogre::String bestFps = "Best FPS: ";
	static Ogre::String worstFps = "Worst FPS: ";
	static Ogre::String tris = "Triangle Count: ";
	static Ogre::String batches = "Batch Count: ";


	Ogre::OverlayElement* guiAvg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
	Ogre::OverlayElement* guiCurr = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
	Ogre::OverlayElement* guiBest = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
	Ogre::OverlayElement* guiWorst = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

	const Ogre::RenderTarget::FrameStats& stats = mWindow->getStatistics();
	guiAvg->setCaption(avgFps + Ogre::StringConverter::toString(stats.avgFPS));
	guiCurr->setCaption(currFps + Ogre::StringConverter::toString(stats.lastFPS));
	guiBest->setCaption(bestFps + Ogre::StringConverter::toString(stats.bestFPS)
		+" "+ Ogre::StringConverter::toString(stats.bestFrameTime)+" ms");
	guiWorst->setCaption(worstFps + Ogre::StringConverter::toString(stats.worstFPS)
		+" "+ Ogre::StringConverter::toString(stats.worstFrameTime)+" ms");

	Ogre::OverlayElement* guiTris = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
	guiTris->setCaption(tris + Ogre::StringConverter::toString(stats.triangleCount));

	Ogre::OverlayElement* guiBatches = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
	guiBatches->setCaption(batches + Ogre::StringConverter::toString(stats.batchCount));

	
}


