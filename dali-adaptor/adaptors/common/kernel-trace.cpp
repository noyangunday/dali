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
#include "kernel-trace.h"

// EXTERNAL HEADERS
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

// INTERNAL HEADERS
#include <dali/integration-api/debug.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{
const char* TRACE_MARKER_FILE = "/sys/kernel/debug/tracing/trace_marker";
const char* SPI_PREFIX = "SPI_EV_DALI_"; ///< prefix to let the SPI tool know it should read the trace
}// un-named name space

KernelTrace::KernelTrace()
: mFileDescriptor( 0 ),
  mLoggedError( false )
{
}

KernelTrace::~KernelTrace()
{
  if( mFileDescriptor )
  {
    close( mFileDescriptor );
  }
}

// If this function doesn't appear to work, you can test manually on the device.
// $ cd /sys/kernel/debug/tracing
//
// If the folder doesn't exist then the kernel needs to be re-built with ftrace enabled
// If it does exist, then you can continue to test ftrace is working:
//
// $ echo 1 > tracing_enabled
// $ echo "test" > trace_marker
// $ cat trace
// should print out test message
// If the message did not get added to the trace, then check you have write permissions to the trace_marker file.
//
//
void KernelTrace::Trace( const PerformanceMarker& marker, const std::string& traceMessage )
{
  // Open the trace_marker file
  if( mFileDescriptor == 0 )
  {
    mFileDescriptor = open( TRACE_MARKER_FILE , O_WRONLY);
    if( mFileDescriptor == -1 )
    {
      // we want to keep trying to open it, so it will start working if someone fixes
      // the permissions on the trace marker
      mFileDescriptor = 0;

      // first time we fail to open the file, log an error
      if( !mLoggedError )
      {
        mLoggedError = true;
        DALI_LOG_ERROR("Failed to open /sys/kernel/debug/tracing/trace_marker for writing please check file permissions.");
      }

    }
  }

  if( mFileDescriptor > 0 )
  {
      std::string msg( SPI_PREFIX );
      msg+=traceMessage;

      int ret = write( mFileDescriptor, msg.c_str(), msg.length() );
          // if it failed then close the file description and try again next time we trace
      if( ret < 0 )
      {
        close( mFileDescriptor );
        mFileDescriptor = 0;
      }
  }
}

} // namespace Internal

} // namespace Adaptor

} // namespace Dali

