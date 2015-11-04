#ifndef __DALI_INTERNAL_PAN_GESTURE_DETECTOR_BASE_H__
#define __DALI_INTERNAL_PAN_GESTURE_DETECTOR_BASE_H__

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
#include <dali/public-api/common/vector-wrapper.h>
#include <dali/public-api/math/vector2.h>
#include <dali/integration-api/events/pan-gesture-event.h>

// INTERNAL INCLUDES
#include <events/gesture-detector.h>

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

class EnvironmentOptions;

/**
 * When given a set of touch events, this detector attempts to determine if a pan gesture has taken place.
 */
class PanGestureDetectorBase : public GestureDetector
{
public:

  /**
   * Virtual destructor.
   */
  virtual ~PanGestureDetectorBase();

public:

  /**
   * @copydoc Dali::Internal::GestureDetector::SendEvent(const Integration::TouchEvent&)
   */
  virtual void SendEvent(const Integration::TouchEvent& event);

  /**
   * @copydoc Dali::Internal::GestureDetector::Update(const Integration::GestureRequest&)
   */
  virtual void Update(const Integration::GestureRequest& request);

protected:

  /**
   * Constructor
   * @param[in]  screenSize  The size of the screen.
   * @param[in]  request     The details of the request.
   */
  PanGestureDetectorBase(Vector2 screenSize, const Integration::PanGestureRequest& request, EnvironmentOptions* environmentOptions);

private:

  /**
   * Emits the pan gesture event (performs some smoothing operation).
   * @param[in]  state         The state of the pan.
   * @param[in]  currentEvent  The latest touch event.
   */
  void SendPan(Gesture::State state, const Integration::TouchEvent& currentEvent);

  /**
   * Emits the pan gesture event to the core.
   * @param[in] gesture The pan gesture event.
   */
  virtual void EmitPan(const Integration::PanGestureEvent gesture) = 0;

private:

  /**
   * Internal state machine.
   */
  enum State
  {
    Clear,    ///< No gesture detected.
    Possible, ///< The current touch event data suggests that a gesture is possible.
    Started,  ///< A gesture has been detected.
    Finished, ///< A previously started pan gesture has finished.
    Failed,   ///< Current touch event data suggests a pan gesture is not possible.
  };

  State mState; ///< The current state of the detector.
  std::vector<Integration::TouchEvent> mTouchEvents;     ///< A container of all touch events after an initial down event.

  Vector2 mPrimaryTouchDownLocation;    ///< The initial touch down point.
  Vector2 mThresholdAdjustmentPerFrame; ///< The adjustment per frame at the start of a slow pan.
  Vector2 mPreviousPosition;            ///< The previous position.

  unsigned int mThresholdAdjustmentsRemaining; ///< No. of threshold adjustments still to apply (for a slow-pan).
  unsigned int mThresholdTotalAdjustments;     ///< The total number of adjustments required.

  unsigned long mPrimaryTouchDownTime;  ///< The initial touch down time.
  unsigned int mMinimumTouchesRequired; ///< The minimum touches required before a pan should be emitted.
  unsigned int mMaximumTouchesRequired; ///< The maximum touches after which a pan should not be emitted.

  unsigned int mMinimumDistanceSquared; ///< The minimum distance squared before pan should start.
  unsigned int mMinimumMotionEvents;    ///< The minimum motion events before pan should start.
  unsigned int mMotionEvents;           ///< The motion events received so far (before pan is emitted).
};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_PAN_GESTURE_DETECTOR_BASE_H__
