#include "StdAfx.h"
#include "OgreApp.h"
#include "Chrono.h"
#include "StatsFrameListener.h"
#include "SharedStuff.h"
#include "AppLogic.h"


AppLogic::AppLogic() : mApplication()
{
	mSceneMgr			= NULL;
	mViewport			= NULL;
	mCamera				= NULL;
	mCameraNode			= NULL;
	mVideoDevice		= NULL;
	mObjectNode			= NULL;
	mTrackingSystem		= NULL;
	mStatsFrameListener = NULL;
	mAnimState			= NULL;
	mSceneLoader		= NULL;
	mEnemyMgr			= NULL;
	mTowerMgr			= NULL;
	mConfigMgr			= NULL;
	mScoresMgr			= NULL;
	mHUDMgr				= NULL;

	mOISListener.mParent = this;

	mContinue = true;
	mGamePaused = true;
}

AppLogic::~AppLogic()
{}

// preAppInit
bool AppLogic::preInit(const Ogre::StringVector &commandArgs)
{
	return true;
}

// postAppInit
bool AppLogic::init(void)
{
	mConfigMgr = new ConfigurationManager();
	mConfigMgr->init();

	createSceneManager();
	createViewport();
	createCamera();
	setupLights();
	
	mScoresMgr = new ScoresManager();
	createScene();

	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(mConfigMgr->TextureFilter());
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(mConfigMgr->AnisotropyLevel());

	mColisionTools = new MOC::CollisionTools(mSceneMgr);
	
	//webcam resolution
	int width, height;
	initTracking(width, height);
	createWebcamPlane(width, height, 4500.0f);	

	mHUDMgr = new HUDManager();
	mHUDMgr->init();

	mStatsFrameListener = new StatsFrameListener(mApplication->getRenderWindow());
	mApplication->getOgreRoot()->addFrameListener(mStatsFrameListener);
	mStatsFrameListener->showDebugOverlay(true);

	mApplication->getKeyboard()->setEventCallback(&mOISListener);
	mApplication->getMouse()->setEventCallback(&mOISListener);


	mHUDMgr->showPopup(true, "Press SPACE to start");

	return true;
}

bool AppLogic::preUpdate(Ogre::Real deltaTime)
{
	if(mApplication->getRenderWindow()->isActive() == false) pause(true);
	return true;
}

bool AppLogic::update(Ogre::Real deltaTime)
{
	if(!mContinue) return false;

	//If there is a new frame available on video device
	if (mVideoDevice->update())
	{
		Ogre::PixelBox box(mVideoDevice->getWidth(), mVideoDevice->getHeight(), 1, Ogre::PF_B8G8R8, (void*) mVideoDevice->getBufferData());

		//Tracking using ArToolKitPlus
		if (mTrackingSystem->update(box))
		{
			//mObjectNode->setVisible(true);
			showScene();
			mCameraNode->setOrientation(mTrackingSystem->getOrientation());
			mCameraNode->setPosition(mTrackingSystem->getTranslation());
		}
		else
		{
			hideScene();
			//mObjectNode->setVisible(false);
		}
	}

	if(!mGamePaused)
	{
		std::vector<int> shootedEnemies;
		mEnemyMgr->update(deltaTime);
		shootedEnemies = mTowerMgr->update(deltaTime, &(mEnemyMgr->getEnemyPos()));
		mEnemyMgr->addShotsToEnemies(shootedEnemies);
	}

	mHUDMgr->update();

	return true;
}

void AppLogic::shutdown(void)
{
	mVideoDevice->shutdown();
//	if(mVideoDevice) delete mVideoDevice;
	mVideoDevice = NULL;

	if(mHUDMgr) delete mHUDMgr;
	mHUDMgr = NULL;

	if(mConfigMgr) delete mConfigMgr;
	mConfigMgr = NULL;

	if(mScoresMgr) delete mScoresMgr;
	mScoresMgr = NULL;

	if(mColisionTools) delete mColisionTools;
	mColisionTools = NULL;

	if(mSceneLoader) delete mSceneLoader;
	mSceneLoader = NULL;

	if(mEnemyMgr) delete mEnemyMgr;
	mEnemyMgr = NULL;

	if(mTowerMgr) delete mTowerMgr;
	mTowerMgr = NULL;

	if(mTrackingSystem) delete mTrackingSystem;
	mTrackingSystem = NULL;

	mApplication->getOgreRoot()->removeFrameListener(mStatsFrameListener);
	if(mStatsFrameListener) delete mStatsFrameListener;
	mStatsFrameListener = NULL;
	
	if(mSceneMgr) mApplication->getOgreRoot()->destroySceneManager(mSceneMgr);
	mSceneMgr = NULL;
}

void AppLogic::postShutdown(void)
{

}

//--------------------------------- Init --------------------------------

void AppLogic::createSceneManager(void)
{
	mSceneMgr = mApplication->getOgreRoot()->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
}

void AppLogic::createViewport(void)
{
	mViewport = mApplication->getRenderWindow()->addViewport(0);
}

void AppLogic::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("camera");
	mCamera->setNearClipDistance(0.5);
	mCamera->setFarClipDistance(10000);
	mCamera->setPosition(0, 0, 0);
	mCamera->lookAt(0, 0, 1);
	mCamera->setFOVy(Ogre::Degree(26)); //FOVy camera Ogre = 40°
	mCamera->setAspectRatio((float) mViewport->getActualWidth() / (float) mViewport->getActualHeight());	
	mViewport->setCamera(mCamera);

	mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("cameraNode");
	mCameraNode->setPosition(0, 0, 50000);
	//mCameraNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
	mCameraNode->attachObject(mCamera);
	mCameraNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
	//mCameraNode->setOrientation(mCameraNode->getOrientation() * Ogre::Quaternion(Ogre::Degree(180.f), Ogre::Vector3::UNIT_Y));
	mCameraNode->setDirection(0,0,1, Ogre::SceneNode::TS_WORLD);
}

void AppLogic::createScene(void)
{
	Ogre::Entity::setDefaultQueryFlags(AugmentedTowerDefense::MASK_DEFAULT);
	mSceneMgr->setSkyBox(true, "Examples/Grid");

	mSceneLoader = new SceneLoader(mSceneMgr);
	mSceneLoader->init();
	mSceneLoader->hide();

	std::vector<Ogre::Vector3> walkArray = mSceneLoader->getWalkVector();
	//for(Ogre::uint i = 1; i < walkArray.size(); i++)
	//{
	//	HelperClass::CreateLine(mSceneMgr, walkArray[i-1], walkArray[i]);
	//}

	mEnemyMgr = new EnemyManager(mSceneMgr);
	mEnemyMgr->init(walkArray);
	mEnemyMgr->hide();

	mTowerMgr = new TowerManager(mSceneMgr);
	mTowerMgr->init();
	mTowerMgr->hide();

// 	Ogre::Real scale = 10;
// 	Ogre::Entity* ent = mSceneMgr->createEntity("Sinbad.mesh");	//1x1_cube.mesh //Sinbad.mesh //axes.mesh
// 
// 	mObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("cube");	
// 	mObjectNode->setOrientation(Quaternion(Degree(90.f), Vector3::UNIT_X));
// 	mObjectNode->setPosition(0, 0, 5*scale);
// 	mObjectNode->setScale(Ogre::Vector3::UNIT_SCALE*scale);
// 	mObjectNode->attachObject(ent);
// 
// 	// create swords and attach them to sinbad
// 	Ogre::Entity* sword1 = mSceneMgr->createEntity("SinbadSword1", "Sword.mesh");
// 	Ogre::Entity* sword2 = mSceneMgr->createEntity("SinbadSword2", "Sword.mesh");
// 	ent->attachObjectToBone("Sheath.L", sword1);
// 	ent->attachObjectToBone("Sheath.R", sword2);
// 	mAnimState = ent->getAnimationState("Dance");
// 	mAnimState->setLoop(true);
// 	mAnimState->setEnabled(true);


//	HelperClass::CreateAxis(mSceneMgr);
}

void AppLogic::initTracking( int &width, int &height )
{
	int iCameraCount = mVideoDeviceManager.size();
	if (iCameraCount > 0)
	{
		for(int i = 0; i < iCameraCount; i++)
		{
			mVideoDevice = mVideoDeviceManager[i];
			if(mVideoDevice->init())
			{
				break;
			}
		}
		if(mVideoDevice->IsWorking())
		{
//			mVideoDevice->showControlPanel();
			width = mVideoDevice->getWidth();
			height = mVideoDevice->getHeight();

			mVideoDevice->createTexture("WebcamTexture");

			//Create Webcam Material
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("WebcamMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			Ogre::Technique *technique = material->createTechnique();
			technique->createPass();
			material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
			material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
			material->getTechnique(0)->getPass(0)->createTextureUnitState("WebcamTexture");	

			// Init TrackingSystem
			mTrackingSystem = new TrackingSystem();
			mTrackingSystem->init(width, height);
		}
		else
		{
			throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "Webcam not working", "AppLogic");
		}
	}
	else
	{
		throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "No webcam found", "AppLogic");
	}
}

void AppLogic::createWebcamPlane(int width, int height, Ogre::Real _distanceFromCamera)
{
	// Create a prefab plane dedicated to display video
	float videoAspectRatio = width / (float) height;

	float planeHeight = 2 * _distanceFromCamera * Ogre::Math::Tan(Ogre::Degree(26)*0.5); //FOVy webcam = 26° (intrinsic param)
	float planeWidth = planeHeight * videoAspectRatio;

	Ogre::Plane p(Ogre::Vector3::UNIT_Z, 0.0);
	Ogre::MeshManager::getSingleton().createPlane("VerticalPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		p , planeWidth, planeHeight, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* planeEntity = mSceneMgr->createEntity("VideoPlane", "VerticalPlane"); 
	planeEntity->setMaterialName("WebcamMaterial");
	planeEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1);
	planeEntity->setCastShadows(false);

	// Create a node for the plane, inserts it in the scene
	Ogre::SceneNode* node = mCameraNode->createChildSceneNode("planeNode");
	node->attachObject(planeEntity);

	// Update position    
	Ogre::Vector3 planePos(0, 0, _distanceFromCamera);// = mCamera->getPosition() + mCamera->getDirection() * _distanceFromCamera;
	node->setPosition(planePos);

	// Update orientation
	node->setDirection(Ogre::Vector3::UNIT_Z);
	//node->setOrientation(mCamera->getOrientation());

//	planeEntity->setVisible(false);

}

void AppLogic::setupLights()
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
// 	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
	mSceneMgr->setShadowTechnique(mConfigMgr->ShadowType());
	
	Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
	pointLight->setType(Ogre::Light::LT_POINT);	
	pointLight->setDiffuseColour(0.8f, 0.8f, 0.8f);
	pointLight->setSpecularColour(1.0f, 1.0f, 1.0f);
	pointLight->setPosition(Ogre::Vector3(0, 100, 250));

 	//Ogre::Light* dirLight = mSceneMgr->createLight("dirLight");
 	//dirLight->setType(Ogre::Light::LT_DIRECTIONAL);	
 	//dirLight->setDiffuseColour(0.8f, 0.8f, 0.8f);
 	//dirLight->setSpecularColour(1.0f, 1.0f, 1.0f);
 	//dirLight->setDirection(Ogre::Vector3(-0.2f, -0.5f, -1));

}

void AppLogic::hideScene()
{
	mSceneLoader->hide();
	mEnemyMgr->hide();
	mTowerMgr->hide();
}

void AppLogic::showScene()
{
	mSceneLoader->show();
	mEnemyMgr->show();
	mTowerMgr->show();
}

void AppLogic::pause( bool pause )
{
	mGamePaused = pause; 
	if(mGamePaused) mHUDMgr->showPopup(true, "Press SPACE to continue");
	else mHUDMgr->showPopup(false, "");
}

//--------------------------------- Inputs --------------------------------

bool AppLogic::OISListener::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}

bool AppLogic::OISListener::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{		
	return true;
}

bool AppLogic::OISListener::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(mParent->mGamePaused) return true;

	Ogre::Vector2 mouseCoords((float)arg.state.X.abs,(float)arg.state.Y.abs * 1.0f);
	Ogre::Entity *pWallTarget = NULL;
	float closestDist;
	Ogre::Vector3 result;

	// fire a ray from camera position that only detects walls (MASK_WALLS)
	if(mParent->mColisionTools->raycastFromCamera(mParent->mApplication->getRenderWindow(),
		mParent->mCamera, mouseCoords, result, pWallTarget, closestDist, AugmentedTowerDefense::MASK_WALLS))
	{
		Ogre::SceneNode *pWallNode = pWallTarget->getParentSceneNode();
		Ogre::Vector3 wallPos = pWallNode->getPosition();	
		Ogre::Entity *pTowerTarget = NULL;
		Ogre::Vector3 pointAboveWallCenter(wallPos.x, wallPos.y, 99999);

		//////////////////////////////////////////////////////////////////////////
		// Make sure there's not a tower already in this wall 
		if(pWallTarget->isVisible() && mParent->mColisionTools->raycastFromPoint(
			pointAboveWallCenter, Ogre::Vector3::NEGATIVE_UNIT_Z, result, pTowerTarget, 
			closestDist, AugmentedTowerDefense::MASK_TOWER) == false)
		{
			
			wallPos.z += pWallTarget->getBoundingBox().getSize().z * pWallNode->getScale().z * 0.5f;
			mParent->mTowerMgr->addTower(wallPos);
		}	
	}
	return true;
}

bool AppLogic::OISListener::keyPressed( const OIS::KeyEvent &arg )
{
	switch (arg.key)
	{
	case OIS::KC_ESCAPE:
		mParent->end();
		break;
	case OIS::KC_SPACE:
		mParent->pause(!mParent->mGamePaused);		
		break;	

	case OIS::KC_F1:
		mParent->mCamera->setPolygonMode( Ogre::PM_SOLID );		
		break;
	case OIS::KC_F2:
		mParent->mCamera->setPolygonMode( Ogre::PM_WIREFRAME );
		break;
	case OIS::KC_F3:
		mParent->mCamera->setPolygonMode( Ogre::PM_POINTS );
		break;
	case OIS::KC_F9:
		mParent->mStatsFrameListener->toogleDebugOverlay();
		break;
	case OIS::KC_F10:
		mParent->mSceneLoader->togleVisibility();
		break;
	case OIS::KC_F11:
		//mParent->mCameraNode->setPosition(50,0,560);
		//mParent->mCameraNode->setDirection(0,0,1);
		//mParent->mSceneLoader->togleVisibility();
		mParent->mTrackingSystem->mSimulate = !mParent->mTrackingSystem->mSimulate;
		break;
	}
	return true;
}

bool AppLogic::OISListener::keyReleased( const OIS::KeyEvent &arg )
{
	return true;
}