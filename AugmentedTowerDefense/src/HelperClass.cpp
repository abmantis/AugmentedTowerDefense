#include "StdAfx.h"
#include "HelperClass.h"

HelperClass::HelperClass(void)
{
}

HelperClass::~HelperClass(void)
{
}

void HelperClass::CreateLineAxis( Ogre::SceneManager *sceneMgr )
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

void HelperClass::CreateAxis( Ogre::SceneManager *sceneMgr )
{
	Ogre::Entity* ent = sceneMgr->createEntity("axes.mesh");	//1x1_cube.mesh //Sinbad.mesh //axes.mesh
	Ogre::Real scale = 5;
	Ogre::SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode("Axes");
	node->setPosition(0, 0, 0);
	node->setScale(Ogre::Vector3::UNIT_SCALE*scale);
	node->attachObject(ent);
}

void HelperClass::Print( Ogre::Vector3 vector3, std::string prefix /*= ""*/, std::string sufix /*= "\n"*/ )
{
	std::cout << prefix << vector3.x << ";" << vector3.y << ";" << vector3.z << sufix;
}

void HelperClass::Print( Ogre::Matrix4 matrix4, std::string prefix /*= ""*/, std::string sufix /*= "\n"*/ )
{
	std::cout << prefix << std::endl;

	for(int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout  << matrix4[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << sufix;
}

std::string HelperClass::ToString( int iVal )
{
	std::stringstream st;
	st << iVal;
	return st.str();
}

void HelperClass::CreateLine( Ogre::SceneManager *sceneMgr, Ogre::Vector3 start, Ogre::Vector3 end )
{
	Ogre::ManualObject* myManualObject =  sceneMgr->createManualObject(); 
	Ogre::SceneNode* myManualObjectNode = sceneMgr->getRootSceneNode()->createChildSceneNode(); 

	Ogre::MaterialPtr myManualObjectMaterial = Ogre::MaterialManager::getSingleton().create("manual1Material", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); 
	myManualObjectMaterial->setReceiveShadows(false); 
	myManualObjectMaterial->getTechnique(0)->setLightingEnabled(false); 



	myManualObject->begin("manual1Material", Ogre::RenderOperation::OT_LINE_LIST); 
	myManualObject->position(start); 
	myManualObject->position(end); 
	myManualObject->end(); 

	myManualObjectNode->attachObject(myManualObject);
	myManualObjectNode->setPosition(0,0,0);
}

void HelperClass::DestroyAllAttachedMovableObjects( Ogre::SceneNode* pNode )
{
	if ( pNode == NULL)
	{
		assert( pNode != NULL );
		return;
	}

	// Destroy all the attached objects
	Ogre::SceneNode::ObjectIterator itObject = pNode->getAttachedObjectIterator();

	while ( itObject.hasMoreElements() )
	{
		Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(itObject.getNext());
		pNode->getCreator()->destroyMovableObject( pObject );
	}

	// Recurse to child SceneNodes
	Ogre::SceneNode::ChildNodeIterator itChild = pNode->getChildIterator();

	while ( itChild.hasMoreElements() )
	{
		Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
		DestroyAllAttachedMovableObjects( pChildNode );
	}
}


