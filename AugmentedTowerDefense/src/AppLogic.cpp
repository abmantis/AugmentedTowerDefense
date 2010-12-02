#include "StdAfx.h"
#include "OgreApp.h"
#include "Chrono.h"
#include "StatsFrameListener.h"
#include "DebugStuff.h"
#include "AppLogic.h"

using namespace Ogre;

AppLogic::AppLogic() : mApplication(0)
{
	// ogre
	mSceneMgr		= 0;
	mViewport		= 0;
	mCamera         = 0;
	mCameraNode     = 0;
	mVideoDevice    = 0;
	mObjectNode     = 0;
	mTrackingSystem = 0;
	mStatsFrameListener = 0;
	mAnimState = 0;

	mOISListener.mParent = this;
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
	createSceneManager();
	createViewport();
	createCamera();
	setupLights();
	createScene();

	mSceneLoader = new SceneLoader(mSceneMgr);
	mSceneLoader->init();
	
	//webcam resolution
	int width;
	int height;

	initTracking(width, height);
	createWebcamPlane(width, height, 45000.0f);	

	mStatsFrameListener = new StatsFrameListener(mApplication->getRenderWindow());
	mApplication->getOgreRoot()->addFrameListener(mStatsFrameListener);
	mStatsFrameListener->showDebugOverlay(true);

	mApplication->getKeyboard()->setEventCallback(&mOISListener);
	mApplication->getMouse()->setEventCallback(&mOISListener);

	return true;
}

bool AppLogic::preUpdate(Ogre::Real deltaTime)
{
	return true;
}

bool AppLogic::update(Ogre::Real deltaTime)
{
	//If there is a new frame available on video device
	if (mVideoDevice->update())
	{
		Ogre::PixelBox box(mVideoDevice->getWidth(), mVideoDevice->getHeight(), 1, Ogre::PF_B8G8R8, (void*) mVideoDevice->getBufferData());

		//Tracking using ArToolKitPlus
		if (mTrackingSystem->update(box))
		{
			//mObjectNode->setVisible(true);
			mSceneLoader->show();
			mCameraNode->setOrientation(mTrackingSystem->getOrientation());
			mCameraNode->setPosition(mTrackingSystem->getTranslation());
		}
		else
		{
			mSceneLoader->hide();
			//mObjectNode->setVisible(false);
		}
	}

//	HelperClass::Print(mCameraNode->getPosition());

// 	if (mAnimState)
// 		mAnimState->addTime(deltaTime);

	bool result = processInputs(deltaTime);
	return result;
}

void AppLogic::shutdown(void)
{
	mVideoDevice->shutdown();
//	if(mVideoDevice) delete mVideoDevice;
	mVideoDevice = NULL;

	if(mSceneLoader) delete mSceneLoader;
	mSceneLoader = NULL;

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
	mSceneMgr = mApplication->getOgreRoot()->createSceneManager(ST_GENERIC, "SceneManager");
}

void AppLogic::createViewport(void)
{
	mViewport = mApplication->getRenderWindow()->addViewport(0);
}

void AppLogic::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("camera");
	mCamera->setNearClipDistance(0.5);
	mCamera->setFarClipDistance(50000);
	mCamera->setPosition(0, 0, 0);
	mCamera->lookAt(0, 0, 1);
	mCamera->setFOVy(Degree(26)); //FOVy camera Ogre = 40�
	mCamera->setAspectRatio((float) mViewport->getActualWidth() / (float) mViewport->getActualHeight());	
	mViewport->setCamera(mCamera);

	mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("cameraNode");
	mCameraNode->setPosition(0, 0, 100000);
	//mCameraNode->lookAt(Vector3(0, 1700, -1), Node::TS_WORLD);
	mCameraNode->lookAt(Vector3(0, 0, 0), Node::TS_WORLD);
	mCameraNode->attachObject(mCamera);
	mCameraNode->setFixedYawAxis(true, Vector3::UNIT_Y);
	mCameraNode->setOrientation(mCameraNode->getOrientation() * Ogre::Quaternion(Degree(180.f), Vector3::UNIT_Y));
}

void AppLogic::createScene(void)
{
	mSceneMgr->setSkyBox(true, "Examples/Grid");

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


	HelperClass::CreateAxis(mSceneMgr);
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
			MaterialPtr material = MaterialManager::getSingleton().create("WebcamMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
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

	float planeHeight = 2 * _distanceFromCamera * Ogre::Math::Tan(Degree(26)*0.5); //FOVy webcam = 26� (intrinsic param)
	float planeWidth = planeHeight * videoAspectRatio;

	Plane p(Vector3::UNIT_Z, 0.0);
	MeshManager::getSingleton().createPlane("VerticalPlane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p , planeWidth, planeHeight, 1, 1, true, 1, 1, 1, Vector3::UNIT_Y);
	Entity* planeEntity = mSceneMgr->createEntity("VideoPlane", "VerticalPlane"); 
	planeEntity->setMaterialName("WebcamMaterial");
	planeEntity->setRenderQueueGroup(RENDER_QUEUE_WORLD_GEOMETRY_1);

	// Create a node for the plane, inserts it in the scene
	Ogre::SceneNode* node = mCameraNode->createChildSceneNode("planeNode");
	node->attachObject(planeEntity);

	// Update position    
	Vector3 planePos = mCamera->getPosition() + mCamera->getDirection() * _distanceFromCamera;
	node->setPosition(planePos);

	// Update orientation
	node->setOrientation(mCamera->getOrientation());
}

//--------------------------------- update --------------------------------

bool AppLogic::processInputs(Ogre::Real deltaTime)
{
	OIS::Keyboard *keyboard = mApplication->getKeyboard();
	if(keyboard->isKeyDown(OIS::KC_ESCAPE))
	{
		return false;
	}

	return true;
}

void AppLogic::setupLights()
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
	pointLight->setType(Ogre::Light::LT_POINT);	
	pointLight->setDiffuseColour(1.0f, 1.0f, 1.0f);
	pointLight->setSpecularColour(1.0f, 1.0f, 1.0f);
//	mCameraNode->attachObject(pointLight);
	pointLight->setPosition(Ogre::Vector3(0, 120, 250));
//	pointLight->setDirection(1,-1,-1);
}

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
	return true;
}

bool AppLogic::OISListener::keyPressed( const OIS::KeyEvent &arg )
{
	Ogre::SceneNode *pWebcamNode = static_cast<Ogre::SceneNode*>(mParent->mCameraNode->getChild("planeNode"));
	switch (arg.key)
	{
	case OIS::KC_UP:
		pWebcamNode->translate(0,0,500);
		break;
	case OIS::KC_DOWN:
		pWebcamNode->translate(0,0,-500);
		break;
	case OIS::KC_F9:
		mParent->mStatsFrameListener->toogleDebugOverlay();
		break;
	case OIS::KC_F10:
		mParent->mSceneLoader->togleVisibility();
		break;
	}
	return true;
}

bool AppLogic::OISListener::keyReleased( const OIS::KeyEvent &arg )
{
	return true;
}