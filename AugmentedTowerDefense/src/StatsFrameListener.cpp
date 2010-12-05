#include "StdAfx.h"
#include "StatsFrameListener.h"		

StatsFrameListener::StatsFrameListener(Ogre::RenderTarget *window)
{
	mWindow = window;
	mDebugText = "";
	mDebugOverlay = Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");
	mIsAttached = false;
//	showDebugOverlay(true);
}


StatsFrameListener::~StatsFrameListener()
{
	showDebugOverlay(false);
}

bool StatsFrameListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	return true;
}

bool StatsFrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
	if(mDebugOverlay)
		updateStats();
	return true;
}

void StatsFrameListener::showDebugOverlay(bool show)
{
	if (mDebugOverlay)
	{
		if (show)
		{
			if(!mIsAttached)
			{
				Ogre::Root::getSingleton().addFrameListener(this);
				mIsAttached = true;
			}
			mDebugOverlay->show();
		}
		else
		{
			if(mIsAttached)
			{
				Ogre::Root::getSingleton().removeFrameListener(this);
				mIsAttached = false;
			}
			mDebugOverlay->hide();
		}
	}
}

void StatsFrameListener::updateStats(void)
{
	static Ogre::String currFps = "Current FPS: ";
	static Ogre::String avgFps = "Average FPS: ";
	static Ogre::String bestFps = "Best FPS: ";
	static Ogre::String worstFps = "Worst FPS: ";
	static Ogre::String tris = "Triangle Count: ";
	static Ogre::String batches = "Batch Count: ";

	// update stats when necessary
	try {
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

		Ogre::OverlayElement* guiDbg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
		guiDbg->setCaption(mDebugText);
	}
	catch(...) { /* ignore */ }
}

void StatsFrameListener::toogleDebugOverlay()
{
	if(mDebugOverlay)	
	{
		bool bVisible = mDebugOverlay->isVisible();
		showDebugOverlay(!bVisible);
	}
}
