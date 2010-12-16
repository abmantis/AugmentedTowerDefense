#include "StdAfx.h"
#include "SharedStuff.h"


AugmentedTowerDefense::HelperClass::HelperClass(void)
{
}

AugmentedTowerDefense::HelperClass::~HelperClass(void)
{
}

void AugmentedTowerDefense::HelperClass::CreateLineAxis( Ogre::SceneManager *sceneMgr )
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

void AugmentedTowerDefense::HelperClass::CreateAxis( Ogre::SceneManager *sceneMgr )
{
	Ogre::Entity* ent = sceneMgr->createEntity("axes.mesh");	//1x1_cube.mesh //Sinbad.mesh //axes.mesh
	Ogre::Real scale = 5;
	Ogre::SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode("Axes");
	node->setPosition(0, 0, 0);
	node->setScale(Ogre::Vector3::UNIT_SCALE*scale);
	node->attachObject(ent);
}

void AugmentedTowerDefense::HelperClass::Print( Ogre::Vector3 vector3, std::string prefix /*= ""*/, std::string sufix /*= "\n"*/ )
{
	std::cout << prefix << ToString(vector3) << sufix;
}

void AugmentedTowerDefense::HelperClass::Print( Ogre::Matrix4 matrix4, std::string prefix /*= ""*/, std::string sufix /*= "\n"*/ )
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

std::string AugmentedTowerDefense::HelperClass::ToString( int iVal )
{
	std::stringstream st;
	st << iVal;
	return st.str();
}

std::string AugmentedTowerDefense::HelperClass::ToString( Ogre::Vector3 vector3 )
{
	std::string str = "";
	std::stringstream st;
	
	st << vector3.x;
	str = st.str() + "; ";
	
	st.clear();
	st << vector3.y;
	str += st.str() + "; ";
	
	st.clear();
	st << vector3.z;
	str += st.str();
	
	return str;
}

std::string AugmentedTowerDefense::HelperClass::ToString( Ogre::Vector2 vector2 )
{
	std::string str = "";
	std::stringstream st;

	st << vector2.x;
	str = st.str() + "; ";

	st.clear();
	st << vector2.y;
	str += st.str();

	return str;
}

void AugmentedTowerDefense::HelperClass::CreateLine( Ogre::SceneManager *sceneMgr, Ogre::Vector3 start, Ogre::Vector3 end )
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

void AugmentedTowerDefense::HelperClass::DestroyAllAttachedMovableObjects( Ogre::SceneNode* pNode )
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

void AugmentedTowerDefense::HelperClass::DoSafeRotation( Ogre::SceneNode *pNode, Ogre::Vector3 src, Ogre::Vector3 direction )
{
	if ((1.0f + src.dotProduct(direction)) < 0.0001f) 
	{
		pNode->yaw(Ogre::Degree(180));
	}
	else
	{
		Ogre::Quaternion quat = src.getRotationTo(direction);
		pNode->rotate(quat);
	}
}