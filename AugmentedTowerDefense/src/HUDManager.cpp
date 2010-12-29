#include "StdAfx.h"
#include "HUDManager.h"

HUDManager::HUDManager()
{
	mOverlayManager = Ogre::OverlayManager::getSingletonPtr();
	mScoresOverlays.scoresManager = ScoresManager::getSingletonPtr();

	mScoresOverlays.scoresPanelOverlay = mOverlayManager->getByName("ATD/ScorsOverlay");
	mScoresOverlays.guiEnergy = mOverlayManager->getOverlayElement("ATD/Energy");
	mScoresOverlays.guiLevel = mOverlayManager->getOverlayElement("ATD/Level");
	mScoresOverlays.guiPoints = mOverlayManager->getOverlayElement("ATD/Points");
	mScoresOverlays.guiTowerPrice = mOverlayManager->getOverlayElement("ATD/TowerPrice");
	mScoresOverlays.guiTowerLevel = mOverlayManager->getOverlayElement("ATD/TowerLevel");

	mMessageOverlays.popupMessagePanel = mOverlayManager->getByName("ATowerDefense/PopupMessageOverlay");
	mMessageOverlays.popupMessageText = mOverlayManager->getOverlayElement("ATowerDefense/PopupMessageText");
}

HUDManager::~HUDManager()
{
}

void HUDManager::init()
{
	mScoresOverlays.scoresPanelOverlay->show();
	mMessageOverlays.popupMessagePanel->hide();
}

void HUDManager::update()
{
	updateScores();
}

void HUDManager::updateScores()
{
	mScoresOverlays.guiEnergy->setCaption(		"Energy: "			+ Ogre::StringConverter::toString(mScoresOverlays.scoresManager->GetPlayerEnergy()));
	mScoresOverlays.guiLevel->setCaption(		"Level: "			+ Ogre::StringConverter::toString(mScoresOverlays.scoresManager->GetLevel()));
	mScoresOverlays.guiPoints->setCaption(		"Points: "			+ Ogre::StringConverter::toString(mScoresOverlays.scoresManager->GetPoints()));
	mScoresOverlays.guiTowerPrice->setCaption(	"Tower Price: "		+ Ogre::StringConverter::toString(mScoresOverlays.scoresManager->GetTowerPrice()));
	mScoresOverlays.guiTowerLevel->setCaption(	"Tower Fire Rate: "	+ Ogre::StringConverter::toString(mScoresOverlays.scoresManager->GetTowerLevel()));
}

void HUDManager::showPopup( bool show, Ogre::String text )
{
	if(show) mMessageOverlays.popupMessagePanel->show();
	else mMessageOverlays.popupMessagePanel->hide();

//	mMessageOverlays.popupMessageText->show();
	mMessageOverlays.popupMessageText->setCaption(text);
}
