#ifndef __DALI_INTERNAL_ADAPTOR_SOCKET_FACTORY_H__
#define __DALI_INTERNAL_ADAPTOR_SOCKET_FACTORY_H__
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

#include <base/interfaces/socket-factory-interface.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * @brief concrete implementation of the socket factory interface
 */
class SocketFactory : public SocketFactoryInterface
{
public:


  /**
   * @brief Constructor
   */
  SocketFactory( )
  {
  }

  /**
   * @brief destructor
   */
  virtual ~SocketFactory()
  {
  }

  /**
   * @copydoc SocketFactoryInterface::NewSocket()
   */
  virtual SocketInterface* NewSocket( SocketInterface::Protocol protocol  );

  /**
   * @copydoc SocketFactoryInterface::DestroySocket()
   */
  virtual void DestroySocket( SocketInterface* socket  );

};

} // Adaptor

} // Internal

} // Dali

#endif // __DALI_INTERNAL_ADAPTOR_SOCKET_FACTORY_H__
