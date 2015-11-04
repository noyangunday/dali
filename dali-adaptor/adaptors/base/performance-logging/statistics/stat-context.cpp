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
#include "stat-context.h"

// EXTERNAL INCLUDES
#include <cstdio>

// INTERNAL INCLUDES
#include <dali/integration-api/platform-abstraction.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

#define TIME_FMT "%0.2f ms"         // 2 decimal places, e.g. 5.34 ms
#define TOTAL_TIME_FMT "%0.1f secs" // 1 decimal place, e.g. 4.5 seconds

namespace
{
const unsigned int MILLISECONDS_PER_SECOND = 1000;    ///< 1000 milliseconds per second
const char* const UNKNOWN_CONTEXT_NAME = "UNKNOWN_CONTEXT_NAME";
const unsigned int MICROSECONDS_PER_SECOND = 1000000; ///< 1000000 microseconds per second
const unsigned int CONTEXT_LOG_SIZE = 120;

}

StatContext::StatContext( unsigned int id,
                          const char* const contextName,
                          PerformanceMarker::MarkerFilter contextType,
                          unsigned int logFrequencySeconds,
                          StatContextLogInterface& logInterface )
: mInitialMarker(PerformanceInterface::VSYNC),
  mName( contextName ),
  mLogInterface( logInterface ),
  mNamePlusStart( std::string(contextName) + "_START" ),
  mNamePlusEnd( std::string(contextName) + "_END" ),
  mId( id ),
  mLogFrequencyMicroseconds( logFrequencySeconds * MICROSECONDS_PER_SECOND ),
  mFilterType( contextType ),
  mLoggingEnabled( true ),
  mInitialMarkerSet( false )
{
  mTempLogBuffer = new char[ CONTEXT_LOG_SIZE ];
}

StatContext::~StatContext()
{
  delete []mTempLogBuffer;
}
unsigned int StatContext::GetId() const
{
  return mId;
}

const char* const StatContext::GetName() const
{
  return mName;
}

const char* const StatContext::GetMarkerDescription( PerformanceInterface::MarkerType type ) const
{
  if( type == PerformanceInterface::START )
  {
    return mNamePlusStart.c_str();
  }
  else if( type == PerformanceInterface::END )
  {
    return mNamePlusEnd.c_str();
  }
  return UNKNOWN_CONTEXT_NAME;
}
void StatContext::SetLogFrequency( unsigned int logFrequencySeconds )
{
  mLogFrequencyMicroseconds = logFrequencySeconds * MICROSECONDS_PER_SECOND;
}

void StatContext::EnableLogging( bool enableLogging )
{
  mLoggingEnabled = enableLogging;
}

void StatContext::ProcessCustomMarker( const PerformanceMarker& marker )
{
  // this marker has come from the application PerformanceLogger API
  RecordMarker( marker);
}

void StatContext::ProcessInternalMarker( const PerformanceMarker& marker )
{
  // this marker has come from DALi internal not the application
  // see if this context is for update, render or event
 if( marker.IsFilterEnabled( mFilterType ))
 {
   RecordMarker( marker );
 }
 // V_SYNC is always processed
 if( marker.GetType() == PerformanceInterface::VSYNC )
 {
   FrameTick( marker );
 }
}

void StatContext::RecordMarker( const PerformanceMarker& marker )
{
  if( marker.GetEventType() == PerformanceMarker::START_TIMED_EVENT )
  {
    mStats.StartTime( marker.GetTimeStamp() );
  }
  else if( marker.GetEventType() == PerformanceMarker::END_TIMED_EVENT )
  {
    mStats.EndTime( marker.GetTimeStamp() );
  }
}

void StatContext::FrameTick( const PerformanceMarker& marker )
{
  // wait until we've got some data
  if( ! mInitialMarkerSet )
  {
    mInitialMarker = marker;
    mInitialMarkerSet = true;
    return;
  }
  // log out every mLogFrequency.
  // check difference between first and last frame
  unsigned int microseconds = PerformanceMarker::MicrosecondDiff( mInitialMarker, marker );

  if( microseconds < mLogFrequencyMicroseconds )
  {
    return;
  }

  if( mLoggingEnabled )
  {
    LogMarker();
  }
  mStats.Reset();             // reset data for statistics
  mInitialMarkerSet = false;  // need to restart the timer

}

void StatContext::LogMarker()
{
  float mean, standardDeviation;
  mStats.CalculateMean( mean, standardDeviation );

  snprintf( mTempLogBuffer, CONTEXT_LOG_SIZE, "%s, min " TIME_FMT ", max " TIME_FMT ", total (" TOTAL_TIME_FMT "), avg " TIME_FMT ", std dev " TIME_FMT "\n",
     mName ? mName : UNKNOWN_CONTEXT_NAME,
     mStats.GetMinTime() * MILLISECONDS_PER_SECOND,
     mStats.GetMaxTime() * MILLISECONDS_PER_SECOND,
     mStats.GetTotalTime(),
     mean * MILLISECONDS_PER_SECOND,
     standardDeviation * MILLISECONDS_PER_SECOND );

    mLogInterface.LogContextStatistics( mTempLogBuffer );

}



} // namespace Internal

} // namespace Adaptor

} // namespace Dali
