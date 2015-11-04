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

// CLASS HEADER
#include "vsync-notifier.h"

// EXTERNAL INCLUDES
#include <unistd.h>
#include <dali/integration-api/core.h>
#include <dali/integration-api/platform-abstraction.h>

// INTERNAL INCLUDES
#include <base/interfaces/adaptor-internal-services.h>
#include <base/thread-synchronization.h>
#include <base/environment-options.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{

const unsigned int MICROSECONDS_PER_SECOND( 1000000u );
const unsigned int TIME_PER_FRAME_IN_MICROSECONDS( 16667u );

#if defined(DEBUG_ENABLED)
Integration::Log::Filter* gSyncLogFilter = Integration::Log::Filter::New(Debug::NoLogging, false, "LOG_VSYNC_NOTIFIER");
#endif

} // unnamed namespace

VSyncNotifier::VSyncNotifier( ThreadSynchronization& sync,
                              AdaptorInternalServices& adaptorInterfaces,
                              const EnvironmentOptions& environmentOptions )
: mThreadSynchronization( sync ),
  mCore( adaptorInterfaces.GetCore() ),
  mPlatformAbstraction( adaptorInterfaces.GetPlatformAbstractionInterface() ),
  mVSyncMonitor( adaptorInterfaces.GetVSyncMonitorInterface() ),
  mThread( NULL ),
  mEnvironmentOptions( environmentOptions ),
  mNumberOfVSyncsPerRender(1)
{
}

VSyncNotifier::~VSyncNotifier()
{
  DALI_LOG_INFO( gSyncLogFilter, Debug::General, "%s\n", __func__ );

  Stop();
}

void VSyncNotifier::Start()
{
  DALI_LOG_INFO( gSyncLogFilter, Debug::General, "%s\n", __func__ );

  if ( !mThread )
  {
    mVSyncMonitor->Initialize();

    mThread = new pthread_t();
    int error = pthread_create( mThread, NULL, InternalThreadEntryFunc, this );
    DALI_ASSERT_ALWAYS( !error && "Return code from pthread_create() in VSyncNotifier" );
  }
}

void VSyncNotifier::Stop()
{
  DALI_LOG_INFO( gSyncLogFilter, Debug::General, "%s\n", __func__ );

  if( mThread )
  {
    // wait for the thread to finish
    pthread_join(*mThread, NULL);

    delete mThread;
    mThread = NULL;
  }

  mVSyncMonitor->Terminate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The following is executed inside the notifier thread !!!
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void VSyncNotifier::Run()
{
  // install a function for logging
  mEnvironmentOptions.InstallLogFunction();

  unsigned int frameNumber( 0u );             // frameCount, updated when the thread is paused
  unsigned int currentSequenceNumber( 0u );   // platform specific vsync sequence number (increments with each vsync)
  unsigned int currentSeconds( 0u );          // timestamp at latest sync
  unsigned int currentMicroseconds( 0u );     // timestamp at latest sync
  unsigned int seconds( 0u );
  unsigned int microseconds( 0u );

  bool validSync( true );
  while( mThreadSynchronization.VSyncReady( validSync, frameNumber++, currentSeconds, currentMicroseconds, mNumberOfVSyncsPerRender ) )
  {
    DALI_LOG_INFO( gSyncLogFilter, Debug::General, "VSyncNotifier::Run. 1 SyncWithUpdateAndRender(frame#:%d, current Sec:%u current uSec:%u)\n", frameNumber-1, currentSeconds, currentMicroseconds);

    // Hardware VSyncs available?
    if( mVSyncMonitor->UseHardware() )
    {
      DALI_LOG_INFO( gSyncLogFilter, Debug::General, "VSyncNotifier::Run. 2 Start hardware sync (%d frames) \n", mNumberOfVSyncsPerRender);

      for( unsigned int i=0; i<mNumberOfVSyncsPerRender; ++i )
      {
        // Yes..wait for N hardware VSync ticks
        validSync = mVSyncMonitor->DoSync( currentSequenceNumber, currentSeconds, currentMicroseconds );
      }
    }
    else
    {
      // No..use software timer
      mPlatformAbstraction.GetTimeMicroseconds( seconds, microseconds );

      unsigned int timeDelta( MICROSECONDS_PER_SECOND * (seconds - currentSeconds) );
      if( microseconds < currentMicroseconds)
      {
        timeDelta += (microseconds + MICROSECONDS_PER_SECOND) - currentMicroseconds;
      }
      else
      {
        timeDelta += microseconds - currentMicroseconds;
      }

      currentSeconds = seconds;
      currentMicroseconds = microseconds;

      unsigned int sleepTimeInMicroseconds = 0;

      if( timeDelta < TIME_PER_FRAME_IN_MICROSECONDS )
      {
        sleepTimeInMicroseconds = TIME_PER_FRAME_IN_MICROSECONDS - timeDelta;
      }
      sleepTimeInMicroseconds += mNumberOfVSyncsPerRender * TIME_PER_FRAME_IN_MICROSECONDS;

      DALI_LOG_INFO( gSyncLogFilter, Debug::General, "VSyncNotifier::Run. 2 Start software sync (%d frames, %u microseconds) \n", mNumberOfVSyncsPerRender, sleepTimeInMicroseconds);

      timespec sleepTime;
      sleepTime.tv_sec = 0;
      sleepTime.tv_nsec = sleepTimeInMicroseconds * 1000;
      nanosleep( &sleepTime, NULL );
    }
    mThreadSynchronization.AddPerformanceMarker( PerformanceInterface::VSYNC );
  }

  // uninstall a function for logging
  mEnvironmentOptions.UnInstallLogFunction();

}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
