#ifndef __DALI_INTERNAL_FILE_DESCRIPTOR_MONITOR_H__
#define __DALI_INTERNAL_FILE_DESCRIPTOR_MONITOR_H__

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
#include <dali/public-api/signals/callback.h>

namespace Dali
{

namespace Integration
{
class Core;
}

namespace Internal
{

namespace Adaptor
{

/**
 * @brief Monitors the given file descriptor and whenever anything is written to it, the provided
 * callback is called
 */
class FileDescriptorMonitor
{
public:

  /**
   * Constructor
   * @param[in]  fileDescriptor  The file descriptor to monitor
   * @param[in]  callback        Called when anything is written to the file descriptor
   * @note The ownership of callback is taken by this class.
   */
  FileDescriptorMonitor( int fileDescriptor, CallbackBase* callback );

  /**
   * Destructor
   */
  ~FileDescriptorMonitor();

private:

  // Undefined
  FileDescriptorMonitor( const FileDescriptorMonitor& fileDescriptorMonitor );

  // Undefined
  FileDescriptorMonitor& operator=( const FileDescriptorMonitor& fileDescriptorMonitor );

private:
  struct Impl;
  Impl* mImpl;
};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_FILE_DESCRIPTOR_MONITOR_H__
