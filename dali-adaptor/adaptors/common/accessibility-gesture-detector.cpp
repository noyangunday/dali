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
#include "accessibility-gesture-detector.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/events/gesture-requests.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

AccessibilityGestureDetector::AccessibilityGestureDetector()
: PanGestureDetectorBase(Vector2::ZERO, Integration::PanGestureRequest(), NULL),
  mGestureHandler(NULL),
  mPanning(false)
{
}

AccessibilityGestureDetector::~AccessibilityGestureDetector()
{
}

void AccessibilityGestureDetector::SetGestureHandler(AccessibilityGestureHandler& handler)
{
  mGestureHandler = &handler;
}

void AccessibilityGestureDetector::EmitPan(const Integration::PanGestureEvent gesture)
{
  if( mGestureHandler )
  {
    if(gesture.state == Gesture::Started)
    {
      mPanning = true;
    }

    if( mPanning )
    {
      mGestureHandler->HandlePanGesture(gesture);

      if( (gesture.state == Gesture::Finished) ||
          (gesture.state == Gesture::Cancelled) )
      {
        mPanning = false;
      }
    }
  }
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
