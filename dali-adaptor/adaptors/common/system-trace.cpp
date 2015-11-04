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
#include "system-trace.h"

// EXTERNAL HEADERS
#include <string>
#include <dali/devel-api/common/hash.h>

// INTERNAL HEADERS
#include <dali/integration-api/debug.h>

#ifdef ENABLE_TTRACE
#include <ttrace.h>
#else

// Emulate trace calls if ttrace isn't available
namespace
{
const int TTRACE_TAG_GRAPHICS = 1;

void traceAsyncBegin(int tag, int cookie, const char *name, ...)
{
  Debug::LogMessage(Debug::DebugInfo, "AsyncBegin: %s : cookie %d\n", name, cookie );
}
void traceAsyncEnd(int tag, int cookie, const char *name, ...)
{
  Debug::LogMessage(Debug::DebugInfo, "AsyncEnd: %s : cookie %d\n", name, cookie );
}
void traceMark(int tag, const char *name, ...)
{
  Debug::LogMessage(Debug::DebugInfo, "Marker: %s \n", name);
}
} // un-named namespace
#endif

namespace
{

int GetCookie( const std::string& description, std::string& markerName )
{
  // description holds the marker name and postfix of _START or _END
  std::size_t pos = description.find("_START");
  if( pos == std::string::npos )
  {
    pos = description.find("_END");
  }
  if( !pos )
  {
    // if this asserts then check the postfix strings in StatContext.cpp for
    // custom markers and performance-marker.cpp for built-in markers
    DALI_ASSERT_DEBUG(0);
  }
  markerName = description.substr( 0, pos );

  std::size_t hash =  Dali::CalculateHash( markerName.c_str() );
  return static_cast<int>( hash );
}
}

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

SystemTrace::SystemTrace()
{
}
SystemTrace::~SystemTrace()
{
}

void SystemTrace::Trace( const PerformanceMarker& marker, const std::string& traceMessage )
{
  PerformanceMarker::MarkerEventType eventType = marker.GetEventType();

  if( eventType == PerformanceMarker::SINGLE_EVENT )
  {
    traceMark( TTRACE_TAG_GRAPHICS, traceMessage.c_str() );
    return;
  }

  // DALi is multi-threaded so timed events will occur asynchronously
  std::string markerName;

  int cookie = GetCookie(traceMessage, markerName );

  if( eventType == PerformanceMarker::START_TIMED_EVENT )
  {
    traceAsyncBegin( TTRACE_TAG_GRAPHICS, cookie,  markerName.c_str() );
  }
  else
  {
    traceAsyncEnd( TTRACE_TAG_GRAPHICS, cookie,  markerName.c_str() );
  }
}

} // namespace Internal

} // namespace Adaptor

} // namespace Dali

