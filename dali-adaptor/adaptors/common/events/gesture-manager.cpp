/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// CLASS HEADER
#include "gesture-manager.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <events/gesture-detector.h>
#include <events/long-press-gesture-detector.h>
#include <events/pan-gesture-detector.h>
#include <events/pinch-gesture-detector.h>
#include <events/tap-gesture-detector.h>
#include <base/core-event-interface.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{

#if defined(DEBUG_ENABLED)
Integration::Log::Filter* gLogFilter  = Integration::Log::Filter::New( Debug::NoLogging, false, "LOG_GESTURE_MANAGER" );

/**
 * Helper method to return the string representation of a gesture type.
 */
const char * GetGestureTypeString( Gesture::Type type )
{
  static const char * const pinch( "Pinch" );
  static const char * const pan( "Pan" );
  static const char * const tap( "tap" );
  static const char * const longPress( "LongPress" );
  static const char * const invalid( "Invalid" );

  const char * retVal( NULL );

  switch ( type )
  {
    case Gesture::LongPress:
    {
      retVal = longPress;
      break;
    }

    case Gesture::Pan:
    {
      retVal = pan;
      break;
    }

    case Gesture::Pinch:
    {
      retVal = pinch;
      break;
    }

    case Gesture::Tap:
    {
      retVal = tap;
      break;
    }

    default:
      retVal = invalid;
      break;
  }

  return retVal;
};
#endif // DEBUG_ENABLED

const float MINIMUM_DISTANCE_DELTA_DIVISOR = 85.0f;
} // unnamed namespace

GestureManager::GestureManager(CoreEventInterface& coreEventInterface, Vector2 screenSize,CallbackManager* callbackManager, EnvironmentOptions& environmentOptions)
: mCoreEventInterface( coreEventInterface ),
  mScreenSize( screenSize ),
  mCallbackManager( callbackManager ),
  mEnvironmentOptions( environmentOptions ),
  mMinimumDistanceDelta(-1.0f),
  mRunning( true ) // This allows gestures to be created before Adaptor::Start() is called e.g. by Indicator
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "Creating GestureManager\n" );
}

GestureManager::~GestureManager()
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "Destroying GestureManager\n" );
}

void GestureManager::SendEvent(const Integration::TouchEvent& event)
{
  if (mRunning)
  {
    DALI_LOG_INFO( gLogFilter, Debug::Verbose, "SendEvent: START\n" );

    // gestures can be added / deleted during SendEvent so we make a copy of the container.
    // the gestures are reference counted, so unused gesture detectors will be deleted when
    // the local variable detectors goes out of scope.
    GestureDetectorContainer detectors( mGestureDetectors );

    // Send the event to all gesture detectors.
    for ( GestureDetectorContainer::iterator iter = detectors.begin(), endIter = detectors.end(); iter != endIter; ++iter )
    {
      (*iter)->SendEvent(event);
    }
    DALI_LOG_INFO( gLogFilter, Debug::Verbose, "SendEvent: END\n" );
  }
}

void GestureManager::Stop()
{
  if (mRunning)
  {
    DALI_LOG_INFO( gLogFilter, Debug::Verbose, "Stop\n" );

    mGestureDetectors.clear();
    mRunning = false;
  }
}

void GestureManager::SetMinimumPinchDistance(float distance)
{
  mMinimumDistanceDelta = distance;
  for( GestureDetectorContainer::iterator iter = mGestureDetectors.begin(), endIter = mGestureDetectors.end(); iter != endIter; ++iter )
  {
    if ( ( *iter )->GetType() == Gesture::Pinch )
    {
      DALI_LOG_INFO( gLogFilter, Debug::General, "Set Minimum Pinch Distance: %f\n", distance );
      PinchGestureDetector* gestureDetector = static_cast<PinchGestureDetector*>(iter->Get());
      gestureDetector->SetMinimumPinchDistance(distance);
      break;
    }
  }
}

void GestureManager::Register(const Integration::GestureRequest& request)
{
  if (mRunning)
  {
    DALI_LOG_INFO( gLogFilter, Debug::General, "Creating %s Detector\n", GetGestureTypeString( request.type ) );

    switch (request.type)
    {
      case Gesture::LongPress:
      {
        GestureDetectorPtr gesture = new LongPressGestureDetector(mCoreEventInterface, mScreenSize, static_cast<const Integration::LongPressGestureRequest&>(request));
        mGestureDetectors.push_back(gesture);
        break;
      }

      case Gesture::Pan:
      {
        GestureDetectorPtr gesture = new PanGestureDetector(mCoreEventInterface, mScreenSize, static_cast<const Integration::PanGestureRequest&>(request), mEnvironmentOptions);
        mGestureDetectors.push_back(gesture);
        break;
      }

      case Gesture::Pinch:
      {
        float minPinchDistance = mMinimumDistanceDelta >= 0.0f ? mMinimumDistanceDelta : (mScreenSize.height / MINIMUM_DISTANCE_DELTA_DIVISOR);
        GestureDetectorPtr gesture = new PinchGestureDetector(mCoreEventInterface, mScreenSize, minPinchDistance);
        mGestureDetectors.push_back(gesture);
        break;
      }

      case Gesture::Tap:
      {
        GestureDetectorPtr gesture = new TapGestureDetector(mCoreEventInterface, mScreenSize, static_cast<const Integration::TapGestureRequest&>(request));
        mGestureDetectors.push_back(gesture);
        break;
      }

      default:
        DALI_ASSERT_DEBUG(false);
        break;
    }
  }
}

void GestureManager::Unregister(const Integration::GestureRequest& request)
{
  if ( mRunning )
  {
    DALI_LOG_INFO( gLogFilter, Debug::General, "Unregister: %s\n", GetGestureTypeString( request.type ) );

    DeleteGestureDetector( request.type );

  }
}

void GestureManager::Update(const Integration::GestureRequest& request)
{
  for( GestureDetectorContainer::iterator iter = mGestureDetectors.begin(), endIter = mGestureDetectors.end(); iter < endIter; ++iter )
  {
    if ( (*iter)->GetType() == request.type )
    {
      DALI_LOG_INFO( gLogFilter, Debug::General, "Update: %s\n", GetGestureTypeString( request.type ) );
      (*iter)->Update( request );
      break;
    }
  }
}

void GestureManager::DeleteGestureDetector( Gesture::Type type )
{
  for( GestureDetectorContainer::iterator iter = mGestureDetectors.begin(), endIter = mGestureDetectors.end(); iter != endIter; ++iter )
  {
    if ( ( *iter )->GetType() == type )
    {
      DALI_LOG_INFO( gLogFilter, Debug::General, "DeleteGestureDetector: %s\n", GetGestureTypeString( type ) );
      mGestureDetectors.erase( iter );
      break;
    }
  }
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
