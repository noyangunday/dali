#ifndef __DALI_EVENT_THREAD_CALLBACK_H_
#define __DALI_EVENT_THREAD_CALLBACK_H_

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
#include <dali/public-api/common/dali-common.h>
#include <dali/public-api/signals/callback.h>

namespace Dali
{

/**
 * @brief The EventThreadCallback class provides a mechanism for the worker thread to trigger the execution of a given callback in main event thread .
 *
 * @note The EventThreadCallback object should only be created in the main thread.
 */
class DALI_IMPORT_API EventThreadCallback
{
public:

  /**
   * @brief Constructor. Create an object that will call the given callback in main event thread.
   *
   * @param[in] callback The callback to call.
   */
  EventThreadCallback( CallbackBase* callback );

  /**
   * @brief Destructor.
   */
  ~EventThreadCallback();

  /**
   * @brief Trigger the calling of callback.
   *
   * The method can be used from worker threads to notify the main thread as main thread is running the event loop and thus cannot be blocked
   */
  void Trigger();

private:

  // undefined copy constructor.
  EventThreadCallback( const EventThreadCallback& );

  // undefined assignment operator
  EventThreadCallback& operator=( const EventThreadCallback& );

private:

  struct Impl;
  Impl* mImpl;

};

}
#endif /* __DALI_EVENT_THREAD_CALLBACK_H_ */
