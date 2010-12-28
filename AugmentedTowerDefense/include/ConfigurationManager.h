#ifndef ConfigurationManager_h__
#define ConfigurationManager_h__

#pragma once

class ConfigurationManager : public Ogre::Singleton<ConfigurationManager>
{
public:
	ConfigurationManager();
	~ConfigurationManager();

	void init();

	Ogre::TextureFilterOptions TextureFilter() const { return mTextureFilter; }	
	Ogre::ShadowTechnique ShadowType() const { return mShadowType; }
	Ogre::uint AnisotropyLevel() const { return mAnisotropyLevel; }

	/** Override standard Singleton retrieval.
	@remarks
	Why do we do this? Well, it's because the Singleton
	implementation is in a .h file, which means it gets compiled
	into anybody who includes it. This is needed for the
	Singleton template to work, but we actually only want it
	compiled into the implementation of the class based on the
	Singleton, not all of them. If we don't change this, we get
	link errors when trying to use the Singleton-based class from
	an outside dll.
	*/
	static ConfigurationManager& getSingleton(void);
	static ConfigurationManager* getSingletonPtr(void);
private:
	void setDefaults();

	static std::string mConfigFilename;

	Ogre::ShadowTechnique mShadowType;
	
	
	Ogre::TextureFilterOptions mTextureFilter;

	Ogre::uint mAnisotropyLevel;
	
};
#endif // ConfigurationManager_h__
