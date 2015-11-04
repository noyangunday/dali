#ifndef __DALI_INTERNAL_ADAPTOR_BASE_SOCKET_INTERFACE_H__
#define __DALI_INTERNAL_ADAPTOR_BASE_SOCKET_INTERFACE_H__

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

#include <stdint.h>

namespace Dali
{
namespace Internal
{
namespace Adaptor
{

/**
 * @brief Abstract socket interface.
 * The typical usage is:
 *
 * @code
 *
 * SocketFactory::NewSocket( SocketInterface::TCP );
 * socket->ReuseAddress( true );
 * socket->Bind( port );
 * socket->Listen();
 *
 * ret = socket->Select();     // call socket->ExitSelect to break from select from another thread
 * if ( ret == DATA_AVAILABLE )
 * {
 *   socket->Read( ...);
 * }
 * socket->Close();
 *
 * @endcode
 *
 */
class SocketInterface
{
public:

  /**
   * @brief Protocol type
   */
  enum Protocol
  {
    TCP,          ///< Reliable, connection oriented
    UDP,          ///< Connection less, no guarantees of packet delivery, ordering
  };

  /**
   * @brief check is a socket is open
   * @return true if the socket is currently open
   */
  virtual bool SocketIsOpen() const = 0;

  /**
   * @brief CloseSocket
   * @return true on success, false on failure
   */
  virtual bool CloseSocket() = 0;

  /**
   * @brief Socket bind,  associate a local address with a socket ( normally uses AF_INET + INADDR_ANY)
   * @param[in] port network port
   * @return true on success, false on failure
   */
  virtual bool Bind( uint16_t port ) = 0;

  /**
   * @brief Indicate a willingness to accept incoming connection requests
   * @param[in] backlog maximum length of the queue of pending connections.
   * @return true on success, false on failure
   */
  virtual bool Listen( int blacklog) = 0;

  /**
   * @brief Wait for connection request and make connection
   * @return new client socket
   */
  virtual SocketInterface* Accept() const = 0;

  /**
   * @brief Select return values
   */
  enum SelectReturn
  {
    DATA_AVAILABLE,   ///< Data is available to read
    QUIT,             ///< ExitSelect() has been called on the socket
    ERROR             ///< Socket error
  };

  /**
   * @brief Waits for an event to occur (data available / error)
   * Returns when
   * - data has been sent to the socket
   * - client has closed the connection ( Read will return 0 bytes)
   * - ExitSelect has been called (returns QUIT)
   * - There is an error (returns ERROR)
   * @return DATA_AVAILABLE if data is available
   */
  virtual SelectReturn Select() = 0;

  /**
   * @brief To be called from a separate thread to break out of select
   */
  virtual void ExitSelect() = 0;

  /**
   * @brief Read data from the socket
   * @param[out] buffer data
   * @param[in] bufferSizeInBytes buffer size in bytes
   * @param[out] bytesRead number of bytes read
   * @return true on success, false on failure
   */
  virtual bool Read( void* buffer, unsigned int bufferSizeInBytes, unsigned int& bytesRead  ) = 0;

  /**
   * @brief Send data to the socket
   * @param[in] buffer data to write
   * @param[in] bufferSizeInBytes buffer size in write
   * @return true on success, false on failure
   */
  virtual bool Write( const void* buffer, unsigned int bufferSizeInBytes ) = 0;

  //
  // Common socket options. Please add more as required.
  // These should be wrappers around the setsockopt API

  /**
   * @brief Whether the SO_REUSEADDR is enabled or not.
   * @param[in] reuse flag.
   * @return true on success, false on failure
   */
  virtual bool ReuseAddress( bool reUse ) = 0;

  /**
   * @brief Socket buffer type
   */
  enum BufferType
  {
    SEND_BUFFER,        ///< (SO_SNDBUF) Send buffer size
    RECIEVE_BUFFER      ///< (SO_RCVBUF) Size of buffer allocated to hold data arriving to the socket
  };

  /**
   * @brief Set the send and recieve buffer sizes ( SO_SNDBUF, SO_RCVBUF )
   * @param[in] type buffer type
   * @param[in] size buffer size
   * @return true on success, false on failure
   */
  virtual bool SetBufferSize( BufferType type, unsigned int size ) = 0;

protected:

  /**
   * @brief Constructor
   */
  SocketInterface( )
  {
  }

  /**
   * @brief virtual destructor
   */
  virtual ~SocketInterface()
  {
  }

private:

  // Undefined copy constructor.
  SocketInterface( const SocketInterface& );

  // Undefined assignment operator.
  SocketInterface& operator=( const SocketInterface& );

};


} // Adaptor
} // Internal
} // Dali

#endif // __DALI_INTERNAL_ADAPTOR_BASE_SOCKET_INTERFACE_H__
