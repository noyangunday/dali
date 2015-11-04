#ifndef __DALI_ACCESSIBILITY_GESTURE_HANDLER_H__
#define __DALI_ACCESSIBILITY_GESTURE_HANDLER_H__

/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
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

// EXTERNAL INCLUDES
#include <dali/integration-api/events/pan-gesture-event.h>

namespace Dali
{

/**
 * AccessibilityGestureHandler is an interface used by Dali to handle accessibility gestures
 * passed by the accessibility manager.
 */
class AccessibilityGestureHandler
{
public:

  /**
   * Handle the accessibility pan gesture.
   * @param[in]  panEvent  The pan event to be handled.
   * @return whether the gesture is handled successfully or not.
   */
  virtual bool HandlePanGesture( const Integration::PanGestureEvent& panEvent ) = 0;

}; // class AccessibilityGestureHandler

} // namespace Dali

#endif // __DALI_ACCESSIBILITY_GESTURE_HANDLER_H__
