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
#include "tap-gesture-detector.h"

// EXTERNAL INCLUDES
#include <cmath>

#include <dali/public-api/math/vector2.h>

#include <dali/integration-api/events/gesture-requests.h>
#include <dali/integration-api/events/touch-event-integ.h>
#include <base/core-event-interface.h>

// INTERNAL INCLUDES

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{
// TODO: Set these according to DPI
const float MAXIMUM_MOTION_ALLOWED = 20.0f;
const unsigned long MAXIMUM_TIME_ALLOWED = 500u;
} // unnamed namespace

TapGestureDetector::TapGestureDetector(CoreEventInterface& coreEventInterface, Vector2 screenSize, const Integration::TapGestureRequest& request)
: GestureDetector(screenSize, Gesture::Tap),
  mCoreEventInterface(coreEventInterface),
  mState(Clear),
  mMinimumTapsRequired(request.minTaps),
  mMaximumTapsRequired(request.maxTaps),
  mTapsRegistered(0),
  mTouchPosition(),
  mTouchTime(0u),
  mLastTapTime(0u)
{
}

TapGestureDetector::~TapGestureDetector()
{
}

void TapGestureDetector::SendEvent(const Integration::TouchEvent& event)
{
  if (event.GetPointCount() == 1)
  {
    const TouchPoint& point = event.points[0];
    TouchPoint::State pointState = point.state;

    switch (mState)
    {
      case Clear:
      {
        if (pointState == TouchPoint::Down)
        {
          SetupForTouchDown( event, point );
        }
        break;
      }

      case Touched:
      {
        unsigned long deltaBetweenTouchDownTouchUp = abs( event.time - mTouchTime ) ;

        if ( pointState == TouchPoint::Up )
        {
          if ( deltaBetweenTouchDownTouchUp < MAXIMUM_TIME_ALLOWED )
          {
            mLastTapTime = mTouchTime;
            EmitSingleTap( event.time, point );
            mState = Registered;
          }
          else
          {
            mState = Clear;
          }
        }
        else if (pointState == TouchPoint::Interrupted)
        {
          mState = Clear;
        }
        break;
      }

      case Registered:
      {
        if ( pointState == TouchPoint::Up )
        {
          unsigned long deltaBetweenTouchDownTouchUp = abs( event.time - mTouchTime ) ;

          if ( deltaBetweenTouchDownTouchUp < MAXIMUM_TIME_ALLOWED )
          {
            // This is a possible multiple tap, so has it been quick enough ?
            unsigned long timeDelta = abs( event.time - mLastTapTime );
            if ( timeDelta > MAXIMUM_TIME_ALLOWED ) // If exceeded time between taps then just a single tap.
            {
              mLastTapTime = event.time;
              EmitSingleTap( event.time, point );
              mState = Registered;
            }
            else
            {
              ++mTapsRegistered;
              EmitGesture( Gesture::Started, event.time );
              mState = Clear;
            }
          }
          else // Delta between touch down and touch up too long to be considered a Tap event
          {
            mState = Clear;
          }
        }
        else if (pointState == TouchPoint::Down)
        {
          Vector2 distanceDelta(abs(mTouchPosition.x - point.screen.x),
                                abs(mTouchPosition.y - point.screen.y));

          unsigned long timeDelta = abs( event.time - mLastTapTime );

          if (distanceDelta.x > MAXIMUM_MOTION_ALLOWED ||
              distanceDelta.y > MAXIMUM_MOTION_ALLOWED ||
              timeDelta > MAXIMUM_TIME_ALLOWED )
          {
            SetupForTouchDown( event, point );
          }
          else
          {
            EmitPossibleState( event );
          }
        }
        break;
      }

      case Failed:
      default:
      {
        mState = Clear;
        break;
      }
    }
  }
  else
  {
    mState = Failed;

    // We have entered a multi-touch event so emit registered gestures if required.
    EmitGesture(Gesture::Started, event.time);
  }
}

void TapGestureDetector::SetupForTouchDown( const Integration::TouchEvent& event, const TouchPoint& point )
{
  mTouchPosition.x = point.screen.x;
  mTouchPosition.y = point.screen.y;
  mTouchTime = event.time;
  mLastTapTime = 0u;
  mTapsRegistered = 0;
  mState = Touched;
  EmitPossibleState( event );
}

void TapGestureDetector::EmitPossibleState( const Integration::TouchEvent& event )
{
  Integration::TapGestureEvent tapEvent( Gesture::Possible );
  tapEvent.point = mTouchPosition;
  tapEvent.time = event.time;
  mCoreEventInterface.QueueCoreEvent(tapEvent);
}


void TapGestureDetector::Update(const Integration::GestureRequest& request)
{
  const Integration::TapGestureRequest& tap = static_cast<const Integration::TapGestureRequest&>(request);

  mMinimumTapsRequired = tap.minTaps;
  mMaximumTapsRequired = tap.maxTaps;
}

void TapGestureDetector::EmitGesture( Gesture::State state, unsigned int time )
{
  if ( (state == Gesture::Cancelled) ||
       (mTapsRegistered >= mMinimumTapsRequired && mTapsRegistered <= mMaximumTapsRequired) )

  {
    Integration::TapGestureEvent event( state );
    EmitTap( time, event );
  }
}

void TapGestureDetector::EmitSingleTap( unsigned int time, const TouchPoint& point )
{
  Integration::TapGestureEvent event( Gesture::Started );
  Vector2 distanceDelta(abs(mTouchPosition.x - point.screen.x),
                        abs(mTouchPosition.y - point.screen.y));

  if (distanceDelta.x > MAXIMUM_MOTION_ALLOWED ||
      distanceDelta.y > MAXIMUM_MOTION_ALLOWED )
  {
    event.state = Gesture::Cancelled;
  }
  mTapsRegistered = 1u;
  EmitTap( time, event );
}

void TapGestureDetector::EmitTap( unsigned int time, Integration::TapGestureEvent& event )
{
  event.numberOfTaps = mTapsRegistered;
  event.point = mTouchPosition;
  event.time = time;
  mCoreEventInterface.QueueCoreEvent(event);
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
