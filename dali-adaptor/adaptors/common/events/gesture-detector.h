#ifndef __DALI_INTERNAL_GESTURE_DETECTOR_H__
#define __DALI_INTERNAL_GESTURE_DETECTOR_H__

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
#include <dali/public-api/events/gesture.h>
#include <dali/public-api/math/vector2.h>
#include <dali/public-api/object/ref-object.h>

namespace Dali
{

namespace Integration
{
class Core;
class GestureRequest;
struct TouchEvent;
}

namespace Internal
{

namespace Adaptor
{

/**
 * Abstract Base class for all adaptor gesture detectors.
 *
 * @note this may be replaced by gesture events sent directly from X.
 */
class GestureDetector : public RefObject
{
public:

  /**
   * Called by the gesture manager when it gets a touch event.  The gesture detector should
   * evaluate this event along with previously received events to determine whether the gesture
   * they require has taken place.
   * @param[in]  event  The latest touch event.
   */
  virtual void SendEvent(const Integration::TouchEvent& event) = 0;

  /**
   * Called by the gesture manager when Core updates the gesture's detection requirements.
   * @param[in]  request  The updated detection requirements.
   */
  virtual void Update(const Integration::GestureRequest& request) = 0;

  /**
   * Returns the type of gesture detector.
   * @return Type of gesture detector.
   */
  Gesture::Type GetType() const { return mType; }

protected:

  /**
   * Protected Constructor.  Should only be able to create derived class objects.
   * @param[in]  screenSize    The size of the screen.
   * @param[in]  detectorType  The type of gesture detector.
   */
  GestureDetector(Vector2 screenSize, Gesture::Type detectorType)
  : mScreenSize(screenSize), mType(detectorType) {}

  /**
   * Virtual destructor.
   */
  virtual ~GestureDetector() {}

protected:

  Vector2 mScreenSize;
  Gesture::Type mType;
};

typedef IntrusivePtr<GestureDetector> GestureDetectorPtr;

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_GESTURE_DETECTOR_H__
