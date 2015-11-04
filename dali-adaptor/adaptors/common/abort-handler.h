#ifndef __DALI_INTERNAL_ADAPTOR_ABORT_HANDLER_H__
#define __DALI_INTERNAL_ADAPTOR_ABORT_HANDLER_H__

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
#include <signal.h>
#include <dali/public-api/signals/callback.h>

// INTERNAL INCLUDES
#include "application.h"

namespace Dali
{
namespace Internal
{
namespace Adaptor
{

/**
 * Class to listen to system signals and trigger an abort callback
 * when they occur.
 *
 * This class maintains a process wide singleton, as the signal(2) system
 * call is process specific, not thread specific.
 *
 * Currently, this precludes having multiple DALi instances in the same process.
 */
class AbortHandler
{
public:
  /**
   * Constructor
   * @param[in] callback The function to call when abort signals occur
   * @note The ownership of callback is passed onto this class.
   */
  AbortHandler( CallbackBase* callback );

  /**
   * Destructor
   */
  ~AbortHandler();

  /**
   * Add a signal you want to be handled by this abort handler.
   * @param[in] signum The signal number (from signum.h)
   * @return true if the signal handler was installed ok
   */
  bool AbortOnSignal( int signum );

private:
  /**
   * Signal handler - Called when signal is received.
   * Stops the application.
   */
  static void SignalHandler( int signum );

  /**
   * Default constructor - undefined
   */
  AbortHandler();

  /**
   * Copy constructor - undefined
   */
  AbortHandler(const AbortHandler& rhs);

  /**
   * Assignment operator - undefined
   */
  AbortHandler& operator=(const AbortHandler& rhs);

private:
  typedef void (*SignalHandlerFuncPtr )( int );

  // _NSIG comes from the signal.h linux system header, defining the number of signals.
  SignalHandlerFuncPtr        mSignalOldHandlers[_NSIG-1];
  unsigned long long          mSignalMask;

  CallbackBase*               mCallback;

  static AbortHandler*        gInstance;
};

} // Namespace Adaptor
} // Namespace Internal
} // Namespace Dali

#endif //  __DALI_INTERNAL_ADAPTOR_ABORT_HANDLER_H__
