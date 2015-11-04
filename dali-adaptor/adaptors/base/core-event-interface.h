#ifndef __DALI_INTERNAL_ADAPTOR_CORE_EVENT_INTERFACE_H__
#define __DALI_INTERNAL_ADAPTOR_CORE_EVENT_INTERFACE_H__

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

namespace Integration
{
struct Event;
}

namespace Internal
{

namespace Adaptor
{

/**
 * This interface should be used by adaptor components to send events to Core.
 * This is preferable to using Core directly i.e. so there is a common place for measuring performance.
 */
class CoreEventInterface
{
public:

  /**
   * Queue an event with Core.
   * @param[in] event The new event.
   */
  virtual void QueueCoreEvent(const Dali::Integration::Event& event) = 0;

  /**
   * Process the events queued with QueueEvent().
   */
  virtual void ProcessCoreEvents() = 0;

protected:

  /**
   * Protected virtual destructor
   */
  virtual ~CoreEventInterface() {}
};

} // namespace Internal

} // namespace Adaptor

} // namespace Dali

#endif // __DALI_INTERNAL_ADAPTOR_CORE_EVENT_INTERFACE_H__
