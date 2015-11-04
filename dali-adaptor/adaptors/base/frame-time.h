#ifndef __DALI_INTERNAL_ADAPTOR_FRAME_TIME_H__
#define __DALI_INTERNAL_ADAPTOR_FRAME_TIME_H__

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
#include <stdint.h>

namespace Dali
{

namespace Integration
{
class PlatformAbstraction;
}

namespace Internal
{
namespace Adaptor
{

/**
 * FrameTime stores the time of the last VSync. It can then be used by the update thread to predict
 * the current update will be rendered.
 */
class FrameTime
{
public:

  // Called from Event thread

  /**
   * Constructor
   * @param[in]  platform  The platform used to retrieve the current time
   */
  FrameTime( Integration::PlatformAbstraction& platform );

  /**
   * Destructor, non virtual
   */
  ~FrameTime();

  /**
   * Sets the expected minimum frame time interval.
   * @param[in]  interval  The interval in microseconds.
   */
  void SetMinimumFrameTimeInterval( unsigned int interval );

  /**
   * Suspends the FrameTime object when the application state changes
   */
  void Suspend();

  /**
   * Resumes the FrameTime object when the application state changes
   */
  void Resume();

  // Called from Update thread

  /**
   * Sets the FrameTime object to sleep, i.e. when there are no more updates required.
   */
  void Sleep();

  /**
   * Wakes the FrameTime object from a sleep state.
   */
  void WakeUp();

  /**
   * Predicts when the next render time will occur.
   *
   * @param[out]  lastFrameDeltaSeconds      The delta, in seconds (with float precision), between the last two renders.
   * @param[out]  lastSyncTimeMilliseconds  The time, in milliseconds, of the last Sync.
   * @param[out]  nextSyncTimeMilliseconds  The estimated time, in milliseconds, at the next Sync.
   *
   * @note Should only be called once per tick, from the update thread.
   */
  void PredictNextSyncTime( float& lastFrameDeltaSeconds, unsigned int& lastVSyncTimeMilliseconds, unsigned int& nextVSyncTimeMilliseconds );

  // Called from VSync thread

  /**
   * Tells the FrameTime object that a Sync has occurred.
   *
   * @param[in]  frameNumber  The frame number of the current Sync.
   *
   * @note Should only be called from the VSync thread.
   */
  void SetSyncTime( unsigned int frameNumber );

private:

  /**
   * Sets the current time to be the last Vsync time.
   */
  inline void SetLastSyncTime();

private:

  Integration::PlatformAbstraction& mPlatform; ///< The platform abstraction.

  unsigned int mMinimumFrameTimeInterval; ///< The minimum frame time interval, set by Adaptor.

  uint64_t mLastSyncTime;                ///< The last Sync time (in microseconds).
  uint64_t mLastSyncTimeAtUpdate;        ///< The last Sync time at Update (in microseconds).

  unsigned int mLastSyncFrameNumber;     ///< The last Sync frame number
  unsigned int mLastUpdateFrameNumber;   ///< The last Sync frame number handled in Update.

  // NOTE cannot use bitfields or booleans as these are used from multiple threads, must use variable with machine word size for atomic read/write
  unsigned int mRunning;                 ///< The state of the FrameTime object.
  unsigned int mFirstFrame;              ///< Whether the current update is the first frame (after initialisation, resume or wake up).

  unsigned int mPreviousUpdateFrames[3]; ///< Array holding the number of frames Update took in the last three iterations.
  unsigned int writePos;                 ///< The current write position in the array.

  unsigned int mExtraUpdatesSinceSync;   ///< The number of extra updates since the last Sync.
};

} // namespace Adaptor
} // namespace Internal
} // namespace Dali

#endif // __DALI_INTERNAL_ADAPTOR_FRAME_TIME_H__
