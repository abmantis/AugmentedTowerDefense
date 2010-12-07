#ifndef HelperClass_h__
#define HelperClass_h__
#pragma once

class HelperClass
{
public:
	HelperClass(void);
	virtual ~HelperClass(void);

	static void CreateAxis(Ogre::SceneManager *sceneMgr);
	static void CreateLineAxis(Ogre::SceneManager *sceneMgr);
	static void CreateLine(Ogre::SceneManager *sceneMgr, Ogre::Vector3 start, Ogre::Vector3 end);
	static void Print(Ogre::Vector3 vector3, std::string prefix = "", std::string sufix = "\n");
	static void Print(Ogre::Matrix4 matrix4, std::string prefix = "", std::string sufix = "\n");
	static std::string ToString(int iVal);
};
#endif // HelperClass__