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
#include "stat-context-manager.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{
const char* const UPDATE_CONTEXT_NAME = "Update";
const char* const RENDER_CONTEXT_NAME = "Render";
const char* const EVENT_CONTEXT_NAME = "Event";
const unsigned int DEFAULT_LOG_FREQUENCY = 2;
}

StatContextManager::StatContextManager( StatContextLogInterface& logInterface )
: mLogInterface( logInterface ),
  mNextContextId( 0 ),
  mStatisticsLogBitmask(0),
  mLogFrequency( DEFAULT_LOG_FREQUENCY )
{

  mStatContexts.Reserve(4); // intially reserve enough for 3 internal + 1 custom

  // Add defaults
  mUpdateStats = AddContext( UPDATE_CONTEXT_NAME, PerformanceMarker::UPDATE );
  mRenderStats = AddContext( RENDER_CONTEXT_NAME, PerformanceMarker::RENDER );
  mEventStats = AddContext( EVENT_CONTEXT_NAME,   PerformanceMarker::EVENT_PROCESS );

}

StatContextManager::~StatContextManager()
{
  for( StatContexts::Iterator it = mStatContexts.Begin(), itEnd = mStatContexts.End(); it != itEnd; ++it )
  {
    StatContext* context = *it;
    delete context;
  }
  mStatContexts.Clear();
}
PerformanceInterface::ContextId StatContextManager::AddContext( const char* const name,
                                                                PerformanceMarker::MarkerFilter type  )
{
  unsigned int contextId = mNextContextId++;

  DALI_ASSERT_DEBUG( NULL == GetContext( contextId ) );

  // logging enabled by default
  StatContext* statContext = new StatContext( contextId, name, type,  mLogFrequency , mLogInterface );

  // check to see if custom markers are enabled
  if( ! ( mStatisticsLogBitmask & PerformanceInterface::LOG_CUSTOM_MARKERS ) )
  {
    statContext->EnableLogging( false );
  }

  mStatContexts.PushBack( statContext );

  return contextId;
}

void StatContextManager::AddInternalMarker( const PerformanceMarker& marker )
{
  // log to the stat contexts, can be called from multiple threads so we
  // protect the data
  Mutex::ScopedLock lock( mDataMutex );
  for( StatContexts::Iterator it = mStatContexts.Begin(), itEnd = mStatContexts.End(); it != itEnd; ++it )
  {
    StatContext* context = *it;
    context->ProcessInternalMarker( marker );
  }
}

void StatContextManager::AddCustomMarker( const PerformanceMarker& marker, PerformanceInterface::ContextId contextId )
{
  // log to the stat contexts, can be called from multiple threads so we
  // protect the data
  Mutex::ScopedLock lock( mDataMutex );
  StatContext* context = GetContext( contextId );
  if( context )
  {
    context->ProcessCustomMarker( marker );
  }
}

void StatContextManager::RemoveContext(PerformanceInterface::ContextId contextId )
{
  for( StatContexts::Iterator it = mStatContexts.Begin(), itEnd = mStatContexts.End(); it != itEnd; ++it )
  {
    StatContext* context = *it;

    if( context->GetId() == contextId )
    {
      delete context;
      mStatContexts.Erase( it );
      return;
    }
  }
}


void StatContextManager::EnableLogging( bool enable, PerformanceInterface::ContextId contextId )
{
  StatContext* context = GetContext( contextId );
  if( context )
  {
    context->EnableLogging( enable );
  }
}

void StatContextManager::SetLoggingLevel(  unsigned int statisticsLogOptions, unsigned int logFrequency)
{
  mStatisticsLogBitmask = statisticsLogOptions;

  if( mStatisticsLogBitmask == PerformanceInterface::LOG_EVERYTHING )
  {
    mStatisticsLogBitmask = 0xFFFFFFFF; // enable everything
  }

  mLogFrequency = logFrequency;

  // currently uses DALI_LOG_PERFORMANCE_STATS_FREQ environment variable to determine to log frequency
  // if it's not set it will be zero
  if( mLogFrequency == 0 )
  {
    mLogFrequency = DEFAULT_LOG_FREQUENCY;
  }
  EnableLogging( mStatisticsLogBitmask & PerformanceInterface::LOG_UPDATE_RENDER, mUpdateStats );
  EnableLogging( mStatisticsLogBitmask & PerformanceInterface::LOG_UPDATE_RENDER, mRenderStats );
  EnableLogging( mStatisticsLogBitmask & PerformanceInterface::LOG_EVENT_PROCESS, mEventStats );

  for( StatContexts::Iterator it = mStatContexts.Begin(), itEnd = mStatContexts.End(); it != itEnd; ++it )
  {
     StatContext* context = *it;
     context->SetLogFrequency( mLogFrequency );
  }
}

void StatContextManager::SetLoggingFrequency( unsigned int logFrequency,
                                              PerformanceInterface::ContextId contextId  )
{
  StatContext* context = GetContext( contextId );
  if( context )
  {
    if( logFrequency == 0 )
    {
      logFrequency = DEFAULT_LOG_FREQUENCY;
    }
    context->SetLogFrequency( logFrequency );
  }
}
const char* const StatContextManager::GetContextName(PerformanceInterface::ContextId contextId) const
{
  StatContext* context = GetContext(contextId);
  if( context )
  {
    return context->GetName();
  }
  return "context not found";
}

const char* const StatContextManager::GetMarkerDescription( PerformanceInterface::MarkerType type, PerformanceInterface::ContextId contextId ) const
{
  StatContext* context = GetContext(contextId);
  if( context )
  {
    return context->GetMarkerDescription( type );
  }
  return "context not found";
}


StatContext* StatContextManager::GetContext( PerformanceInterface::ContextId contextId ) const
{
  for( StatContexts::Iterator it = mStatContexts.Begin(), itEnd = mStatContexts.End(); it != itEnd; ++it )
  {
    StatContext* context = *it;

    if( context->GetId() == contextId )
    {
      return context;
    }
  }

  return NULL;
}


} // namespace Internal

} // namespace Adaptor

} // namespace Dali


