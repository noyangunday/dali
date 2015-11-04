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

// CLASS HEADER
#include "trigger-event.h"

// EXTERNAL INCLUDES
#include <sys/eventfd.h>
#include <unistd.h>

#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES

#include <file-descriptor-monitor.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

TriggerEvent::TriggerEvent( CallbackBase* callback, TriggerEventInterface::Options options )
: mFileDescriptorMonitor( NULL ),
  mCallback( callback ),
  mFileDescriptor( -1 ),
  mOptions( options )
{
  // Create accompanying file descriptor.
  mFileDescriptor = eventfd(0, EFD_NONBLOCK);
  if (mFileDescriptor >= 0)
  {
    // Now Monitor the created event file descriptor
    mFileDescriptorMonitor = new FileDescriptorMonitor( mFileDescriptor, MakeCallback( this, &TriggerEvent::Triggered ) );
  }
  else
  {
    DALI_LOG_ERROR("Unable to create TriggerEvent File descriptor\n");
  }
}

TriggerEvent::~TriggerEvent()
{
  delete mFileDescriptorMonitor;
  delete mCallback;

  if (mFileDescriptor >= 0)
  {
    close(mFileDescriptor);
    mFileDescriptor = 0;
  }
}

void TriggerEvent::Trigger()
{
  if (mFileDescriptor >= 0)
  {
    // Increment event counter by 1.
    // Writing to the file descriptor triggers the Dispatch() method in the other thread
    // (if in multi-threaded environment).

    uint64_t data = 1;
    int size = write(mFileDescriptor, &data, sizeof(uint64_t));

    if (size != sizeof(uint64_t))
    {
      DALI_LOG_ERROR("Unable to write to UpdateEvent File descriptor\n");
    }
  }
  else
  {
    DALI_LOG_WARNING("Attempting to write to an invalid file descriptor\n");
  }
}

void TriggerEvent::Triggered()
{
  // Reading from the file descriptor resets the event counter, we can ignore the count.
  uint64_t receivedData;
  size_t size;
  size = read(mFileDescriptor, &receivedData, sizeof(uint64_t));
  if (size != sizeof(uint64_t))
  {
    DALI_LOG_WARNING("Unable to read to UpdateEvent File descriptor\n");
  }

  // Call the connected callback
  CallbackBase::Execute( *mCallback );

  //check if we should delete ourselves after the trigger
  if( mOptions == TriggerEventInterface::DELETE_AFTER_TRIGGER )
  {
    delete this;
  }
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
