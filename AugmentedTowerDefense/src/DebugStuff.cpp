#include "StdAfx.h"
#include "DebugStuff.h"

DebugStuff::DebugStuff(void)
{
}

DebugStuff::~DebugStuff(void)
{
}

void DebugStuff::CreateLineAxis( Ogre::SceneManager *sceneMgr )
{
	Ogre::ManualObject* myManualObject =  sceneMgr->createManualObject("manual1"); 
	Ogre::SceneNode* myManualObjectNode = sceneMgr->getRootSceneNode()->createChildSceneNode("manual1_node"); 

	Ogre::MaterialPtr myManualObjectMaterial = Ogre::MaterialManager::getSingleton().create("manual1Material", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); 
	myManualObjectMaterial->setReceiveShadows(false); 
	myManualObjectMaterial->getTechnique(0)->setLightingEnabled(false); 
	


	myManualObject->begin("manual1Material", Ogre::RenderOperation::OT_LINE_LIST); 
	myManualObject->position(0, 0, 0); 
	myManualObject->colour(1,0,0);
	myManualObject->position(100, 0, 0); 
//	myManualObject->colour(1,0,0);

	myManualObject->position(0, 0, 0); 
	myManualObject->colour(0,1,0);
	myManualObject->position(0, 100, 0);
//	myManualObject->colour(0,1,0);
	
	myManualObject->position(0, 0, 0); 
	myManualObject->colour(0,0,1);
	myManualObject->position(0, 0, 100); 
//	myManualObject->colour(0,0,1);

	myManualObject->end(); 

	myManualObjectNode->attachObject(myManualObject);
	myManualObjectNode->setPosition(0,0,0);
}

void DebugStuff::CreateAxis( Ogre::SceneManager *sceneMgr )
{
	Ogre::Entity* ent = sceneMgr->createEntity("axes.mesh");	//1x1_cube.mesh //Sinbad.mesh //axes.mesh
	Ogre::Real scale = 5;
	Ogre::SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode("Axes");
	node->setPosition(0, 0, 0);
	node->setScale(Ogre::Vector3::UNIT_SCALE*scale);
	node->attachObject(ent);
}

void DebugStuff::Print( Ogre::Vector3 vector3, std::string prefix /*= ""*/, std::string sufix /*= "\n"*/ )
{
	std::cout << prefix << vector3.x << ";" << vector3.y << ";" << vector3.z << sufix;
}


