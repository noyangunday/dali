#ifndef __DALI_INTERNAL_VSYNC_NOTIFIER_H__
#define __DALI_INTERNAL_VSYNC_NOTIFIER_H__

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
class PlatformAbstraction;

} // namespace Integration

namespace Internal
{

namespace Adaptor
{

class VSyncMonitorInterface;
class ThreadSynchronization;
class EnvironmentOptions;
class AdaptorInternalServices;

/**
 * Implements a simple class that either monitors vertical blanks from libdrm, or manages
 * a software timer to handle syncing.
 */
class VSyncNotifier
{
public:

  /**
   * Create the vsync notification thread; this will not start to monitor vsync and
   * send notifications until Start() is called.
   * @param[in] sync         An object used to synchronize update, render and vsync threads.
   * @param[in] adaptorInterfaces base adaptor interface
   * @param[in] environmentOptions environment options
   */
  VSyncNotifier( ThreadSynchronization& sync,
                 AdaptorInternalServices& adaptorInterfaces,
                 const EnvironmentOptions& environmentOptions);

  /**
   * Non-virtual destructor; VSyncNotifier is not suitable as a base class.
   */
  ~VSyncNotifier();

  /**
   * Starts the thread
   */
  void Start();

  /**
   * Stops the thread
   */
  void Stop();

private:

  /**
   * The main thread loop. The system thread will be destroyed on
   * exit from this function.
   */
  void Run();

  /**
   * Helper for the thread calling the entry function
   * @param[in] This A pointer to the current VSyncNotifier object
   */
  static inline void* InternalThreadEntryFunc( void* This )
  {
    ( static_cast<VSyncNotifier*>( This ) )->Run();
    return NULL;
  }

private:

  ThreadSynchronization&              mThreadSynchronization;   ///< Used to synchronize all the threads
  Dali::Integration::Core&            mCore;                    ///< Dali core reference
  Integration::PlatformAbstraction&   mPlatformAbstraction;     ///< The platform abstraction for retrieving the current time etc.
  VSyncMonitorInterface*              mVSyncMonitor;            ///< VSyncMonitor interface
  pthread_t*                          mThread;                  ///< The actual thread.
  const EnvironmentOptions&           mEnvironmentOptions;      ///< Environment options
  unsigned int                        mNumberOfVSyncsPerRender; ///< How many frames for each update/render cycle.

}; // class VSyncNotifier

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_VSYNC_NOTIFIER_H__
