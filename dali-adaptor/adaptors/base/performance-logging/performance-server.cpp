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

// CLASS HEADER
#include "performance-server.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/platform-abstraction.h>

// INTERNAL INCLUDES
#include <base/environment-options.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

PerformanceServer::PerformanceServer( AdaptorInternalServices& adaptorServices,
                                      const EnvironmentOptions& environmentOptions)
:mPlatformAbstraction( adaptorServices.GetPlatformAbstractionInterface() ),
 mEnvironmentOptions( environmentOptions ),
 mKernelTrace( adaptorServices.GetKernelTraceInterface() ),
 mSystemTrace( adaptorServices.GetSystemTraceInterface() ),
 mNetworkServer( adaptorServices, environmentOptions ),
 mStatContextManager( *this ),
 mStatisticsLogBitmask( 0 ),
 mNetworkControlEnabled( mEnvironmentOptions.GetNetworkControlMode()),
 mLoggingEnabled( false ),
 mLogFunctionInstalled( false )
{
  SetLogging( mEnvironmentOptions.GetPerformanceStatsLoggingOptions(),
              mEnvironmentOptions.GetPerformanceTimeStampOutput(),
              mEnvironmentOptions.GetPerformanceStatsLoggingFrequency());

  if( mNetworkControlEnabled )
  {
    mLoggingEnabled  = true;
    mNetworkServer.Start();
  }
}

PerformanceServer::~PerformanceServer()
{
  if( mNetworkControlEnabled )
  {
    mNetworkServer.Stop();
  }

  if( mLogFunctionInstalled )
  {
    mEnvironmentOptions.UnInstallLogFunction();
  }
}

void PerformanceServer::SetLogging( unsigned int statisticsLogOptions,
                                    unsigned int timeStampOutput,
                                    unsigned int logFrequency )
{
  mStatisticsLogBitmask = statisticsLogOptions;
  mPerformanceOutputBitmask = timeStampOutput;

  mStatContextManager.SetLoggingLevel( mStatisticsLogBitmask, logFrequency);

  if( ( mStatisticsLogBitmask == 0) && ( mPerformanceOutputBitmask == 0 ))
  {
    mLoggingEnabled = false;
  }
  else
  {
    mLoggingEnabled = true;
  }
}

void PerformanceServer::SetLoggingFrequency( unsigned int logFrequency, ContextId contextId )
{
  mStatContextManager.SetLoggingFrequency( logFrequency, contextId );
}

void PerformanceServer::EnableLogging( bool enable, ContextId contextId )
{
  mStatContextManager.EnableLogging( enable, contextId );
}

PerformanceInterface::ContextId PerformanceServer::AddContext( const char* name )
{
  // for adding custom contexts
  return mStatContextManager.AddContext( name, PerformanceMarker::CUSTOM_EVENTS );
}

void PerformanceServer::RemoveContext( ContextId contextId )
{
  mStatContextManager.RemoveContext( contextId );
}

void PerformanceServer::AddMarker( MarkerType markerType, ContextId contextId )
{
  // called only for custom markers

  if( !mLoggingEnabled )
  {
    return;
  }

  // Get the time stamp
  unsigned int seconds = 0;
  unsigned int microseconds = 0;
  mPlatformAbstraction.GetTimeMicroseconds( seconds, microseconds );

  // Create a marker
  PerformanceMarker marker( markerType, FrameTimeStamp( 0, seconds, microseconds ) );

  // get the marker description for this context, e.g SIZE_NEGOTIATION_START
  const char* const description = mStatContextManager.GetMarkerDescription( markerType, contextId );

  // log it
  LogMarker( marker, description );

  // Add custom marker to statistics context manager
  mStatContextManager.AddCustomMarker( marker, contextId );
}

void PerformanceServer::AddMarker( MarkerType markerType )
{
  // called only for internal markers

  if( !mLoggingEnabled )
  {
    return;
  }

  if( markerType == VSYNC )
  {
    // make sure log function is installed, note this will be called only from v-sync thread
    // if the v-sync thread has already installed one, it won't make any difference.
    if( ! mLogFunctionInstalled )
    {
      mEnvironmentOptions.InstallLogFunction();
      mLogFunctionInstalled = true;
    }
  }

  // Get the time
  unsigned int seconds = 0;
  unsigned int microseconds = 0;
  mPlatformAbstraction.GetTimeMicroseconds( seconds, microseconds );

  // Create a marker
  PerformanceMarker marker( markerType, FrameTimeStamp( 0, seconds, microseconds ) );

  // log it
  LogMarker(marker, marker.GetName() );

  // Add internal marker to statistics context manager
  mStatContextManager.AddInternalMarker( marker );

}

void PerformanceServer::LogContextStatistics( const char* const text )
{
  Integration::Log::LogMessage( Dali::Integration::Log::DebugInfo, text );
}

void PerformanceServer::LogMarker( const PerformanceMarker& marker, const char* const description )
{
  // log to the network ( this is thread safe )
  if( mNetworkControlEnabled )
  {
    mNetworkServer.TransmitMarker( marker, description );
  }

  // log to kernel trace
  if( mPerformanceOutputBitmask & OUTPUT_KERNEL_TRACE )
  {
    // Kernel tracing implementation may not be thread safe
    Mutex::ScopedLock lock( mLogMutex );
    // description will be something like UPDATE_START or UPDATE_END
    mKernelTrace.Trace( marker, description );
  }

  // log to system trace
  if( mPerformanceOutputBitmask & OUTPUT_SYSTEM_TRACE )
  {
    // System  tracing implementation may not be thread safe
    Mutex::ScopedLock lock( mLogMutex );

    mSystemTrace.Trace( marker, description );
  }

  // log to Dali log ( this is thread safe )
  if ( mPerformanceOutputBitmask & OUTPUT_DALI_LOG )
  {
    Integration::Log::LogMessage( Dali::Integration::Log::DebugInfo,
                                    "%d.%06d (seconds), %s\n",
                                    marker.GetTimeStamp().seconds,
                                    marker.GetTimeStamp().microseconds,
                                    description);
  }
}


} // namespace Internal

} // namespace Adaptor

} // namespace Dali
