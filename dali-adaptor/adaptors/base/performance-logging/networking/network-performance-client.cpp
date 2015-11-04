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
#include "network-performance-client.h"

// EXTERNAL INCLUDES
#include <stdio.h>
#include <string>

// INTERNAL INCLUDES
#include <base/interfaces/socket-interface.h>
#include <base/performance-logging/networking/network-performance-protocol.h>
#include <base/performance-logging/networking/event/automation.h>


namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{
const char UNKNOWN_CMD[]= "Command or parameter invalid, type help for list of commands\n";


/**
 * helper class to store data along with the automation callback.
 */
class AutomationCallback: public CallbackBase
{
public:

  /**
   * instead of using templates, or having different callback classes for each callback
   * we use a command id that decides which static function to call on the Automation class.
   */
  enum CommandId
  {
    UNKNOWN_COMMAND,
    SET_PROPERTY,
    DUMP_SCENE
  };

  AutomationCallback(  unsigned int clientId, ClientSendDataInterface& sendDataInterface )
  :CallbackBase( reinterpret_cast< void* >( this ),
                 NULL, // we get the dispatcher to call function directly
                 reinterpret_cast< CallbackBase::Dispatcher>( &AutomationCallback::Dispatcher) ),
  mSendDataInterface( sendDataInterface ),
  mCommandId( UNKNOWN_COMMAND ),
  mClientId( clientId )
  {}

  void AssignSetPropertyCommand( std::string setPropertyCommand )
  {
    mCommandId = SET_PROPERTY;
    mPropertyCommand = setPropertyCommand;
  }
  void AssignDumpSceneCommand()
  {
     mCommandId = DUMP_SCENE;
  }

  void RunCallback()
  {
    switch( mCommandId )
    {
      case SET_PROPERTY:
      {
        Automation::SetProperty( mPropertyCommand );
        break;
      }
      case DUMP_SCENE:
      {
        Automation::DumpScene( mClientId, &mSendDataInterface);
        break;
      }
      default:
      {
        DALI_ASSERT_DEBUG( 0 && "Unknown command");
        break;
      }
    }
  }
  static void Dispatcher( CallbackBase& base )
  {
    AutomationCallback& automationCallback( static_cast< AutomationCallback& >( base) );
    automationCallback.RunCallback();
  }

private:

  std::string mPropertyCommand;                   ///< property command
  ClientSendDataInterface& mSendDataInterface;    ///< Abstract client send data interface
  CommandId mCommandId;                           ///< command id
  const unsigned int mClientId;                   ///< client id
};

} // unnamed namespace

NetworkPerformanceClient::NetworkPerformanceClient(  pthread_t* thread,
                                                     SocketInterface *socket,
                                                     unsigned int clientId,
                                                     TriggerEventFactoryInterface& triggerEventFactory,
                                                     ClientSendDataInterface& sendDataInterface,
                                                     SocketFactoryInterface& socketFactory )
: mThread( thread ),
  mSocket( socket ),
  mMarkerBitmask( PerformanceMarker::FILTERING_DISABLED ),
  mTriggerEventFactory( triggerEventFactory ),
  mSendDataInterface( sendDataInterface ),
  mSocketFactoryInterface( socketFactory ),
  mClientId( clientId ),
  mConsoleClient(false)
{

}

NetworkPerformanceClient::~NetworkPerformanceClient()
{
  if( mSocket->SocketIsOpen() )
  {
    mSocket->CloseSocket();
  }
  mSocketFactoryInterface.DestroySocket( mSocket );
}

unsigned int NetworkPerformanceClient::GetId() const
{
  return mClientId;
}

SocketInterface& NetworkPerformanceClient::GetSocket()
{
  return *mSocket;
}

bool NetworkPerformanceClient::WriteSocket( const void* buffer, unsigned int bufferSizeInBytes )
{
  return mSocket->Write( buffer, bufferSizeInBytes );
}

bool NetworkPerformanceClient::TransmitMarker( const PerformanceMarker& marker, const char* const description )
{
  if( ! marker.IsFilterEnabled( mMarkerBitmask ) )
  {
    return true;
  }
  if( mConsoleClient )
  {
    // write out the time stamp
    char buffer[64];
    int size = snprintf( buffer, sizeof(buffer),"%d.%06d (seconds), %s\n",
                                    marker.GetTimeStamp().seconds,
                                    marker.GetTimeStamp().microseconds,
                                    description );

   return mSocket->Write( buffer, size );

  }

  // todo serialize the data
  return false;
}

void NetworkPerformanceClient::ExitSelect()
{
  mSocket->ExitSelect();
}

pthread_t* NetworkPerformanceClient::GetThread()
{
  return mThread;
}

void NetworkPerformanceClient::ProcessCommand( char* buffer, unsigned int bufferSizeInBytes )
{
  // if connected via console, then strip off the carriage return, and switch to console mode
  if( buffer[ bufferSizeInBytes - 1] == '\n')
  {
    buffer[ bufferSizeInBytes - 1] = 0;
    mConsoleClient = true;
  }
  unsigned int param(0);
  std::string stringParam;
  PerformanceProtocol::CommandId commandId( PerformanceProtocol::UNKNOWN_COMMAND );

  bool ok =  PerformanceProtocol::GetCommandId( buffer, bufferSizeInBytes, commandId, param, stringParam );
  if( !ok )
  {
    WriteSocket( UNKNOWN_CMD, sizeof(UNKNOWN_CMD) );
    return;
  }
  std::string response;

  switch( commandId )
  {
    case PerformanceProtocol::HELP_MESSAGE:
    {
      response = PerformanceProtocol::GetHelpMessage();
      break;
    }

    case PerformanceProtocol::ENABLE_TIME_MARKER_BIT_MASK:
    {
      mMarkerBitmask  = static_cast<  PerformanceMarker::MarkerFilter >( param );
      response = "enable time marker ";
      break;
    }

    case PerformanceProtocol::DUMP_SCENE_GRAPH:
    {
      // this needs to be run on the main thread, use the trigger event....
      AutomationCallback* callback = new AutomationCallback( mClientId, mSendDataInterface );
      callback->AssignDumpSceneCommand();

      // create a trigger event that automatically deletes itself after the callback has run in the main thread
      TriggerEventInterface *interface = mTriggerEventFactory.CreateTriggerEvent( callback, TriggerEventInterface::DELETE_AFTER_TRIGGER );

      // asynchronous call, the call back will be run sometime later on the main thread
      interface->Trigger();
      break;
    }

    case PerformanceProtocol::SET_PROPERTIES:
    {
      // this needs to be run on the main thread, use the trigger event....
      AutomationCallback* callback = new AutomationCallback( mClientId, mSendDataInterface );
      callback->AssignSetPropertyCommand( stringParam );

      // create a trigger event that automatically deletes itself after the callback has run in the main thread
      TriggerEventInterface *interface = mTriggerEventFactory.CreateTriggerEvent( callback, TriggerEventInterface::DELETE_AFTER_TRIGGER );

      // asynchronous call, the call back will be run sometime later on the main thread
      interface->Trigger();
      break;
    }

    case PerformanceProtocol::LIST_METRICS_AVAILABLE:
    case PerformanceProtocol::ENABLE_METRIC:
    case PerformanceProtocol::DISABLE_METRIC:
    {
      response="Metrics currently not supported";
      break;
    }
    default:
    {
      response = UNKNOWN_CMD;
      break;
    }
  }
  if( ! response.empty() )
  {
    // add a carriage return for console clients
    if( mConsoleClient )
    {
      response+="\n";
    }
    WriteSocket( response.c_str(), response.length()  );
  }
}



} // namespace Internal

} // namespace Adaptor

} // namespace Dali
