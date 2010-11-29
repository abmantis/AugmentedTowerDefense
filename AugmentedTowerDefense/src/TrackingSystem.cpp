#include "StdAfx.h"

#include "TrackingSystem.h"

#include "ARToolKitPlus/TrackerMultiMarkerImpl.h"
#include "DebugStuff.h"


using namespace std;
using namespace Ogre;

Marker::Marker()
{
	id = 0;
	trans = Ogre::Matrix4::IDENTITY;
}
Marker::Marker(const Ogre::Matrix4& _trans, int _id)
{
	id    = _id;
	trans = _trans;
}

std::string TrackingSystem::configFilename      = "ar_config.cfg";
std::string TrackingSystem::calibrationFilename = "ar_calib.cal";
bool TrackingSystem::isUsingFullResImage        = true;
bool TrackingSystem::isUsingHistory             = true;
bool TrackingSystem::isUsingAutoThreshold       = true;
int TrackingSystem::threshold                   = 140;

TrackingSystem::TrackingSystem()
: mRot180Z(Degree(180.f), Vector3::UNIT_Z)
{
	mInitialized = false;
	mMarkersFound = false;

	mTrackerMulti = NULL;
}

TrackingSystem::~TrackingSystem()
{
	delete mTrackerMulti;
}

void TrackingSystem::init(int _width, int _height)
{
	mTrackerMulti = new ARToolKitPlus::TrackerMultiMarkerImpl<6, 6, 6, 1, 8>(_width, _height);

	//
	mTrackerMulti->setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_BGR); //PIXEL_FORMAT_LUM

	//
	if(!mTrackerMulti->init(TrackingSystem::calibrationFilename.c_str(), TrackingSystem::configFilename.c_str(), 5.0f, 50000.0f))
		throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE, "Init failed : calibration file not bFound", "MultiTracker");

	//Set Marker border size : thin = 0.125f & large = 0.250f
	mTrackerMulti->setBorderWidth(0.125f);

    mTrackerMulti->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);
	mTrackerMulti->setMarkerMode(ARToolKitPlus::MARKER_ID_SIMPLE);
	mTrackerMulti->changeCameraSize(_width, _height);

	//Set image full res analysis on or off
	if (TrackingSystem::isUsingFullResImage)
		mTrackerMulti->setImageProcessingMode(ARToolKitPlus::IMAGE_FULL_RES);

	//Set History on or off
	mTrackerMulti->setUseDetectLite(!TrackingSystem::isUsingHistory);
	
	//Set Threshold value or use auto-thresholding
	mTrackerMulti->activateAutoThreshold(TrackingSystem::isUsingAutoThreshold);	
	if (!TrackingSystem::isUsingAutoThreshold)
		mTrackerMulti->setThreshold(TrackingSystem::threshold);

	mInitialized = true;
}

bool TrackingSystem::update(const Ogre::PixelBox& frame)
{
	if (!mInitialized)
		return false;

	//calc() method return the number of markers bFound
	bool bFound = mTrackerMulti->calc((unsigned char*)frame.data) != 0;
	
	if (bFound)
	{		
		convertPoseToOgreCoordinate();
	}

	mPoseComputed = bFound;
	return bFound;
}

void TrackingSystem::convertPoseToOgreCoordinate() 
{
	const ARToolKitPlus::ARMultiMarkerInfoT* config = mTrackerMulti->getMultiMarkerConfig();	
	// Convert the AR matrix to an OGRE matrix and then compute the camera 
	// position relative to the markers system 
	Matrix4 invTrans = convert(config->trans).inverseAffine();
	

	Vector3 invTransPosition = invTrans.getTrans();
	Quaternion invTransOrientation = invTrans.extractQuaternion();	
	invTransOrientation = invTransOrientation * mRot180Z;	
		
	mTranslation = invTransPosition;
	mOrientation = invTransOrientation;	
}

const std::vector<int> TrackingSystem::getVisibleMarkersId() const
{
	std::vector<int> ids;
	
	int* markersIds;
	mTrackerMulti->getDetectedMarkers(markersIds);
	for (int i=0; i<mTrackerMulti->getNumDetectedMarkers(); ++i)
		ids.push_back(markersIds[i]);

	return ids;
}

const std::vector<Marker> TrackingSystem::getMarkersInfo() const
{
	std::vector<Marker> markers;
	const ARToolKitPlus::ARMultiMarkerInfoT* config = mTrackerMulti->getMultiMarkerConfig();
	for (int i=0; i<config->marker_num; ++i)
	{
		int id = config->marker[i].patt_id;
		Matrix4 trans = convert(config->marker[i].trans);
		markers.push_back(Marker(trans, id));
	}

	return markers;
}

void TrackingSystem::printMarkersInfo()
{
	std::cout << std::endl << std::endl;
	const ARToolKitPlus::ARMultiMarkerInfoT* config = mTrackerMulti->getMultiMarkerConfig();
	for (int i=0; i<config->marker_num; ++i)
	{
		int id = config->marker[i].patt_id;
		Matrix4 trans = convert(config->marker[i].trans);
		HelperClass::Print(trans, HelperClass::ToString(id));
	}
}

Ogre::Matrix4 TrackingSystem::convert(const ARFloat _trans[3][4]) const
{
	Ogre::Matrix4 m;
	for (int i = 0; i<3; i++)
		for (int j=0; j<4; j++)        
            m[i][j] = _trans[i][j];
    m[3][0] = m[3][1] = m[3][2] = 0.0;
    m[3][3] = 1.0;

	return m;
}

Ogre::Vector3 TrackingSystem::getTranslation() const
{
	return mTranslation;
}

Ogre::Quaternion TrackingSystem::getOrientation() const
{
	return mOrientation;
}

bool TrackingSystem::isPoseComputed() const
{
	return mPoseComputed;
}