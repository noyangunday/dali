#ifndef __DALI_INTERNAL_ACCESSIBILITY_GESTURE_DETECTOR_H__
#define __DALI_INTERNAL_ACCESSIBILITY_GESTURE_DETECTOR_H__

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

// EXTERNAL INCLUDES

// INTERNAL INCLUDES
#include <events/pan-gesture-detector-base.h>
#include <adaptor-impl.h>
#include <accessibility-gesture-handler.h>

namespace Dali
{

namespace Integration
{
class Core;
struct TouchEvent;
struct PanGestureRequest;
}

namespace Internal
{

namespace Adaptor
{

/**
 * Detects an accessibility pan gesture and sends it to the gesture handler.
 */
class AccessibilityGestureDetector : public PanGestureDetectorBase
{
public:

  /**
   * Constructor
   */
  AccessibilityGestureDetector();

  /**
   * Virtual destructor.
   */
  virtual ~AccessibilityGestureDetector();

  /**
   * Set the handler to handle accessibility gestures.
   * @param[in] handler The Accessibility gesture handler.
   * @note Handlers should remove themselves when they are destroyed.
   */
  void SetGestureHandler(AccessibilityGestureHandler& handler);

private:

  /**
   * Emits the pan gesture event to the gesture handler.
   * @param[in] gesture The pan gesture event.
   */
  virtual void EmitPan(const Integration::PanGestureEvent gesture);

private:

  AccessibilityGestureHandler* mGestureHandler; ///< The pointer of accessibility gesture handler
  bool mPanning;    ///< Keep track of panning state, when panning is occuring, this is true.
};

typedef IntrusivePtr<AccessibilityGestureDetector> AccessibilityGestureDetectorPtr;

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_ACCESSIBILITY_GESTURE_DETECTOR_H__
