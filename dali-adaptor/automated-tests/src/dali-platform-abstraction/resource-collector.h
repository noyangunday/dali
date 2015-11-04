#ifndef __DALI_ADAPTOR_TCT_RESOURCE_COLLECTOR_H_
#define __DALI_ADAPTOR_TCT_RESOURCE_COLLECTOR_H_
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
#include <dali/dali.h>
#include <dali/integration-api/resource-cache.h>

#include <map>

namespace Dali
{

namespace Integration
{
class PlatformAbstraction;
}

namespace Internal
{
namespace Platform
{
  /** Stores true for success and false for a failure for each completed
   *  resource id.*/
  typedef std::map<Integration::ResourceId, bool> ResourceStatusMap;
  /** Stores an integer counter for a resource ID, e.g., to count the number of
   *  times a load or a fail is reported.*/
  typedef std::map<Integration::ResourceId, unsigned> ResourceCounterMap;
  /** Used to track the order in which a sequence of requests is completed.*/
  typedef std::vector<Integration::ResourceId> ResourceSequence;

/**
 * @brief Used for platform testing to record the result of resource requests
 * initiated by tests.
 */
class ResourceCollector : public Integration::ResourceCache
{
public:

  ResourceCollector();
  virtual ~ResourceCollector();

  virtual void LoadResponse(Dali::Integration::ResourceId id, Dali::Integration::ResourceTypeId type, Dali::Integration::ResourcePointer resource, Dali::Integration::LoadStatus status);

  virtual void LoadFailed(Dali::Integration::ResourceId id, Dali::Integration::ResourceFailure failure);

  // Data:
  /** Record of the status of each completed resource. */
  ResourceStatusMap mCompletionStatuses;
  /** Record of how many times each resource completed (every value should be 1,
   *  else we are broken). */
  ResourceCounterMap mCompletionCounts;
  /** Record of how many times each resource succeeded (every value should be 0 or
   *  1, else we are broken). */
  ResourceCounterMap mSuccessCounts;
  /** Record of how many times each resource failed (every value should be 0 or 1,
   * else we are broken).
   * Only resource IDs that correspond to deliberately unloadable resources
   * should have counts other than 0. */
  ResourceCounterMap mFailureCounts;
  /** Remember the order of request completions so request priority can be tested. */
  ResourceSequence mCompletionSequence;
  /** Count of all successes and failures.*/
  unsigned mGrandTotalCompletions;
  /** Count of all successes, failures, loading notifications and partially loaded notifications.*/
  unsigned mGrandTotalNotifications;

};

/**
 * Helper to poll the abstraction for notifications assuming loads have been
 * issued to it previously and are in-flight.
 */
void PollForNotification( ResourceCollector& collector, Integration::PlatformAbstraction&  abstraction, const unsigned maxPolls = 100 );

} /* namespace Platform */
} /* namespace Internal */
} /* namespace Dali */

#endif /* __DALI_ADAPTOR_TCT_RESOURCE_COLLECTOR_H_ */
