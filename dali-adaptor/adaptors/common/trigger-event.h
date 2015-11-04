#ifndef __DALI_INTERNAL_TRIGGER_EVENT_H__
#define __DALI_INTERNAL_TRIGGER_EVENT_H__

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
#include <dali/public-api/common/dali-common.h>
#include <dali/public-api/signals/callback.h>

// INTERNAL INCLUDES
#include <trigger-event-interface.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

class FileDescriptorMonitor;

/**
 * The TriggerEvent class is used to send events between threads.  For example, this can be used
 * to wake up one thread from another thread.
 *
 * Typically, these should be created in the application thread.
 *
 * The observer will be informed whenever the event is triggered.
 *
 * The implementation of TriggerEvent uses an event file descriptor.
 */
class TriggerEvent : public TriggerEventInterface
{
public:

  /**
   * Constructor
   * Creates an event file descriptor and starts a GSource which reads from the file
   * descriptor when there is data.
   *
   * @param[in] callback The callback to call
   * @param[in] options Trigger event options.
   * @note The ownership of callback is taken by this class.
   */
  TriggerEvent( CallbackBase* callback, TriggerEventInterface::Options options );

  /**
   * Destructor
   */
  ~TriggerEvent();

public:

  /**
   * Triggers the event.
   *
   * This can be called from one thread in order to wake up another thread.
   */
  void Trigger();

private:

  /**
   * Called when our event file descriptor has been written to.
   */
  void Triggered();

private:

  struct Source;

private:

  FileDescriptorMonitor* mFileDescriptorMonitor;
  CallbackBase* mCallback;
  int mFileDescriptor;
  TriggerEventInterface::Options mOptions;
};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_TRIGGER_EVENT_H__
