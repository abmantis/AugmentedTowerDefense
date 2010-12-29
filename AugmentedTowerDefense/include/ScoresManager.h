#ifndef ScoresManager_h__
#define ScoresManager_h__

#pragma once

class ScoresManager : public Ogre::Singleton<ScoresManager>
{
public:
	ScoresManager();
	~ScoresManager();

	void update();

	int GetPlayerEnergy() { return mPlayerEnergy; }
	int GetTowerPrice() { return mTowerPrice; }
	int GetPoints() { return mPoints; }
	int GetTowerLevel() { return mTowerLevel; }
	int GetLevel() { return mLevel; }

	void ChangeEnergy( int energy ) { mPlayerEnergy += energy; }
	void ChangePoints( int points ) { mPoints += points; }
	void TowerLevelUp() { mTowerLevel++; mTowerPrice += 2; }
	void LevelUp() { mLevel++; }

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
	static ScoresManager& getSingleton(void);
	static ScoresManager* getSingletonPtr(void);
	

private:

	int mPlayerEnergy;
	int mTowerPrice;
	int mPoints;
	int mTowerLevel;
	int mLevel;

	Ogre::Overlay* mDebugOverlay;
};
#endif // ScoresManager_h__