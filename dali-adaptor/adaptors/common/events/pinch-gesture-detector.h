#ifndef __DALI_INTERNAL_PINCH_GESTURE_DETECTOR_H__
#define __DALI_INTERNAL_PINCH_GESTURE_DETECTOR_H__

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

// INTERNAL INCLUDES
#include <events/gesture-detector.h>

namespace Dali
{

namespace Integration
{
struct TouchEvent;
}

namespace Internal
{

namespace Adaptor
{

class CoreEventInterface;

/**
 * When given a set of touch events, this detector attempts to determine if a pinch gesture has taken place.
 */
class PinchGestureDetector : public GestureDetector
{
public:

  /**
   * Constructor
   * @param[in] coreEventInterface Used to send events to Core.
   * @param[in] screenSize The size of the screen.
   */
  PinchGestureDetector(CoreEventInterface& coreEventInterface, Vector2 screenSize, float minimumPinchDistance);

  /**
   * Virtual destructor.
   */
  virtual ~PinchGestureDetector();

  /**
   * @brief Sets minimum distance in pixels that the fingers must move towards/away from each other in order to
   * trigger a pinch gesture
   *
   * @param[in] distance The minimum pinch distance in pixels
   */
  void SetMinimumPinchDistance(float distance);

public:

  /**
   * @copydoc Dali::Internal::GestureDetector::SendEvent(const Integration::TouchEvent&)
   */
  virtual void SendEvent(const Integration::TouchEvent& event);

  /**
   * @copydoc Dali::Internal::GestureDetector::Update(const Integration::GestureRequest&)
   */
  virtual void Update(const Integration::GestureRequest& request);

private:

  /**
   * Emits the pinch gesture event to the core.
   * @param[in]  state         The state of the pinch (whether it's starting, continuing or finished).
   * @param[in]  currentEvent  The latest touch event.
   */
  void SendPinch(Gesture::State state, const Integration::TouchEvent& currentEvent);

private:

  /**
   * Internal state machine.
   */
  enum State
  {
    Clear,    ///< No gesture detected.
    Possible, ///< The current touch event data suggests that a gesture is possible.
    Started,  ///< A gesture has been detected.
  };

  CoreEventInterface& mCoreEventInterface; ///< Used to send events to Core.
  State mState; ///< The current state of the detector.
  std::vector<Integration::TouchEvent> mTouchEvents; ///< The touch events since initial touch down.

  float mMinimumDistanceDelta; ///< The minimum distance before a pinch is applicable.

  float mStartingDistance; ///< The distance between the two touch points when the pinch is first detected.
};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_PINCH_GESTURE_DETECTOR_H__
