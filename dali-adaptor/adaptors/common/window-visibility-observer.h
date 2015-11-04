#ifndef __DALI_INTERNAL_WINDOW_VISIBILITY_OBSERVER_H__
#define __DALI_INTERNAL_WINDOW_VISIBILITY_OBSERVER_H__

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

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * An interface used to observe when the application's window is shown/hidden.
 */
class WindowVisibilityObserver
{
public:

  /**
   * Called when the window becomes fully or partially visible.
   */
  virtual void OnWindowShown() = 0;

  /**
   * Called when the window is fully hidden.
   */
  virtual void OnWindowHidden() = 0;

protected:

  /**
   * Protected Constructor.
   */
  WindowVisibilityObserver()
  {
  }

  /**
   * Protected virtual destructor.
   */
  virtual ~WindowVisibilityObserver()
  {
  }
};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_WINDOW_VISIBILITY_OBSERVER_H__
