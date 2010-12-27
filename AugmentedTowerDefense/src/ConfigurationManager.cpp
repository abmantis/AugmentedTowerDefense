#include "StdAfx.h"
#include "ConfigurationManager.h"

template<> ConfigurationManager* Ogre::Singleton<ConfigurationManager>::ms_Singleton = 0;
std::string ConfigurationManager::mConfigFilename = "ARTD.cfg";

ConfigurationManager::ConfigurationManager()
{
}

ConfigurationManager::~ConfigurationManager()
{
}

void ConfigurationManager::init()
{
	setDefaults();

	Ogre::String valueStr;
	Ogre::LogManager::getSingletonPtr()
		->logMessage("Reading main configurations from " + mConfigFilename);

	// Load config file
	Ogre::ConfigFile configFile;
	configFile.load(mConfigFilename);

	//Shadow type
	valueStr = configFile.getSetting("ShadowQuality");
	if		(valueStr.compare("0") == 0) mShadowType = Ogre::SHADOWTYPE_NONE;
	else if	(valueStr.compare("1") == 0) mShadowType = Ogre::SHADOWTYPE_TEXTURE_ADDITIVE;
	else if	(valueStr.compare("2") == 0) mShadowType = Ogre::SHADOWTYPE_STENCIL_MODULATIVE;
	else if	(valueStr.compare("3") == 0) mShadowType = Ogre::SHADOWTYPE_STENCIL_ADDITIVE;

	//Texture Filtering
	valueStr = configFile.getSetting("TexFilter");
	if		(valueStr.compare("0") == 0) mTextureFilter = Ogre::TFO_NONE;
	else if	(valueStr.compare("1") == 0) mTextureFilter = Ogre::TFO_BILINEAR;
	else if	(valueStr.compare("2") == 0) mTextureFilter = Ogre::TFO_TRILINEAR;
	else if	(valueStr.compare("3") == 0) mTextureFilter = Ogre::TFO_ANISOTROPIC;

	//Anisotropric Filtering
	valueStr = configFile.getSetting("Anisotropy");
	mAnisotropyLevel = Ogre::StringConverter::parseInt(valueStr, mAnisotropyLevel);

	Ogre::LogManager::getSingletonPtr()
		->logMessage("Configurations loaded from " + mConfigFilename);
}

void ConfigurationManager::setDefaults()
{
	mShadowType = Ogre::SHADOWTYPE_NONE;
	mTextureFilter = Ogre::TFO_BILINEAR;
	mAnisotropyLevel = 4;
}
