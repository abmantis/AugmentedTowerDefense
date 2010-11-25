#include "StdAfx.h"
#include "DebugStuff.h"

DebugStuff::DebugStuff(void)
{
}

DebugStuff::~DebugStuff(void)
{
}

void DebugStuff::CreateAxis( Ogre::SceneManager *sceneMgr )
{
	Ogre::ManualObject* myManualObject =  sceneMgr->createManualObject("manual1"); 
	Ogre::SceneNode* myManualObjectNode = sceneMgr->getRootSceneNode()->createChildSceneNode("manual1_node"); 

	Ogre::MaterialPtr myManualObjectMaterial = Ogre::MaterialManager::getSingleton().create("manual1Material", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); 
	myManualObjectMaterial->setReceiveShadows(false); 
	myManualObjectMaterial->getTechnique(0)->setLightingEnabled(false); 
	


	myManualObject->begin("manual1Material", Ogre::RenderOperation::OT_LINE_LIST); 
	myManualObject->position(0, 0, 0); 
	myManualObject->colour(1,0,0);
	myManualObject->position(10, 0, 0); 
//	myManualObject->colour(1,0,0);

	myManualObject->position(0, 0, 0); 
	myManualObject->colour(0,1,0);
	myManualObject->position(0, 10, 0);
//	myManualObject->colour(0,1,0);
	
	myManualObject->position(0, 0, 0); 
	myManualObject->colour(0,0,1);
	myManualObject->position(0, 0, 10); 
//	myManualObject->colour(0,0,1);

	myManualObject->end(); 

	myManualObjectNode->attachObject(myManualObject);
	myManualObjectNode->setPosition(0,0,0);
}
