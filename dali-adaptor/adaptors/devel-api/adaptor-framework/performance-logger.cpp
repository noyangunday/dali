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
#include <performance-logger.h>

// INTERNAL INCLUDES
#include <performance-logger-impl.h>

namespace Dali
{

PerformanceLogger::PerformanceLogger()
{
}

PerformanceLogger PerformanceLogger::New( const char* name )
{
  Internal::Adaptor::PerformanceLoggerPtr internal = Internal::Adaptor::PerformanceLogger::New( name );
  return PerformanceLogger(internal.Get());
}

PerformanceLogger::PerformanceLogger( const PerformanceLogger& performanceLogger )
: BaseHandle(performanceLogger)
{
}

PerformanceLogger& PerformanceLogger::operator=( const PerformanceLogger& performanceLogger )
{
  // check self assignment
  if( *this != performanceLogger )
  {
    BaseHandle::operator=(performanceLogger);
  }
  return *this;
}

PerformanceLogger::~PerformanceLogger()
{
}

PerformanceLogger PerformanceLogger::DownCast( BaseHandle handle )
{
  return PerformanceLogger( dynamic_cast<Internal::Adaptor::PerformanceLogger*>( handle.GetObjectPtr() ) );
}

void PerformanceLogger::AddMarker( Marker markerType )
{
  Internal::Adaptor::GetImplementation(*this).AddMarker( markerType );
}

void PerformanceLogger::SetLoggingFrequency( unsigned int logFrequency)
{
  Internal::Adaptor::GetImplementation(*this).SetLoggingFrequency( logFrequency );
}

void PerformanceLogger::EnableLogging( bool enable )
{
  Internal::Adaptor::GetImplementation(*this).EnableLogging( enable );
}

PerformanceLogger::PerformanceLogger(Internal::Adaptor::PerformanceLogger* PerformanceLogger)
: BaseHandle(PerformanceLogger)
{
}

} // namespace Dali
