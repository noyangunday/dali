#ifndef __DALI_INTERNAL_ADAPTOR_CONNECTION_H_
#define __DALI_INTERNAL_ADAPTOR_CONNECTION_H_

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
#include <Ecore.h>
#include <Ecore_Ipc.h>

#include <dali/public-api/common/vector-wrapper.h>

namespace Dali
{
namespace Internal
{
namespace Adaptor
{

/**
 * Makes a connection to a given service as a client
 */
class ServerConnection
{
public:
  /**
   * Observes the connection for data and connection closure
   */
  class Observer
  {
  public:
    /**
     * Inform that data has been received on the connection
     * @param[in] event The event that has been received
     */
    virtual void DataReceived(void* event) = 0;

    /**
     * Inform the observer that the connection has closed
     */
    virtual void ConnectionClosed() = 0;
  };

public:
  /**
   * Constructor
   * @param[in] serviceName The name of the service
   * @param[in] serviceNumber The number of the service
   * @param[in] isSystem Whether to connect as local user or system user
   * @param[in] observer The connection observer
   */
  ServerConnection(const char *serviceName, int serviceNumber, bool isSystem, Observer* observer);

  /**
   * Destructor
   */
  ~ServerConnection();

  /**
   * Test if the connection is still alive
   * @return True if the connection is still alive
   */
  bool IsConnected();

  /**
   * Disconnect from the server. Will trigger ConnectionClosed() observer callback
   */
  void OnDisconnect();

  /**
   * Send an event to the server.
   * @param[in] event Event id
   * @param[in] data  Pointer to the event data
   * @param[in] size  Size of the event data
   * @return whether the event is sent successfully or not
   */
  bool SendEvent( int event, const void *data, int size );

  /**
   * Send an event to the server.
   * @param[in] event Event id
   * @param[in] ref   Message Reference number
   * @param[in] refTo Reference number of the message this refers to
   * @param[in] data  Pointer to the event data
   * @param[in] size  Size of the event data
   * @return whether the event is sent successfully or not
   */
  bool SendEvent( int event, int ref, int refTo, const void *data, int size );

private: // Class callbacks
  /**
   * Callback when server added
   */
  static Eina_Bool IpcServerAdd(void *data, int type, void *event);

  /**
   * Callback when server deleted
   */
  static Eina_Bool IpcServerDel(void *data, int type, void *event);

  /**
   * Callback when data available from server
   */
  static Eina_Bool IpcServerData(void *data, int type, void *event);

private:
  void CloseConnection();

private:
  typedef std::vector<Ecore_Event_Handler *> Handlers;

  struct Service
  {
    const char *name;
    int         num;
    bool        isSystem;
  };

  Service           mService;
  bool              mConnected;
  Observer*         mObserver;
  Ecore_Ipc_Server* mIpcServer;
  Handlers          mIpcHandlers;
};

} // Adaptor
} // Internal
} // Dali

#endif // __DALI_INTERNAL_ADAPTOR_CONNECTION_H_
