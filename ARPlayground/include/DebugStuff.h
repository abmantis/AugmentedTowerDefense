

class DebugStuff
{
public:
	DebugStuff(void);
	virtual ~DebugStuff(void);

	static void CreateAxis(Ogre::SceneManager *sceneMgr);
	static void CreateLineAxis(Ogre::SceneManager *sceneMgr);
	static void Print(Ogre::Vector3 vector3, std::string prefix = "", std::string sufix = "\n");
};
