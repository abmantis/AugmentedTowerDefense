#pragma once
#include "ScoresManager.h"

class HUDManager
{
public:
	HUDManager();
	~HUDManager();

	void init();
	void update();

	void showPopup(bool show, Ogre::String text);
private:

	void updateScores();

	Ogre::OverlayManager* mOverlayManager;

	struct MessageOverlays
	{
		Ogre::Overlay* popupMessagePanel;
		Ogre::OverlayElement* popupMessageText;
	} mMessageOverlays;
	
	struct ScoresOverlays
	{
		ScoresManager* scoresManager;

		Ogre::Overlay* scoresPanelOverlay;
		Ogre::OverlayElement* guiEnergy;
		Ogre::OverlayElement* guiLevel;
		Ogre::OverlayElement* guiPoints;
		Ogre::OverlayElement* guiTowerPrice;
		Ogre::OverlayElement* guiTowerLevel;
	} mScoresOverlays;
	
};
