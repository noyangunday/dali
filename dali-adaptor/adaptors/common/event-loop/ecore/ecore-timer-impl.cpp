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

// EXTERNAL INCLUDES
#include <Ecore.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

// LOCAL STUFF
namespace
{
Eina_Bool TimerSourceFunc (void *data)
{
  Timer* timer = static_cast<Timer*>(data);

  bool keepRunning = timer->Tick();

  return keepRunning ? EINA_TRUE : EINA_FALSE;
}
} // unnamed namespace

/**
 * Struct to hide away Ecore implementation details
 */
struct Timer::Impl
{
  Impl( unsigned int milliSec )
  : mId(NULL),
    mInterval(milliSec)
  {
  }

  Ecore_Timer * mId;
  unsigned int mInterval;
};

TimerPtr Timer::New( unsigned int milliSec )
{
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
  if(mImpl->mId != NULL)
  {
    Stop();
  }
  mImpl->mId = ecore_timer_add( (double)mImpl->mInterval/1000.0f, (Ecore_Task_Cb)TimerSourceFunc, this );
}

void Timer::Stop()
{
  if (mImpl->mId != NULL)
  {
    ecore_timer_del(mImpl->mId);
    mImpl->mId = NULL;
  }
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
  return mImpl->mId != NULL;
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
