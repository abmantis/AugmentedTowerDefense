#ifndef __RenderSystemConfig_h_
#define __RenderSystemConfig_h_

#include <Ogre.h>
#include <OgreConfigFile.h>

using namespace Ogre;


#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>

// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
// for locating your configuration files and resources.
std::string macBundlePath()
{
    char path[1024];
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    assert(mainBundle);

    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    assert(mainBundleURL);

    CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
    assert(cfStringRef);

    CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

    CFRelease(mainBundleURL);
    CFRelease(cfStringRef);

    return std::string(path);
}
#endif

class RenderSystemConfig
{
public:
	RenderSystemConfig();
	~RenderSystemConfig();
	virtual void go();
protected:
	Root *mRoot;
	Ogre::String mResourcePath;


	void createRoot();
	void defineResources();
	bool setupRenderSystem();
	void createRenderWindow();

};
#endif // #ifndef __RenderSystemConfig_h_