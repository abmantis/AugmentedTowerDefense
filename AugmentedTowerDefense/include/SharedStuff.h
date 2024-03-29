#ifndef SharedStuff_h__
#define SharedStuff_h__
#pragma once

namespace AugmentedTowerDefense
{
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
		static std::string ToString(Ogre::Vector3 vector3);
		static std::string ToString(Ogre::Vector2 vector2);
		static void DestroyAllAttachedMovableObjects(Ogre::SceneNode* pNode);
		static void DoSafeRotation(Ogre::SceneNode *pNode, Ogre::Vector3 src, Ogre::Vector3 direction);
	};

	enum QueryFlags
	{
		MASK_NONE			= 0,

		MASK_WALLS			= 1<<0,
		MASK_TOWER			= 1<<1,

		MASK_DEFAULT		= 1<<31
	};
}

#endif // SharedStuff_h__
