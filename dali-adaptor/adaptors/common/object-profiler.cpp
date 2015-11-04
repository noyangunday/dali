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
#include "object-profiler.h"

// EXTERNAL INCLUDES
#include <stdlib.h>
#include <dali/integration-api/debug.h>
#include <dali/integration-api/profiling.h>
#include <dali/public-api/actors/image-actor.h>
#include <dali/public-api/common/stage.h>
#include <dali/public-api/object/ref-object.h>
#include <dali/public-api/object/base-object.h>
#include <dali/public-api/object/type-registry.h>

using std::string;
using namespace Dali::Integration::Profiling;

namespace Dali
{
namespace Internal
{
namespace Adaptor
{

ObjectProfiler::ObjectProfiler( unsigned int timeInterval )
{
  // This class must be created after the Stage; this means it doesn't count the initial objects
  // that are created by the stage (base layer, default camera actor)
  mObjectRegistry = Dali::Stage::GetCurrent().GetObjectRegistry();

  mTimer = Dali::Timer::New( timeInterval * 1000 );
  mTimer.TickSignal().Connect( this, &ObjectProfiler::OnTimeout );
  mTimer.Start();

  mObjectRegistry.ObjectCreatedSignal().Connect( this, &ObjectProfiler::OnObjectCreated );
  mObjectRegistry.ObjectDestroyedSignal().Connect( this, &ObjectProfiler::OnObjectDestroyed );
}

ObjectProfiler::~ObjectProfiler()
{
}

void ObjectProfiler::DisplayInstanceCounts()
{
  InstanceCountMapIterator iter = mInstanceCountMap.begin();
  InstanceCountMapIterator end = mInstanceCountMap.end();

  for( ; iter != end; iter++ )
  {
    int memorySize = GetMemorySize(iter->first, iter->second);
    if( memorySize > 0 )
    {
      LogMessage( Debug::DebugInfo, "%-30s: % 4d  Memory MemorySize: ~% 6.1f kB\n",
                  iter->first.c_str(), iter->second, memorySize / 1024.0f );
    }
    else
    {
      LogMessage( Debug::DebugInfo, "%-30s: % 4d\n",
                  iter->first.c_str(), iter->second );
    }
  }
  LogMessage(Debug::DebugInfo, "\n");

  int quadCount = 0;

  // Count number of quads:

  for( InstanceTypes::iterator iter = mInstanceTypes.begin(), end = mInstanceTypes.end(); iter != end; ++iter )
  {
    if( iter->second.compare("ImageActor") == 0 )
    {
      BaseHandle handle(iter->first);
      Dali::ImageActor imageActor = Dali::ImageActor::DownCast(handle);
      if( imageActor )
      {
        if( imageActor.GetStyle() == Dali::ImageActor::STYLE_QUAD )
        {
          quadCount++;
        }
      }
    }
  }

  LogMessage(Debug::DebugInfo, "Number of image actors using Quad style: %d\n", quadCount);
}

bool ObjectProfiler::OnTimeout()
{
  DisplayInstanceCounts();
  return true;
}

void ObjectProfiler::OnObjectCreated(BaseHandle handle)
{
  string theType = handle.GetTypeName();
  if( theType.empty() )
  {
    DALI_LOG_ERROR("Object created from an unregistered type\n");
    theType = "<Unregistered>";
  }

  mInstanceTypes.push_back(InstanceTypePair(&handle.GetBaseObject(), theType));

  InstanceCountMapIterator iter = mInstanceCountMap.find(theType);
  if( iter == mInstanceCountMap.end() )
  {
    InstanceCountPair instanceCount(theType, 1);
    mInstanceCountMap.insert(instanceCount);
  }
  else
  {
    iter->second++;
  }
}

void ObjectProfiler::OnObjectDestroyed(const Dali::RefObject* object)
{
  const BaseObject* baseObject = static_cast<const BaseObject*>(object);

  InstanceTypes::iterator end = mInstanceTypes.end();
  for( InstanceTypes::iterator iter = mInstanceTypes.begin(); iter != end; iter++)
  {
    if( iter->first == baseObject )
    {
      const std::string& theType = iter->second;
      if( !theType.empty() )
      {
        InstanceCountMapIterator countIter = mInstanceCountMap.find(theType);
        if( countIter != mInstanceCountMap.end() )
        {
          countIter->second--;
        }
      }
      mInstanceTypes.erase( iter );
      break;
    }
  }
}

int ObjectProfiler::GetMemorySize(const std::string& name, int count)
{
  struct MemoryMemorySize
  {
    std::string name;
    int memorySize;
  };
  MemoryMemorySize memoryMemorySizes[] =
    {
      { "Animation", ANIMATION_MEMORY_SIZE },
      { "Constraint", CONSTRAINT_MEMORY_SIZE },
      { "Actor", ACTOR_MEMORY_SIZE },
      { "Layer", LAYER_MEMORY_SIZE },
      { "CameraActor", CAMERA_ACTOR_MEMORY_SIZE },
      { "ImageActor", IMAGE_ACTOR_MEMORY_SIZE },
      { "Image", IMAGE_MEMORY_SIZE },
      { "Renderer", RENDERER_MEMORY_SIZE },
      { "Geometry", GEOMETRY_MEMORY_SIZE },
      { "PropertyBuffer", PROPERTY_BUFFER_MEMORY_SIZE },
      { "Material", MATERIAL_MEMORY_SIZE },
      { "Sampler", SAMPLER_MEMORY_SIZE },
      { "Shader", SHADER_MEMORY_SIZE },
    };

  for( size_t i=0; i<sizeof(memoryMemorySizes)/sizeof(MemoryMemorySize); i++ )
  {
    if( memoryMemorySizes[i].name.compare(name) == 0 )
    {
      return count * memoryMemorySizes[i].memorySize;
    }
  }
  return 0;
}

} // Adaptor
} // Internal
} // Dali
