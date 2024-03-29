#include "StdAfx.h"
#include "OgreApp.h"
#include "AppLogic.h"
#include "OgreAppFrameListener.h"

OgreApp *Ogre::Singleton<class OgreApp>::ms_Singleton = 0;

OgreApp::OgreApp()
{
	mInitialisationBegun = false;
	mInitialisationComplete = false;

	// Ogre
	mRoot = 0;
	mFrameListener = 0;
	mWindow = 0;

	// framework
	mAppLogic = 0;

	// OIS
	mInputMgr = 0;
	mKeyboard = 0;
	mMouse = 0; 
}

OgreApp::~OgreApp()
{
	shutdown();
}

////////////////////////////////////////////////

bool OgreApp::init(void)
{
	try
	{
		assert(!mInitialisationBegun);
		mInitialisationBegun = true;

		if(!mAppLogic)
			return false;

		if(!mAppLogic->preInit(mCommandLineArgs))
			return false;

		mRoot = new Ogre::Root("plugins.cfg", "ogre.cfg", "Ogre.log");

		if(!mRoot->restoreConfig() && !mRoot->showConfigDialog())
			return false;

		mWindow = mRoot->initialise(true);

		createInputDevices(false);

		setupResources();

		mFrameListener = new OgreAppFrameListener(this);
		mRoot->addFrameListener(mFrameListener);

		if(!mAppLogic->init())
			return false;

		mInitialisationComplete = true;		
	}
	catch (Ogre::Exception& e) 
	{
		MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		return false;
	}	
	return true;
}

void OgreApp::run(void)
{
	if(init())
		mRoot->startRendering();
	shutdown();
}

bool OgreApp::preUpdate( Ogre::Real deltaTime )
{
	return mAppLogic->preUpdate(deltaTime);
}

bool OgreApp::update(Ogre::Real deltaTime)
{
	updateInputDevices();
	return mAppLogic->update(deltaTime);
}

void OgreApp::shutdown(void)
{
	if(!mInitialisationBegun)
		return;

	if(mInitialisationComplete)
		mAppLogic->shutdown();

	destroyInputDevices();

	delete mFrameListener;
	mFrameListener = 0;

	delete mRoot;
	mRoot = 0;

	if(mAppLogic)
		mAppLogic->postShutdown();

	mInitialisationComplete = false;
	mInitialisationBegun = false;
}

////////////////////////////////////////////////

void OgreApp::setAppLogic(AppLogic *appLogic)
{
	if(mAppLogic == appLogic)
		return;

	if(mAppLogic)
	{
		mAppLogic->setParentApp(0);
	}
	mAppLogic = appLogic;
	if(appLogic)
	{
		appLogic->setParentApp(this);
	}
}

void OgreApp::setCommandLine(const Ogre::String &commandLine)
{
	Ogre::String configFile;
	mCommandLine = commandLine;
	
	if(!commandLine.empty())
	{

		// splits command line in a vector without spliting text between quotes
		Ogre::StringVector quoteSplit = Ogre::StringUtil::split(commandLine, "\"");

		// if the first char was a quote, split() ignored it.
		if(commandLine[0] == '\"')
			quoteSplit.insert(quoteSplit.begin(), " "); // insert a space in the list to reflect the presence of the first quote
		// insert a space instead of an empty string because the next split() will ingore the space but not the empty string
		// split(" ")->{} / split("")->{""}

		for(unsigned int i = 0; i < quoteSplit.size(); i++)
		{
			if(i&1) // odd elements : between quotes
			{
				mCommandLineArgs.push_back(quoteSplit[i]);
			}
			else // even elements : outside quotes
			{
				Ogre::StringVector spaceSplit = Ogre::StringUtil::split(quoteSplit[i]);
				mCommandLineArgs.insert(mCommandLineArgs.end(), spaceSplit.begin(), spaceSplit.end());
			}
		}
	}
}

////////////////////////////////////////////////

void OgreApp::notifyWindowMetrics(Ogre::RenderWindow* rw, int left, int top, int width, int height)
{
	if(mMouse)
	{
		const OIS::MouseState &ms = mMouse->getMouseState();
		ms.height = height; // mutable fields
		ms.width = width;
	}
}

void OgreApp::notifyWindowClosed(Ogre::RenderWindow* rw)
{
	destroyInputDevices();
}


////////////////////////////////////////////////

/// Method which will define the source of resources (other than current folder)
void OgreApp::setupResources(void)
{
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

////////////////////////////////////////////////
void OgreApp::createInputDevices(bool exclusive)
{
	OIS::ParamList pl;
	size_t winHandle = 0;

	mWindow->getCustomAttribute("WINDOW", &winHandle);
	pl.insert(std::make_pair("WINDOW", Ogre::StringConverter::toString(winHandle)));

	if(!exclusive)
	{
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	}

	mInputMgr = OIS::InputManager::createInputSystem(pl);

	mKeyboard = static_cast<OIS::Keyboard*>(mInputMgr->createInputObject(OIS::OISKeyboard, true));
	mMouse = static_cast<OIS::Mouse*>(mInputMgr->createInputObject(OIS::OISMouse, true));
}
void OgreApp::updateInputDevices(void)
{
	if(mInputMgr)
	{
		mKeyboard->capture();
		mMouse->capture();
	}
}
void OgreApp::destroyInputDevices(void)
{
	if(mInputMgr)
	{
		mInputMgr->destroyInputObject(mKeyboard);
		mKeyboard = 0;
		mInputMgr->destroyInputObject(mMouse);
		mMouse = 0;

		OIS::InputManager::destroyInputSystem(mInputMgr);
		mInputMgr = 0;
	}
}