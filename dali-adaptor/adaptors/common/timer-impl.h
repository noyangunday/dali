#ifndef __DALI_INTERNAL_TIMER_H__
#define __DALI_INTERNAL_TIMER_H__

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
#include <dali/public-api/common/dali-common.h>
#include <dali/public-api/object/base-object.h>

// INTERNAL INCLUDES
#include <base/interfaces/timer-interface.h>
#include <timer.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{
class Timer;

typedef IntrusivePtr<Timer> TimerPtr;

/**
 * Implementation of the timer
 */
class Timer : public BaseObject, public TimerInterface
{
public:
  static TimerPtr New( unsigned int milliSec );

  /**
   * Constructor
   * @param[in]  milliSec  Interval in milliseconds.
   */
  Timer( unsigned int milliSec );

  /**
   * Destructor.
   */
  virtual ~Timer();

public:

  /**
   * @copydoc Dali::Timer::Start()
   */
  virtual void Start();

  /**
   * @copydoc Dali::Timer::Stop()
   */
  virtual void Stop();

  /**
   * @copydoc Dali::Timer::SetInterval()
   */
  virtual void SetInterval( unsigned int interval );

  /**
   * @copydoc Dali::Timer::GetInterval()
   */
  virtual unsigned int GetInterval() const;

  /**
   * @copydoc Dali::Timer::IsRunning()
   */
  virtual bool IsRunning() const;

  /**
   * Tick
   */
  bool Tick();

public: // Signals

  Dali::Timer::TimerSignalType& TickSignal();

private: // Implementation

  // not implemented
  Timer( const Timer& );
  Timer& operator=( const Timer& );

private: // Data

  Dali::Timer::TimerSignalType mTickSignal;

  // To hide away implementation details
  struct Impl;
  Impl* mImpl;
};

inline Timer& GetImplementation(Dali::Timer& timer)
{
  DALI_ASSERT_ALWAYS(timer && "Timer handle is empty");

  BaseObject& handle = timer.GetBaseObject();

  return static_cast<Internal::Adaptor::Timer&>(handle);
}

inline const Timer& GetImplementation(const Dali::Timer& timer)
{
  DALI_ASSERT_ALWAYS(timer && "Timer handle is empty");

  const BaseObject& handle = timer.GetBaseObject();

  return static_cast<const Internal::Adaptor::Timer&>(handle);
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_TIMER_H__
