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
#include "frame-time.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <dali/integration-api/platform-abstraction.h>

namespace Dali
{

using Integration::PlatformAbstraction;

namespace Internal
{
namespace Adaptor
{

namespace
{
#if defined(DEBUG_ENABLED)
Integration::Log::Filter* gLogFilter = Integration::Log::Filter::New(Debug::NoLogging, false, "LOG_FRAME_TIME");
#endif

const unsigned int DEFAULT_MINIMUM_FRAME_TIME_INTERVAL( 16667u );

const unsigned int MICROSECONDS_PER_SECOND( 1000000u );
const unsigned int MICROSECONDS_PER_MILLISECOND( 1000u );

const float        MICROSECONDS_TO_SECONDS( 0.000001f );

const unsigned int HISTORY_SIZE(3);

// constants to keep code readability with unsigned int has to be used as boolean (due to multithreading)
const unsigned int TRUE = 1u;
const unsigned int FALSE = 0u;
} // unnamed namespace


FrameTime::FrameTime( PlatformAbstraction& platform )
: mPlatform( platform ),
  mMinimumFrameTimeInterval( DEFAULT_MINIMUM_FRAME_TIME_INTERVAL ),
  mLastSyncTime( 0u ),
  mLastSyncTimeAtUpdate( 0u ),
  mLastSyncFrameNumber( 0u ),
  mLastUpdateFrameNumber( 0u ),
  mRunning( TRUE ),
  mFirstFrame( TRUE ),
  writePos( 0u ),
  mExtraUpdatesSinceSync( 0u )
{
  // Clear buffer
  for ( unsigned int i = 0; i < HISTORY_SIZE; ++i )
  {
    mPreviousUpdateFrames[i] = 0;
  }

  SetLastSyncTime();
  mLastSyncTimeAtUpdate = mLastSyncTime;

  DALI_LOG_INFO( gLogFilter, Debug::Concise, "FrameTime Initialized\n" );
}

FrameTime::~FrameTime()
{
}

void FrameTime::SetMinimumFrameTimeInterval( unsigned int interval )
{
  mMinimumFrameTimeInterval = interval;
}

void FrameTime::SetSyncTime( unsigned int frameNumber )
{
  // Only set the render time if we are running
  if ( mRunning )
  {
    SetLastSyncTime();

    mLastSyncFrameNumber = frameNumber;

    DALI_LOG_INFO( gLogFilter, Debug::General, "FrameTime: SetSyncTime(): Frame: %u: Time: %u\n", mLastSyncFrameNumber, (unsigned int) ( mLastSyncTime / MICROSECONDS_PER_MILLISECOND ) );
  }
}

void FrameTime::Suspend()
{
  mRunning = FALSE;

  // Reset members
  mLastSyncFrameNumber = 0;
  mLastUpdateFrameNumber = 0;
  writePos = 0;
  mExtraUpdatesSinceSync = 0;

  // Clear buffer
  for ( unsigned int i = 0; i < HISTORY_SIZE; ++i )
  {
    mPreviousUpdateFrames[i] = 0;
  }

  DALI_LOG_INFO( gLogFilter, Debug::Concise, "FrameTime: Suspended\n" );
}

void FrameTime::Resume()
{
  DALI_LOG_INFO( gLogFilter, Debug::Concise, "FrameTime: Resuming\n" );

  SetLastSyncTime();   // Should only update the last Sync time so the elapsed time during suspension is taken into consideration when we next update.
  mFirstFrame = TRUE;

  mRunning = TRUE;
}

void FrameTime::Sleep()
{
  DALI_LOG_INFO( gLogFilter, Debug::Concise, "FrameTime: Sleeping\n" );

  // Mimic Suspend behaviour
  Suspend();
}

void FrameTime::WakeUp()
{
  DALI_LOG_INFO( gLogFilter, Debug::Concise, "FrameTime: Waking Up\n" );

  SetLastSyncTime();
  mLastSyncTimeAtUpdate = mLastSyncTime; // We do not want any animations to progress as we have just been woken up.
  mFirstFrame = TRUE;
  mRunning = TRUE;
}

void FrameTime::PredictNextSyncTime( float& lastFrameDeltaSeconds, unsigned int& lastSyncTimeMilliseconds, unsigned int& nextSyncTimeMilliseconds )
{
  if ( mRunning )
  {
    const unsigned int minimumFrameTimeInterval( mMinimumFrameTimeInterval );
    const uint64_t lastSyncTime( mLastSyncTime );
    const unsigned int lastSyncFrameNumber( mLastSyncFrameNumber );

    float lastFrameDelta( 0.0f ); // Assume the last update frame delta is 0.
    unsigned int framesTillNextSync( 1 ); // Assume next render will be in one Sync frame time.

    unsigned int framesInLastUpdate( lastSyncFrameNumber - mLastUpdateFrameNumber );
    lastFrameDelta = lastSyncTime - mLastSyncTimeAtUpdate;

    // We should only evaluate the previous frame values if this is not the first frame.
    if ( !mFirstFrame )
    {
      // Check whether we have had any Syncs since we last did an Update.
      if ( framesInLastUpdate == 0 )
      {
        // We have had another update before a Sync, increment counter.
        ++mExtraUpdatesSinceSync;

        // This update frame will be rendered mUpdatesSinceSync later.
        framesTillNextSync += mExtraUpdatesSinceSync;
        DALI_LOG_INFO(gLogFilter, Debug::Concise, "PredictNextSyncTime UpdateBeforeSync\n");
      }
      else
      {
        mExtraUpdatesSinceSync = 0;
      }

      // If more than one frame elapsed since last Update, then check if this is a recurring theme so we can accurately predict when this Update is rendered.
      if ( framesInLastUpdate > 1 )
      {
        DALI_LOG_INFO(gLogFilter, Debug::Concise, "PredictNextSyncTime framesInLastUpdate:%u\n", framesInLastUpdate);
        unsigned int average(0);
        for ( unsigned int i = 0; i < HISTORY_SIZE; ++i )
        {
          average += mPreviousUpdateFrames[i];
        }
        average /= HISTORY_SIZE;

        if ( average > 1 )
        {
          // Our average shows a recurring theme, we are missing frames when rendering so calculate number of frames this will take.
          framesTillNextSync = average;
        }
      }

      // Write the number of frames the last update took to the array.
      mPreviousUpdateFrames[writePos] = framesInLastUpdate;
      writePos = ( writePos + 1 ) % HISTORY_SIZE;
    }

    mLastUpdateFrameNumber = lastSyncFrameNumber;
    mLastSyncTimeAtUpdate = lastSyncTime;
    mFirstFrame = FALSE;

    // Calculate the time till the next render
    unsigned int timeTillNextRender( minimumFrameTimeInterval * framesTillNextSync );

    // Set the input variables
    lastFrameDeltaSeconds = lastFrameDelta * MICROSECONDS_TO_SECONDS;
    lastSyncTimeMilliseconds = lastSyncTime / MICROSECONDS_PER_MILLISECOND;
    nextSyncTimeMilliseconds = ( lastSyncTime + timeTillNextRender ) / MICROSECONDS_PER_MILLISECOND;

    DALI_LOG_INFO( gLogFilter, Debug::General, "FrameTime: Frame: %u, Time: %u, NextTime: %u, LastDelta: %f\n", mLastUpdateFrameNumber, lastSyncTimeMilliseconds, nextSyncTimeMilliseconds, lastFrameDeltaSeconds );
    DALI_LOG_INFO( gLogFilter, Debug::Verbose, "                      FramesInLastUpdate: %u, FramesTillNextSync: %u\n", framesInLastUpdate, framesTillNextSync );
  }
}

inline void FrameTime::SetLastSyncTime()
{
  unsigned int seconds( 0u );
  unsigned int microseconds( 0u );

  mPlatform.GetTimeMicroseconds( seconds, microseconds );

  mLastSyncTime = seconds; // Promote from 32 bit to 64 bit value
  mLastSyncTime = ( mLastSyncTime * MICROSECONDS_PER_SECOND ) + microseconds;
}

} // namespace Adaptor
} // namespace Internal
} // namespace Dali
