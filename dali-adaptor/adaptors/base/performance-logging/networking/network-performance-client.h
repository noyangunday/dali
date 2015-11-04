#ifndef __DALI_INTERNAL_ADAPTOR_NETWORK_PERFORMANCE_CLIENT_H__
#define __DALI_INTERNAL_ADAPTOR_NETWORK_PERFORMANCE_CLIENT_H__

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

// EXTERNAL INCLUDES
#include <pthread.h>

// INTERNAL INCLUDES
#include <base/performance-logging/performance-marker.h>
#include <trigger-event-factory-interface.h>
#include <base/performance-logging/networking/client-send-data-interface.h>
#include <base/interfaces/socket-factory-interface.h>


namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 *  @brief Network Performance client
 *
 *  Every time a client connects to Dali, a NetworkPerformanceClient object is created.
 *  It is responsible for processing incoming commands, and storing the client state
 *  (e.g. what performance markers it wants).
 *
 *  Certain commands such as dump-scene need to be run on the main Dali event thread.
 *  To achieve this, a trigger event is used which executes a function on the main thread.
 *  The sendDataInterface is then used with the client id to transmit the data to the client.
 *  The reason for using a client id is because the client
 *  can be deleted in between receiving a command and sending a response.
 *  E.g.
 *  NetworkPerformanceClient (own thread, id 5)  <---  Dump Scene Command
 *  delete NetworkPerformanceClient              <---  Connection closed
 *  MainThread. Send scene data to client 5. Client 5 has been deleted so don't send the data.
 *
 */
class NetworkPerformanceClient
{
public:

  /**
   * @brief Constructor
   * @param thread thread pointer
   * @param socket socket interface
   * @param clientId unique client id
   * @param triggerEventFactory used to create trigger events
   * @param sendDataInterface used to send data to the socket from main thread
   * @param SocketFactoryInterface used to delete the socket when the client is destroyed
   */
  NetworkPerformanceClient( pthread_t* thread,
                            SocketInterface *socket,
                            unsigned int clientId,
                            TriggerEventFactoryInterface& triggerEventFactory,
                            ClientSendDataInterface& sendDataInterface,
                            SocketFactoryInterface& socketFactory );

  /**
   * @brief Destructor
   */
  ~NetworkPerformanceClient();

  /**
   * @return client unique id
   */
  unsigned int GetId() const;

  /**
   * @return socket interface
   */
  SocketInterface& GetSocket();

  /**
   * @brief Write data to a socket. Can be called from any thread
   * @copydoc Dali::SocketInterface::Send
   */
  bool WriteSocket( const void* buffer, unsigned int bufferSizeInBytes );

  /**
   * @brief Process a command
   * @param buffer pointer to command data
   * @param bufferSizeInBytes how big the buffer is in bytes
   */
  void ProcessCommand( char* buffer, unsigned int bufferSizeInBytes );

  /**
   * @brief Write a marker to the socket, if this client is filtering this marker.
   * @param marker
   */
  bool TransmitMarker( const PerformanceMarker& marker, const char* const description );

  /**
   * @brief If the client is waiting inside a select statement, this will cause it
   * to break out.
   */
  void ExitSelect();

  /**
   * @brief get the thread running the client
   * @return thread pointer
   */
  pthread_t* GetThread();

private:

  pthread_t* mThread;                                   ///< thread for the client
  SocketInterface* mSocket;                             ///< socket interface
  PerformanceMarker::MarkerFilter mMarkerBitmask;       ///< What markers are currently filtered
  TriggerEventFactoryInterface& mTriggerEventFactory;   ///< Trigger event factory
  ClientSendDataInterface& mSendDataInterface;          ///< used to send data to a client from the main event thread
  SocketFactoryInterface& mSocketFactoryInterface;      ///< used to delete the socket
  unsigned int mClientId;                               ///< unique client id
  bool mConsoleClient;                                  ///< if connected via a console then all responses are in ASCII, not binary packed data.

};


} // namespace Internal

} // namespace Adaptor

} // namespace Dali

#endif
