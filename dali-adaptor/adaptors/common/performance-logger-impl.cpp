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
#include <performance-logger-impl.h>

// INTERNAL INCLUDES
#include <adaptor-impl.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{

PerformanceInterface* GetPerformanceInterface()
{
  if( Adaptor::IsAvailable() )
  {
    return Internal::Adaptor::Adaptor::GetImplementation( Adaptor::Get() ).GetPerformanceInterface();
  }

  return NULL;
}

} // Anonymous namespace

PerformanceLoggerPtr PerformanceLogger::New( const char* name )
{
  PerformanceLoggerPtr logger = new PerformanceLogger( name );
  return logger;
}

PerformanceLogger::PerformanceLogger( const char* name )
: mContext( 0 )
{
  PerformanceInterface* performance = GetPerformanceInterface();
  if( performance )
  {
    mContext = performance->AddContext( name );
  }
}

PerformanceLogger::~PerformanceLogger()
{
  PerformanceInterface* performance = GetPerformanceInterface();
  if( performance )
  {
    performance->RemoveContext( mContext );
  }
}

void PerformanceLogger::AddMarker( Dali::PerformanceLogger::Marker markerType )
{
  PerformanceInterface* performance = GetPerformanceInterface();
  if( performance )
  {
    PerformanceInterface::MarkerType newMarkerType = PerformanceInterface::START;
    switch( markerType )
    {
      case Dali::PerformanceLogger::START_EVENT:
      {
        newMarkerType = PerformanceInterface::START;
        break;
      }
      case Dali::PerformanceLogger::END_EVENT:
      {
        newMarkerType = PerformanceInterface::END;
        break;
      }
    }

    performance->AddMarker( newMarkerType, mContext );
  }
}

void PerformanceLogger::SetLoggingFrequency( unsigned int logFrequency)
{
  PerformanceInterface* performance = GetPerformanceInterface();
  if( performance )
  {
    performance->SetLoggingFrequency( logFrequency, mContext );
  }
}

void PerformanceLogger::EnableLogging( bool enable )
{
  PerformanceInterface* performance = GetPerformanceInterface();
  if( performance )
  {
    performance->EnableLogging( enable, mContext );
  }
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
