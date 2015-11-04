#ifndef __DALI_INTERNAL_BASE_WINDOW_EVENT_INTERFACE_H__
#define __DALI_INTERNAL_BASE_WINDOW_EVENT_INTERFACE_H__

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
#include <dali/public-api/events/touch-point.h>
#include <dali/public-api/events/key-event.h>
#include <dali/public-api/events/wheel-event.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * @brief Abstract interface for handling DALi events received from the native window system
 *
 */
class WindowEventInterface
{

public:

  /**
   * @brief Touch Event callback
   * @param[in] point touch point
   * @param[in] timeStamp time stamp
   */
  virtual void TouchEvent( Dali::TouchPoint& point, unsigned long timeStamp ) = 0;

  /**
   * @brief Key Event callback
   * @param[in] keyEvent key event
   */
  virtual void KeyEvent( Dali::KeyEvent& keyEvent ) = 0;

  /**
   * @brief Wheel Event callback
   * @param[in] wheelEvent wheel event
   */
  virtual void WheelEvent( Dali::WheelEvent& wheelEvent ) = 0;

  /**
   * @brief Window damage callback
   * @param[in] damageArea Window damage area
   */
  virtual void DamageEvent( Dali::Rect<int>& damageArea ) = 0;

  /**
   * @brief Window Focused
   */
  virtual void WindowFocusIn() = 0;

  /**
   * @brief Window lost focus
   */
  virtual void WindowFocusOut() = 0;

protected:

  /**
   * @brief Constructor
   */
  WindowEventInterface()
  {
  }

  /**
   * Virtual destructor
   */
  virtual ~WindowEventInterface()
  {
  }

  // Undefined copy constructor.
  WindowEventInterface( const WindowEventInterface& );

  // Undefined assignment operator.
  WindowEventInterface& operator=( const WindowEventInterface& );

};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_BASE_WINDOW_EVENT_INTERFACE_H__
