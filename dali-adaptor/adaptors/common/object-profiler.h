#ifndef __DALI_ADAPTOR_OBJECT_PROFILER_H__
#define __DALI_ADAPTOR_OBJECT_PROFILER_H__

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
#include <dali/public-api/object/object-registry.h>
#include <dali/public-api/object/type-registry.h>
#include <dali/devel-api/common/map-wrapper.h>
#include <dali/public-api/signals/connection-tracker.h>

// INTERNAL INCLUDES
#include <timer.h>

namespace Dali
{
namespace Internal
{
namespace Adaptor
{

/**
 * Class to profile the number of instances of Objects in the system
 */
class ObjectProfiler : public ConnectionTracker
{
public:

  /**
   * Constructor
   * @param timeInterval to specify the frequency of reporting
   */
  ObjectProfiler( unsigned int timeInterval );

  /**
   * Destructor
   */
  ~ObjectProfiler();

  /**
   * Display a list of types with the current number of instances in the system
   */
  void DisplayInstanceCounts();

private:
  /**
   * If timer is running, display the instance counts
   */
  bool OnTimeout();

  /**
   * Callback used when objects are created. Increases instance count for that object type
   * @param[in] handle of the created object
   */
  void OnObjectCreated(BaseHandle handle);

  /**
   * Callback used when objects are created. Decreases instance count for that object type
   * @param[in] object The object being destroyed
   */
  void OnObjectDestroyed(const Dali::RefObject* object);

  /**
   * Get the memory size of the given object
   */
  int GetMemorySize(const std::string& name, int count);

private:
  typedef std::map<std::string, int> InstanceCountMap;
  typedef std::pair<const std::string, int> InstanceCountPair;
  typedef InstanceCountMap::iterator InstanceCountMapIterator;
  typedef std::pair<BaseObject*, std::string> InstanceTypePair;
  typedef std::vector<InstanceTypePair> InstanceTypes;

  Dali::ObjectRegistry    mObjectRegistry;
  Dali::Timer             mTimer;
  InstanceCountMap        mInstanceCountMap;
  InstanceTypes           mInstanceTypes;
};

} // Adaptor
} // Internal
} // Dali

#endif // __DALI_ADAPTOR_OBJECT_PROFILER_H__
