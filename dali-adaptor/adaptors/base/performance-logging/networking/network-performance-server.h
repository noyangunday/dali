#ifndef __DALI_INTERNAL_ADAPTOR_NETWORK_PERFORMANCE_SERVER_H__
#define __DALI_INTERNAL_ADAPTOR_NETWORK_PERFORMANCE_SERVER_H__

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
#include <dali/devel-api/threading/mutex.h>
#include <dali/public-api/common/dali-vector.h>

// INTERNAL INCLUDES
#include <base/environment-options.h>
#include <base/performance-logging/networking/network-performance-client.h>
#include <base/interfaces/adaptor-internal-services.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

class SocketInterface;
class PerformanceMarker;

/**
 *  @brief  The class listens for incoming connections on a dedicated thread.
 *
 *  When a new connection is established a client thread is spawned to
 *  handle that connection, along with a NetworkPerformanceClient object.
 *  The NetworkPerformanceClient object performs processing of incoming
 *  commands and holds the per-client state information for performance monitoring.
 *
 *  Server->Start()
 *  - Open socket
 *  - Spawns a thread to listen for incoming connections
 *  <---- New connection
 *  - Spawns a client thread to communicate with new client
 *
 *  Server->Stop()
 *  - Stops listening thread
 *  - Stops all client threads
 */
class NetworkPerformanceServer : public ClientSendDataInterface
{

public:

  /**
   * @brief Constructor
   * @param[in] adaptorServices adaptor internal services
   * @param[in] logOptions log options
   */
  NetworkPerformanceServer( AdaptorInternalServices& adaptorServices, const EnvironmentOptions& logOptions );


  /**
   * @brief Start the server, to be called form Dali main thread
   * @pre Can only be called form Dali main thread
   */
  void Start();

  /**
   * @brief Stop the server
   * @pre Can only be called form Dali main thread
   */
  void Stop();

  /**
   * @return true if the server is running
   */
  bool IsRunning() const;

  /**
   * @brief Transmit a marker to any clients are listening for this marker.
   * @param[in] marker performance marker
   * @param[in] description marker description
   * @pre Can be called from any thread
   *
   */
  void TransmitMarker( const PerformanceMarker& marker, const char* const description );

  /**
   * Destructor
   */
   ~NetworkPerformanceServer();

protected:  // ClientSendDataInterface

  /**
   * @copydoc ClientSendDataInterface::ClientSendDataInterface()
   */
  virtual void SendData( const char* const data, unsigned int bufferSizeInBytes, unsigned int clientId );

private:

  /**
   * Helper for the thread calling the entry function.
   * @param[in] This A pointer to the current RenderThread object
   */
  static void* ConnectionListenerFunc( void* This )
  {
    ( static_cast<NetworkPerformanceServer*>( This ) )->ConnectionListener();
    return NULL;
  }

  /**
   * Helper for the thread calling the entry function.
   * @param[in] This A pointer to the current RenderThread object
   */
  static void* ClientThreadFunc( void* data );

  /**
   * @brief Client thread function
   * @param client network client object
   */
  void ClientThread( NetworkPerformanceClient* client );

  /**
   * @brief Stop all client threads
   */
  void StopClients();

  /**
   * @brief Waits for new connections to be made
   */
  void ConnectionListener();

  /**
   * @brief Add a new client to the client list
   * @param clientSocket client socket
   * @param clientThread client thread
   * @return client
   */
  NetworkPerformanceClient* AddClient( SocketInterface* clientSocket, pthread_t* clientThread );

  /**
   * @brief Delete a client from the client list
   * @param client  network client
   */
  void DeleteClient( NetworkPerformanceClient* client );

  NetworkPerformanceServer( const NetworkPerformanceServer& );            ///< undefined copy constructor
  NetworkPerformanceServer& operator=( const NetworkPerformanceServer& ); ///< undefined assignment operator


  TriggerEventFactoryInterface& mTriggerEventFactory;     ///< used to create trigger events
  SocketFactoryInterface& mSocketFactory;                 ///< used to create sockets
  const EnvironmentOptions& mLogOptions;                  ///< log options
  Dali::Vector< NetworkPerformanceClient* > mClients;     ///< list of connected clients
  pthread_t mServerThread;                                ///< thread that listens for new connections
  SocketInterface* mListeningSocket;                      ///< socket used to listen for new connections
  Dali::Mutex mClientListMutex;                           ///< mutex
  unsigned int mClientUniqueId;                           ///< increments for every client connection
  volatile unsigned int mClientCount;                     ///< client count
  bool mLogFunctionInstalled;                             ///< whether the log function is installed

};


} // namespace Internal

} // namespace Adaptor

} // namespace Dali

#endif //__DALI_INTERNAL_ADAPTOR_NETWORK_PERFORMANCE_SERVER_H__
