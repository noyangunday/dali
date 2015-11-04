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

#include "resource-collector.h"
#include "tizen-platform-abstraction.h"
#include <dali/integration-api/debug.h>
#include <unistd.h>

namespace Dali
{
namespace Internal
{
namespace Platform
{
using namespace Dali::Integration;

ResourceCollector::ResourceCollector() :
  mGrandTotalCompletions(0),
  mGrandTotalNotifications(0)
{
}

ResourceCollector::~ResourceCollector() {}

void ResourceCollector::LoadResponse( Dali::Integration::ResourceId id, Dali::Integration::ResourceTypeId type, Dali::Integration::ResourcePointer resource, Dali::Integration::LoadStatus status )
{
  ++mGrandTotalNotifications;
  if( status == RESOURCE_COMPLETELY_LOADED )
  {
    DALI_ASSERT_DEBUG( mCompletionCounts.find(id) == mCompletionCounts.end() && "A resource can only complete once." );
    mCompletionStatuses[id] = true;
    ++mCompletionCounts[id];
    ++mSuccessCounts[id];
    mCompletionSequence.push_back( id );
    ++mGrandTotalCompletions;
  }
}

void ResourceCollector::LoadFailed( Dali::Integration::ResourceId id, Dali::Integration::ResourceFailure failure )
{
  mCompletionStatuses[id] = false;
  ++mFailureCounts[id];
  mCompletionSequence.push_back( id );
  ++mGrandTotalCompletions;
  ++mGrandTotalNotifications;
}

void PollForNotification( ResourceCollector& collector, Integration::PlatformAbstraction&  abstraction, const unsigned maxPolls )
{
  // Poll for at least one completed or partially completed load:
  const unsigned outstandingNotifications = collector.mGrandTotalNotifications;
  for( unsigned poll = 0; poll < maxPolls; ++poll )
  {
    abstraction.GetResources( collector );
    if( collector.mGrandTotalNotifications > outstandingNotifications )
    {
      break;
    }
    usleep( 3 ); //< Wait 3 microseconds each time around.
  }
}

} /* namespace Platform */
} /* namespace Internal */
} /* namespace Dali */
