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

// CLASS HEADER
#include "socket-impl.h"

// EXTERNAL INCLUDES
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <dali/integration-api/debug.h>


namespace Dali
{
namespace Internal
{
namespace Adaptor
{

namespace
{
const unsigned int MAX_SOCKET_DATA_WRITE_SIZE = 1024 * 1024 * 10 ; // limit maximum size to write to 10 MB
}

Socket::Socket( Protocol protocol , int fileDescriptor )
:mSocketFileDescriptor( fileDescriptor ),
 mBound(false),
 mListening(false),
 mQuitPipeCreated(false),
 mBlocked(false)
{
  int addressFamily( AF_INET );
  int netProtocol( IPPROTO_TCP );
  int type( SOCK_STREAM );    // for TCP

  if( protocol == UDP )
  {
    type = SOCK_DGRAM;
    netProtocol = IPPROTO_UDP;
  }
  if( mSocketFileDescriptor == -1)
  {
    mSocketFileDescriptor = socket( addressFamily,type, netProtocol);
  }
  else
  {
    // socket already open
    mBound = true;
  }
}

Socket::~Socket()
{
  if( SocketIsOpen() )
  {
    CloseSocket();
  }
}

bool Socket::SocketIsOpen() const
{
  return (mSocketFileDescriptor != -1);
}

bool Socket::CloseSocket()
{

  if( ! SocketIsOpen() )
  {
    DALI_LOG_ERROR("Socket already closed or is invalid \n");
    return false;
  }

  int ret = close( mSocketFileDescriptor );
  mSocketFileDescriptor = -1;
  mListening = false;
  mBound = false;

  if( ret == -1 )
  {
    DALI_LOG_ERROR("Socket close failed");
    return false;
  }
  return true;
}

bool Socket::Bind( uint16_t port )
{
  if( ! SocketIsOpen() || mBound )
  {
     DALI_LOG_ERROR("Socket is invalid, or already bound");
     return false;
  }
  struct sockaddr_in serverAddress;

  memset( &serverAddress, 0, sizeof(serverAddress) );
  serverAddress.sin_family = AF_INET;                             // internet
  serverAddress.sin_port = htons( port );  //  host-to-net short (16-bit) translation
  serverAddress.sin_addr.s_addr = htonl( INADDR_ANY ); //  binds the socket to all available interfaces

  int ret = bind( mSocketFileDescriptor,
                  (struct sockaddr* ) &serverAddress,
                  sizeof(serverAddress));

  if( ret == -1 )
  {
    char buf[512];
    DALI_LOG_ERROR( "bind failed for port %d %s \n", port, strerror_r( errno, buf, 512 ) );
    return false;
  }

  mBound = true;

  return true;
}

bool Socket::Listen( int blacklog)
{
  if( ! mBound || mListening )
  {
    DALI_LOG_ERROR("socket is not bound, or already opened for listening");
    return false;
  }
  int ret =  listen( mSocketFileDescriptor, blacklog);

  if( ret == -1 )
  {
    DALI_LOG_ERROR("Listen failed");
    return false;
  }

  mListening = true;

  return true;
}

SocketInterface* Socket::Accept() const
{
  if( !mListening )
  {
    DALI_LOG_ERROR("socket is not being listened to");
    return NULL;
  }

  struct sockaddr clientAddress;

  socklen_t addressLength(sizeof(sockaddr_in));

  int clientFileDescriptor = accept( mSocketFileDescriptor, &clientAddress, &addressLength);
  if( clientFileDescriptor == -1 )
  {
     DALI_LOG_ERROR("Accept failed");
     return NULL;
  }

  // create a new socket, only TCP supports connections
  Socket* client = new Socket( TCP, clientFileDescriptor );

  return client;
}

bool Socket::CreateQuitPipe()
{
  if( !mQuitPipeCreated )
  {
    // create a pipe file descriptor to be able to break from the Select statement
    //
    int ret = pipe( mQuitPipe );
    if( ret != 0)
    {
      DALI_LOG_ERROR("Pipe creation failed");
      return false;
    }
    mQuitPipeCreated = true;
  }
  return true;
}
void Socket::DeleteQuitPipe()
{
  if( mQuitPipeCreated )
  {
    close( mQuitPipe[0] );
    close( mQuitPipe[1] );
  }
}

SocketInterface::SelectReturn Socket::Select()
{
  bool ok = CreateQuitPipe();
  if( !ok )
  {
    return ERROR;
  }

  fd_set  readFileDescriptors, exceptFileDescriptors;
  FD_ZERO(&readFileDescriptors);
  FD_ZERO(&exceptFileDescriptors);

  FD_SET(mSocketFileDescriptor,&readFileDescriptors );
  FD_SET(mQuitPipe[0],&readFileDescriptors );

  FD_SET(mSocketFileDescriptor,&exceptFileDescriptors);

  unsigned int maxFd = mQuitPipe[0] > mSocketFileDescriptor ? mQuitPipe[0]: mSocketFileDescriptor;

  for( ;; )
  {
    // this will block waiting for file descriptors
    int ret = select( maxFd+1, &readFileDescriptors, NULL, &exceptFileDescriptors, NULL );
    if( ret == -1 )
    {
      DALI_LOG_ERROR("select failed");
      return ERROR;
    }
    else if ( FD_ISSET( mQuitPipe[0] , &readFileDescriptors ))
    {
      // ExitSelect() called
      return QUIT;
    }
    else if ( FD_ISSET( mSocketFileDescriptor, &readFileDescriptors ))
    {
      // socket data received
      return DATA_AVAILABLE;
    }
  }
  return QUIT;
}

void Socket::ExitSelect()
{
  if( mQuitPipeCreated )
  {
    // write a single character to the pipe (can be anything)
    char c = ' ';
    int ret = write( mQuitPipe[1], &c, 1);
    if( ret < 1 )
    {
      DALI_LOG_ERROR("ExitSelect failed!\n");
    }
    return;
  }
}

bool Socket::ReuseAddress( bool reUse  )
{
  if( ! SocketIsOpen() | mBound )
  {
    DALI_LOG_ERROR("Socket is invalid or already bound \n");
    return false;
  }

  int reUseInteger = reUse; // convert it to an int

  int ret = setsockopt( mSocketFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &reUseInteger, sizeof(reUseInteger));
  if( ret == -1 )
  {
    char buf[512];
    DALI_LOG_ERROR( "SO_REUSEADDR option failed %s \n", strerror_r( errno, buf, 512 ) );
    return false;
  }
  return true;
}

bool Socket::SetBufferSize( SocketInterface::BufferType type, unsigned int size )
{
  if( ! SocketIsOpen() || mBound )
  {
    DALI_LOG_ERROR("Socket is invalid or already bound \n");
    return false;
  }
  int option = SO_RCVBUF;
  if( type == SocketInterface::SEND_BUFFER )
  {
    option = SO_SNDBUF;
  }

  int ret = setsockopt( mSocketFileDescriptor, SOL_SOCKET,option,&size,sizeof(size));
  if( ret == -1 )
  {
    DALI_LOG_ERROR("SO_RCVBUF / SO_SNDBUF  option failed \n");
    return false;
  }
  return true;
}

bool Socket::Read( void* buffer, unsigned int bufferSizeInBytes, unsigned int& bytesRead )
{
  bytesRead = 0;

  if( !SocketIsOpen() )
  {
    DALI_LOG_ERROR("Socket is invalid \n");
    return false;
  }

  bytesRead = read( mSocketFileDescriptor, buffer, bufferSizeInBytes );

  return true;
}

bool Socket::Write( const void* buffer, unsigned int bufferSizeInBytes )
{
  if( !SocketIsOpen() )
  {
    DALI_LOG_ERROR("Socket is invalid \n");
    return false;
  }

  // check we don't try to write more than 10MB ( this can be increased if required)
  if( bufferSizeInBytes > MAX_SOCKET_DATA_WRITE_SIZE )
  {
    DALI_LOG_ERROR("Writing %d bytes exceeds MAX_SOCKET_DATA_WRITE_SIZE of %d bytes \n", bufferSizeInBytes, MAX_SOCKET_DATA_WRITE_SIZE);
    return false;
  }

  int bytesWritten = 0;

  // write isn't guaranteed to write the entire buffer in one go

  while(  bytesWritten != static_cast< int>(bufferSizeInBytes))
  {
    const char* byteBuffer = static_cast<const char *>( buffer );
    byteBuffer+=bytesWritten;

    int ret = write( mSocketFileDescriptor, byteBuffer, bufferSizeInBytes - bytesWritten );
    if( ret < 1)
    {
      DALI_LOG_ERROR("Socket writer error \n");
      return false;
    }
    else
    {
      bytesWritten += ret;
    }
  }
  return true;
}

} // Adaptor
} // Internal
} // Dali

