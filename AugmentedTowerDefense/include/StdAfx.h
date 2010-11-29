#ifndef StdAfx_h__
#define StdAfx_h__
#pragma once

#define _CRT_SECURE_NO_WARNINGS

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <videoInput.h>

#include <ARToolKitPlus/TrackerMultiMarker.h>

#include <Ogre.h>
#include <OgreQuaternion.h>
#include <OgreException.h>
#include <OgrePixelFormat.h>
#include <OgreTextureManager.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgrePrerequisites.h>
#include <OgreStringVector.h>
#include <OgreSingleton.h>
#include <OgreMatrix4.h>

#include <OIS/OIS.h>

#endif // StdAfx_h__

