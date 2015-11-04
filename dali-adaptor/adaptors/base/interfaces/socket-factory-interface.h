#ifndef __DALI_INTERNAL_ADAPTOR_BASE_SOCKET_FACTORY_INTERFACE_H__
#define __DALI_INTERNAL_ADAPTOR_BASE_SOCKET_FACTORY_INTERFACE_H__

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

#include <base/interfaces/socket-interface.h>

namespace Dali
{
namespace Internal
{
namespace Adaptor
{

/**
 * @brief abstract class to create and destroy sockets
 */
class SocketFactoryInterface
{
public:

  /**
   * @brief Create a new socket
   * @param protocol network protocol
   * @return true on success, false on failure
   */
   virtual SocketInterface* NewSocket( SocketInterface::Protocol protocol  ) = 0;

   /**
    * @brief destroy a socket
    * @param[in] socket socket to destroy
    */
   virtual void DestroySocket( SocketInterface* socket  ) = 0;

protected:

  /**
   * @brief Constructor
   */
   SocketFactoryInterface( )
  {
  }

  /**
   * @brief Virtual destructor
   */
  virtual ~SocketFactoryInterface()
  {
  }

private:

  // Undefined copy constructor.
  SocketFactoryInterface( const SocketFactoryInterface& );

  // Undefined assignment operator.
  SocketFactoryInterface& operator=( const SocketFactoryInterface& );

};



} // Adaptor
} // Internal
} // Dali

#endif // __DALI_INTERNAL_ADAPTOR_BASE_SOCKET_FACTORY_INTERFACE_H__
