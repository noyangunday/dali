#ifndef __DALI_INTEGRATION_TRIGGER_EVENT_INTERFACE_H__
#define __DALI_INTEGRATION_TRIGGER_EVENT_INTERFACE_H__

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

/**
 * @brief Interface for a trigger event class.
 *
 * The trigger event is a way of running a function call back on the main event thread of Dali.
 * To create a trigger event use the factory interface.
 */
class TriggerEventInterface
{

public:

  /**
   * @brief trigger event options
   */
  enum Options
  {
    KEEP_ALIVE_AFTER_TRIGGER,
    DELETE_AFTER_TRIGGER,  // automatically delete the trigger event object, after Trigger() is called.
  };

  /**
   * @brief Triggers the event.
   * This method cannot ever block, it needs to return immediately.
   * This can be called from one thread in order to wake up another thread.
   */
  virtual void Trigger() = 0;

protected:

  /**
   * @brief Constructor
   */
  TriggerEventInterface( )
  {
  }

public:
  /**
   * @brief Virtual destructor
   */
  virtual ~TriggerEventInterface()
  {
  }

private:

  // Undefined copy constructor.
  TriggerEventInterface( const TriggerEventInterface& );

  // Undefined assignment operator.
  TriggerEventInterface& operator=( const TriggerEventInterface& );


};

} // namespace Dali

#endif // __DALI_INTEGRATION_TRIGGER_EVENT_INTERFACE_H__
