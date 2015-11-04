#ifndef __DALI_INTERNAL_UPDATE_THREAD_H__
#define __DALI_INTERNAL_UPDATE_THREAD_H__

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
#include <pthread.h>

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

class ThreadSynchronization;
class AdaptorInternalServices;
class EnvironmentOptions;

/**
 * The update-thread is responsible for calling Core::Update(), and
 * for triggering the render-thread after each update.
 */
class UpdateThread
{
public:

  /**
   * Create the update-thread; this will not do anything until Start() is called.
   * @param[in] sync An object used to synchronize update & render threads.
   * @param[in] adaptorInterfaces base adaptor interface
   * @param[in] environmentOptions environment options
   */
  UpdateThread(ThreadSynchronization& sync,
               AdaptorInternalServices& adaptorInterfaces,
               const EnvironmentOptions& environmentOptions );

  /**
   * Non-virtual destructor; UpdateThread is not suitable as a base class.
   */
  ~UpdateThread();

  /**
   * Starts the update-thread
   */
  void Start();

  /**
   * Stops the update-thread
   */
  void Stop();

private:

  /**
   * This method is used by the update-thread for calling Core::Update().
   * @return true, if the thread finishes properly.
   */
  bool Run();

  /**
   * When DALI_FPS_TRACKING is enabled, this method calculates the frame rates for the specified time period
   */
  void FPSTracking(float secondsFromLastFrame);

  /**
   * Output the FPS information
   * when the FSP tracking is enabled,
   * it is called when the specified tracking period is elapsed or in the destructor when the process finished beforehand
   */
  void OutputFPSRecord();

  /**
   * Optionally output the update thread status.
   * @param[in] keepUpdatingStatus Whether the update-thread requested further updates.
   */
  void UpdateStatusLogging( unsigned int keepUpdatingStatus );

  /**
   * Helper for the thread calling the entry function
   * @param[in] This A pointer to the current UpdateThread object
   */
  static inline void* InternalThreadEntryFunc( void* This )
  {
    ( static_cast<UpdateThread*>( This ) )->Run();
    return NULL;
  }

private: // Data

  ThreadSynchronization&              mThreadSynchronization; ///< Used to synchronize all the threads

  Dali::Integration::Core&            mCore;                ///< Dali core reference

  float                               mFpsTrackingSeconds;  ///< fps tracking time length in seconds
  float                               mFrameCount;          ///< how many frames occurred during tracking period
  float                               mElapsedTime;         ///< time elapsed from previous fps tracking output

  unsigned int                        mStatusLogInterval;   ///< Interval in frames between status debug prints
  unsigned int                        mStatusLogCount;      ///< Used to count frames between status debug prints

  pthread_t*                          mThread;              ///< The actual update-thread.
  const EnvironmentOptions&           mEnvironmentOptions;  ///< environment options
}; // class UpdateThread

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_UPDATE_THREAD_H__
