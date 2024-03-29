#ifndef OGREAPP_H
#define OGREAPP_H
#pragma once

class AppLogic;

class OgreApp : public Ogre::Singleton<OgreApp>
{
public:

	OgreApp();
	~OgreApp();
	
	void setAppLogic(AppLogic *appLogic);

	void run(void);
	bool preUpdate(Ogre::Real deltaTime);
	bool update(Ogre::Real deltaTime);
	void notifyWindowMetrics(Ogre::RenderWindow* rw, int left, int top, int width, int height);
	void notifyWindowClosed(Ogre::RenderWindow* rw);

	void setCommandLine(const Ogre::String &commandLine);
	void setCommandLine(int argc, char *argv[]);

	inline Ogre::RenderWindow *getRenderWindow(void) const { return mWindow; }
	inline Ogre::Root *getOgreRoot(void) const { return mRoot; }

	inline OIS::Mouse *getMouse(void) const { return mMouse; }
	inline OIS::Keyboard *getKeyboard(void) const { return mKeyboard; }
protected:

	bool init(void);
	void shutdown(void);

	void createInputDevices(bool exclusive);
	void updateInputDevices(void);
	void destroyInputDevices(void);

	void setupResources(void);

	// Config
	bool mInitialisationBegun;
	bool mInitialisationComplete;

	Ogre::String mCommandLine;
	Ogre::StringVector mCommandLineArgs;

	// OGRE
	Ogre::Root *mRoot;
	Ogre::FrameListener *mFrameListener;
	Ogre::RenderWindow *mWindow;

	// OIS
	OIS::InputManager *mInputMgr;
	OIS::Keyboard* mKeyboard;
	OIS::Mouse* mMouse;  

	// Classes supplied by the user
	AppLogic *mAppLogic;
};

#endif // OGREAPP_H