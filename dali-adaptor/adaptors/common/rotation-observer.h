#ifndef __DALI_INTERNAL_ROTATION_OBSERVER_H__
#define __DALI_INTERNAL_ROTATION_OBSERVER_H__

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

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

struct RotationEvent
{
  int angle;     ///< one of 0, 90, 180, 270
  int winResize; ///< true if the window should be resized
  int width;     ///< new window width
  int height;    ///< new window height
};


/**
 * The RotationObserver can be overridden in order to listen to rotation events.
 */
class RotationObserver
{
public:

  /**
   * Deriving classes should override this to be notified when we receive a RotationPrepare event.
   * @param[in]  area  The area that has been rotationd.
   */
  virtual void OnRotationPrepare( const RotationEvent& rotation ) = 0;

  /**
   * Deriving classes should override this to be notifed when a RotationRequest event is received
   */
  virtual void OnRotationRequest( ) = 0;

protected:

  /**
   * Protected Constructor.
   */
  RotationObserver()
  {
  }

  /**
   * Protected virtual destructor.
   */
  virtual ~RotationObserver()
  {
  }
};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_ROTATION_OBSERVER_H__
