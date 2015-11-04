#ifndef __DALI_INTERNAL_ADAPTOR_SOCKET_IMPL_H__
#define __DALI_INTERNAL_ADAPTOR_SOCKET_IMPL_H__

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

#include <base/interfaces/socket-interface.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * @brief Concrete implementation of a socket under Linux.
 *
 * Provides automatic closing of socket on destruction.
 */
class Socket : public SocketInterface
{
public:

  /**
   * @brief Constructor
   * @param protocol network protocol
   * @param fileDescriptor option file descriptor if the socket is already open
   */
  Socket( Protocol protocol , int fileDescriptor = -1 );

  /**
   * @copydoc Dali::Internal::Adaptor::SocketIsOpen()
   */
  virtual bool SocketIsOpen() const;

  /**
   * @copydoc Dali::Internal::Adaptor::SocketInterface::CloseSocket
   */
  virtual bool CloseSocket();

  /**
   * @copydoc Dali::Internal::Adaptor::SocketInterface::Bind
   */
  virtual bool Bind( uint16_t port ) ;

  /**
   * @copydoc Dali::Internal::Adaptor::SocketInterface::Listen
   */
  virtual bool Listen( int blacklog);

  /**
   * @copydoc Dali::Internal::Adaptor::SocketInterface::Accept
   */
  virtual SocketInterface* Accept() const ;

  /**
   * @copydoc Dali::Internal::Adaptor::SocketInterface::Select
   */
  virtual SelectReturn Select( );

  /**
   * @copydoc Dali::Internal::Adaptor::SocketInterface::ExitSelect
   */
  virtual void ExitSelect();

  /**
   * @copydoc Dali::Internal::Adaptor::SocketInterface::Recieve
   */
  virtual bool Read( void* buffer, unsigned int bufferSizeInBytes, unsigned int& bytesRead );

  /**
   * @copydoc Dali::Internal::Adaptor::SocketInterface::Send
   */
  virtual bool Write( const void* buffer, unsigned int bufferLength );

  /**
   * @copydoc Dali::Internal::Adaptor::SocketInterface::ReuseAddress
   */
  virtual bool ReuseAddress( bool reUse );

  /**
   * @copydoc Dali::Internal::Adaptor::SocketInterface::SetBufferSize
   *
   */
  virtual bool SetBufferSize( SocketInterface::BufferType type, unsigned int bufferSizeInBytes );

  /**
   * @brief Virtual destructor
   */
  virtual ~Socket();

private:


  /**
   * @brief Helper to create the quit pipe
   */
  bool CreateQuitPipe();

  /**
   * @brief  Helper to delete the quit pipe
   */
  void DeleteQuitPipe();

  int mSocketFileDescriptor; ///< file descriptor
  int mQuitPipe[2];          ///< Pipe to inform Select to quit.
  bool mBound:1;             ///< whether the socket is bound
  bool mListening:1;         ///< whether the socket is being listen to
  bool mQuitPipeCreated:1;   ///< whether the quit pipe has been created
  bool mBlocked:1;           ///< whether the socket is blocked waiting for a connection
};



} // Adaptor

} // Internal

} // Dali

#endif // __DALI_INTERNAL_ADAPTOR_SOCKET_IMPL_H__
