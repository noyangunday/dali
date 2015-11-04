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
#include "timer-impl.h"
#include <dali/integration-api/debug.h>

// EXTERNAL INCLUDES
#include <uv.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{
void TimerSourceFunc (uv_timer_t* handle)
{
  Timer* timer = static_cast<Timer*>(handle->data);

  bool keepRunning = timer->Tick();
  if( !keepRunning )
  {
    timer->Stop();
  }
}
void FreeHandleCallback(uv_handle_t* handle )
{
  delete handle;
}

} // unnamed namespace

/**
 * Struct to hide away Ecore implementation details
 */
struct Timer::Impl
{
  Impl( unsigned int milliSec )
  : mTimerHandle( NULL ),
    mInterval( milliSec ),
    mRunning( false )
  {
  }

  ~Impl()
  {
    // the handle will still be alive for a short period after calling uv_close
    // set the data to NULL to avoid a dangling pointer
    mTimerHandle->data = NULL;

    uv_close( reinterpret_cast< uv_handle_t* >( mTimerHandle ), FreeHandleCallback );
  }

  bool Running()
  {
    return mRunning;
  }

  void Start( void* internalTimerPtr )
  {
    Stop(); // make sure we stop first if its currently running

    if( !mTimerHandle )
    {
      // heap allocate the handle as its lifetime will be longer than TimerImpl
      mTimerHandle = new uv_timer_t;

      // initialize the handle
      uv_timer_init( uv_default_loop(), mTimerHandle);
    }

    mRunning = true;

    mTimerHandle->data = internalTimerPtr;

    uv_timer_start( mTimerHandle, TimerSourceFunc, mInterval, mInterval);
  }

  void Stop()
  {
    if( mRunning )
    {
      mTimerHandle->data = NULL;
      uv_timer_stop( mTimerHandle );
      mRunning = false;
    }
  }

  uv_timer_t* mTimerHandle;
  unsigned int mInterval;
  bool      mRunning;
};

TimerPtr Timer::New( unsigned int milliSec )
{
  DALI_LOG_ERROR(" new timer");
  TimerPtr timer( new Timer( milliSec ) );
  return timer;
}

Timer::Timer( unsigned int milliSec )
: mImpl(new Impl(milliSec))
{
}

Timer::~Timer()
{
  // stop timers
  Stop();

  delete mImpl;
}

void Timer::Start()
{
  mImpl->Start( this );
}

void Timer::Stop()
{
  mImpl->Stop();
}

void Timer::SetInterval( unsigned int interval )
{
  // stop existing timer
  Stop();

  mImpl->mInterval = interval;

  // start new tick
  Start();
}

unsigned int Timer::GetInterval() const
{
  return mImpl->mInterval;
}

bool Timer::Tick()
{
  // Guard against destruction during signal emission
  Dali::Timer handle( this );

  bool retVal( false );

  // Override with new signal if used
  if( !mTickSignal.Empty() )
  {
    retVal = mTickSignal.Emit();

    // Timer stops if return value is false
    if (retVal == false)
    {
      Stop();
    }
    else
    {
      retVal = true;   // continue emission
    }
  }
  else // no callbacks registered
  {
    // periodic timer is started but nobody listens, continue
    retVal = true;
  }

  return retVal;
}

Dali::Timer::TimerSignalType& Timer::TickSignal()
{
  return mTickSignal;
}

bool Timer::IsRunning() const
{
  return mImpl->mRunning;
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
