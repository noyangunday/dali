#ifndef __DALI_INTEGRATION_TRIGGER_EVENT_FACTORY_INTERFACE_H__
#define __DALI_INTEGRATION_TRIGGER_EVENT_FACTORY_INTERFACE_H__

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
#include <dali/public-api/signals/callback.h>

// INTERNAL INCLUDES
#include <trigger-event-interface.h>

namespace Dali
{

/**
 * @brief Trigger interface factory class for creating a TriggerEvents
 *
 */
class TriggerEventFactoryInterface
{
public:


  /**
   * @brief Create a new concrete implementation of the event trigger interface.
   * @param callback called when interface->Trigger() is called
   * @param options TriggerEventInterface option
   * @return pointer to a new trigger event
   * @note Ownership of callback should be taken over by deriving classes
   */
  virtual TriggerEventInterface* CreateTriggerEvent( CallbackBase* callback,
                                                     TriggerEventInterface::Options options ) = 0;
  /**
   * @brief destroy a trigger event
   * @param triggerEventInterface event to destroy
   */
  virtual void DestroyTriggerEvent( TriggerEventInterface* triggerEventInterface ) = 0;

protected:

  /**
   * @brief  Constructor
   */
  TriggerEventFactoryInterface()
  {
  }

  /**
   * @brief Virtual Destructor
   */
  virtual ~TriggerEventFactoryInterface()
  {
  }

private:


  // Undefined copy constructor.
  TriggerEventFactoryInterface( const TriggerEventFactoryInterface& );

  // Undefined assignment operator.
  TriggerEventFactoryInterface& operator=( const TriggerEventFactoryInterface& );

};

} // namespace Dali

#endif // __DALI_INTEGRATION_TRIGGER_EVENT_FACTORY_INTERFACE_H__
