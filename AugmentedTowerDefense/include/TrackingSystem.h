#ifndef TrackingSystem_h__
#define TrackingSystem_h__
#pragma once



struct Marker
{
	Marker();
	Marker(const Ogre::Matrix4& _trans, int _id);

	int id;
	Ogre::Matrix4 trans;
};

class TrackingSystem
{
	public:
		TrackingSystem();
		virtual ~TrackingSystem();

		void init(int _width, int _height);

		bool update(const Ogre::PixelBox& grayLevelFrame); //return true if pose is computed

		bool isPoseComputed() const;
		Ogre::Vector3 getTranslation() const;
		Ogre::Quaternion getOrientation() const;

		const std::vector<Marker> getMarkersInfo() const;
		const std::vector<int>    getVisibleMarkersId() const;
		const std::vector<Marker> getVisibleMarkers() const;
		const Marker getSingleMarkerFromList(int* ids, int count) const;
		
		void printMarkersInfo();

		static std::string configFilename;
		static std::string calibrationFilename;		
		static bool isUsingFullResImage;
		static bool isUsingHistory;
		static bool isUsingAutoThreshold;
		static int threshold;
		static ARFloat individualMarkersWidth;
		static ARFloat individualMarkersCenter[2];


		bool mSimulate;

	protected:		

		void convertPoseToOgreCoordinate();		
		Ogre::Matrix4 convert(const ARFloat _trans[3][4]) const;
		void Simulate();

		Ogre::Quaternion mRot180Z;					
		ARToolKitPlus::TrackerMultiMarker *mTrackerMulti;
		bool mMarkersFound;
		bool mInitialized;
		

		Ogre::Vector3     mPosition;
		Ogre::Quaternion  mOrientation;
		bool              mPoseComputed;
};
#endif // TrackingSystem_h__
