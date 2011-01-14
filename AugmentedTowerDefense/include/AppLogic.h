#ifndef APPLOGIC_H
#define APPLOGIC_H
#pragma once

#include "VideoDeviceManager.h"
#include "TrackingSystem.h"
#include "SceneLoader.h"
#include "EnemyManager.h"
#include "TowerManager.h"
#include "CollisionTools.h"
#include "ConfigurationManager.h"
#include "ScoresManager.h"
#include "HUDManager.h"

class OgreApp;
class StatsFrameListener;

class AppLogic
{
public:
	AppLogic();
	~AppLogic();

	void setParentApp(OgreApp *app) { mApplication = app; }

	/// Called before Ogre and everything in the framework is initialized
	/// Configure the framework here
	bool preInit(const Ogre::StringVector &commandArgs);
	/// Called when Ogre and the framework is initialized
	/// Init the logic here
	bool init(void);

	/// Called before everything in the framework is updated
	bool preUpdate(Ogre::Real deltaTime);
	/// Called when the framework is updated
	/// update the logic here
	bool update(Ogre::Real deltaTime);

	/// Called before Ogre and the framework are shut down
	/// shutdown the logic here
	void shutdown(void);
	/// Called when Ogre and the framework are shut down
	void postShutdown(void);

protected:
	void createSceneManager(void);
	void createViewport(void);
	void createCamera(void);
	void createScene(void);
	void setupLights();
	
	void initTracking(int &width, int &height);
	void createWebcamPlane(int width, int height, Ogre::Real _distanceFromCamera);

	void hideScene();
	void showScene();

	void addTowerFromMarker(Ogre::Vector3 markerPos);

	void end() { mContinue = false; }
	void pause(bool pause);


	OgreApp *mApplication;
	Ogre::SceneManager *mSceneMgr;
	Ogre::Viewport *mViewport;
	Ogre::Camera *mCamera;
	Ogre::SceneNode* mObjectNode;
	Ogre::SceneNode* mCameraNode;
	Ogre::SceneNode* mBaseSceneNode;
	VideoDeviceManager mVideoDeviceManager;
	VideoDevice* mVideoDevice;
	TrackingSystem* mTrackingSystem;
	SceneLoader* mSceneLoader;
	EnemyManager* mEnemyMgr;
	TowerManager* mTowerMgr;
	Ogre::AnimationState* mAnimState;
	MOC::CollisionTools* mColisionTools;
	ConfigurationManager* mConfigMgr;
	ScoresManager* mScoresMgr;
	HUDManager* mHUDMgr;
	StatsFrameListener *mStatsFrameListener;

	int mSingleMarkers[2];
	int mSingleMarkersCount;
	Ogre::Real mTimeSinceSeenTowerMarker;

	bool mContinue;
	bool mGamePaused;
	bool mGameOver;

	// OIS
	class OISListener : public OIS::MouseListener, public OIS::KeyListener
	{
	public:
		virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool keyPressed( const OIS::KeyEvent &arg );
		virtual bool keyReleased( const OIS::KeyEvent &arg );
		AppLogic *mParent;
	};
	friend class OISListener;
	OISListener mOISListener;
};

#endif // APPLOGIC_H