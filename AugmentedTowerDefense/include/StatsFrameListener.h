#ifndef StatsFrameListener_h__
#define StatsFrameListener_h__


class StatsFrameListener : public Ogre::FrameListener
{
public:
	StatsFrameListener(Ogre::RenderTarget *window);
	~StatsFrameListener();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);

	void showDebugOverlay(bool show);
	void toogleDebugOverlay();
	void setDebugText(const Ogre::String &debugText) { mDebugText = debugText; }

protected:
	void updateStats(void);

	Ogre::String mDebugText;
	Ogre::Overlay* mDebugOverlay;
	Ogre::RenderTarget *mWindow;
	bool mIsAttached;
};

#endif // StatsFrameListener_h__
