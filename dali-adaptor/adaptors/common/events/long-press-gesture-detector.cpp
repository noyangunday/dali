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
#include "long-press-gesture-detector.h"

// EXTERNAL INCLUDES
#include <cmath>

#include <dali/public-api/events/touch-point.h>
#include <dali/public-api/math/vector2.h>

#include <dali/integration-api/events/gesture-requests.h>
#include <dali/integration-api/events/long-press-gesture-event.h>
#include <dali/integration-api/events/touch-event-integ.h>

#include <system-settings.h>

// INTERNAL INCLUDES
#include <base/core-event-interface.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{
// TODO: Set these according to DPI
const float MAXIMUM_MOTION_ALLOWED = 60.0f;
// TODO: Set this time according to system setting (vconf)
const unsigned long LONG_PRESS_TIME = 500u;
} // unnamed namespace

LongPressGestureDetector::LongPressGestureDetector(CoreEventInterface& coreEventInterface, Vector2 screenSize, const Integration::LongPressGestureRequest& request)
: GestureDetector(screenSize, Gesture::LongPress),
  mCoreEventInterface(coreEventInterface),
  mState(Clear),
  mMinimumTouchesRequired(request.minTouches),
  mMaximumTouchesRequired(request.maxTouches),
  mTouchTime(0),
  mTimerSlot( this )
{
  mTimer = Dali::Timer::New(GetSystemValue());
  mTimer.TickSignal().Connect( mTimerSlot, &LongPressGestureDetector::TimerCallback );
}

LongPressGestureDetector::~LongPressGestureDetector()
{
}

void LongPressGestureDetector::SendEvent(const Integration::TouchEvent& event)
{
  unsigned int pointCount( event.GetPointCount() );

  switch (mState)
  {
    // Clear: Wait till one point touches the screen before starting timer.
    case Clear:
    {
      const TouchPoint& point = event.points[0];

      if ( point.state == TouchPoint::Down )
      {
        mTouchPositions.clear();
        mTouchPositions[point.deviceId] = point.screen;

        mTouchTime = event.time;

        mTimer.SetInterval(GetSystemValue());
        mTimer.Start();

        // A long press gesture may be possible, tell Core about this and change state to Touched.
        mState = Touched;
        EmitGesture( Gesture::Possible );
      }

      break;
    }

    // Touched: Monitor movement and addition/removal of points.
    case Touched:
    {
      if (pointCount > mMaximumTouchesRequired)
      {
        // A long press did not occur, tell Core that it was cancelled and change state to Failed.
        EmitGesture( Gesture::Cancelled );
        mTouchPositions.clear();
        mTimer.Stop();
        mState = Failed;
        break;
      }

      bool endLoop(false);

      for (std::vector<TouchPoint>::const_iterator iter = event.points.begin(), endIter = event.points.end();
           iter != endIter && !endLoop; ++iter)
      {
        switch( iter->state )
        {
          // add point.
          case TouchPoint::Down:
          {
            mTouchPositions[iter->deviceId] = iter->screen;
            break;
          }

          // remove point.
          case TouchPoint::Up:
          case TouchPoint::Interrupted:
          {
            // System has interrupted us, long press is not possible, inform Core
            EmitGesture( Gesture::Cancelled );
            mTouchPositions.clear();
            mTimer.Stop();
            mState = ( pointCount == 1 ) ? Clear : Failed; // Change state to Clear if only one point, Failed otherwise.
            endLoop = true;
            break;
          }

          case TouchPoint::Motion:
          {
            const Vector2 touchPosition( mTouchPositions[iter->deviceId] - iter->screen );
            float distanceSquared = touchPosition.LengthSquared();

            if (distanceSquared > ( MAXIMUM_MOTION_ALLOWED * MAXIMUM_MOTION_ALLOWED ) )
            {
              // We have moved more than the allowable motion for a long press gesture. Inform Core and change state to Failed.
              EmitGesture( Gesture::Cancelled );
              mTimer.Stop();
              mState = Failed;
              endLoop = true;
            }
            break;
          }

          case TouchPoint::Stationary:
          case TouchPoint::Leave:
          case TouchPoint::Last:
          {
            break;
          }
        }
      }
      break;
    }

    // Failed/Finished: Monitor the touches, waiting for all touches to be released.
    case Failed:
    case Finished:
    {
      // eventually the final touch point will be removed, marking the end of this gesture.
      if ( pointCount == 1 )
      {
        TouchPoint::State primaryPointState = event.points[0].state;

        if ( (primaryPointState == TouchPoint::Up) || (primaryPointState == TouchPoint::Interrupted) )
        {
          if(mState == Finished)
          {
            // When the last touch point is lifted, we should inform the Core that the Long press has finished.
            EmitGesture(Gesture::Finished);
          }
          mTouchPositions.clear();
          mState = Clear; // Reset state to clear when last touch point is lifted.
        }
      }
      break;
    }
  }
}

void LongPressGestureDetector::Update(const Integration::GestureRequest& request)
{
  const Integration::LongPressGestureRequest& longPress = static_cast<const Integration::LongPressGestureRequest&>(request);

  mMinimumTouchesRequired = longPress.minTouches;
  mMaximumTouchesRequired = longPress.maxTouches;
}

bool LongPressGestureDetector::TimerCallback()
{
  EmitGesture(Gesture::Started);

  mState = Finished;

  // There is no touch event at this time, so ProcessEvents must be called directly
  mCoreEventInterface.ProcessCoreEvents();

  return false;
}

void LongPressGestureDetector::EmitGesture(Gesture::State state)
{
  unsigned int touchPoints ( mTouchPositions.size() );

  // We should tell Core about the Possible and Cancelled states regardless of whether we have satisfied long press requirements.
  if ( (state == Gesture::Possible) ||
       (state == Gesture::Cancelled) ||
       (touchPoints >= mMinimumTouchesRequired) )
  {
    Integration::LongPressGestureEvent longPress( state );
    longPress.numberOfTouches = touchPoints;

    for (std::map<int, Vector2>::iterator iter = mTouchPositions.begin(), endIter = mTouchPositions.end();
         iter != endIter; ++iter)
    {
      longPress.point += iter->second;
    }
    longPress.point /= touchPoints;

    longPress.time = mTouchTime;
    if ( state != Gesture::Possible )
    {
      longPress.time += GetSystemValue();
    }

    mCoreEventInterface.QueueCoreEvent(longPress);
  }
}

int LongPressGestureDetector::GetSystemValue()
{
  return GetLongPressTime( LONG_PRESS_TIME );
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
