#ifndef __DALI_INTERNAL_ADAPTOR_BASE_TIMER_INTERFACE_H__
#define __DALI_INTERNAL_ADAPTOR_BASE_TIMER_INTERFACE_H__

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

namespace Dali
{
namespace Internal
{
namespace Adaptor
{

/**
 * Interface for a timer class
 */
class TimerInterface
{
public:
  /**
   * @copydoc Dali::Timer::Start()
   */
  virtual void Start() = 0;

  /**
   * @copydoc Dali::Timer::Stop()
   */
  virtual void Stop() = 0;

  /**
   * @copydoc Dali::Timer::SetInterval()
   */
  virtual void SetInterval( unsigned int intervalInMilliseconds ) = 0;

  /**
   * @copydoc Dali::Timer::GetInterval()
   */
  virtual unsigned int GetInterval() const = 0;

  /**
   * @copydoc Dali::Timer::IsRunning()
   */
  virtual bool IsRunning() const = 0;

protected:
  /**
   * Virtual protected destructor, no deletion through this interface
   */
  virtual ~TimerInterface() { }
};


} // Adaptor
} // Internal
} // Dali

#endif // __DALI_INTERNAL_ADAPTOR_BASE_TIMER_INTERFACE_H__
