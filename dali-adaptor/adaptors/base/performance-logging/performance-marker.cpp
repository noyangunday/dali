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
#include "performance-marker.h"

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

struct NamePair
{
  PerformanceInterface::MarkerType type;
  const char* const name;
  PerformanceMarker::MarkerFilter group;
  PerformanceMarker::MarkerEventType eventType;
};

const NamePair MARKER_LOOKUP[] =
{
    // timed event names must be postfixed with with _START and _END
    // this is to allow tracers to extract the event name by removing the _START, _END strings
    //
    { PerformanceInterface::VSYNC       ,         "V_SYNC"               , PerformanceMarker::V_SYNC_EVENTS, PerformanceMarker::SINGLE_EVENT      },
    { PerformanceInterface::UPDATE_START ,        "UPDATE_START"         , PerformanceMarker::UPDATE,        PerformanceMarker::START_TIMED_EVENT },
    { PerformanceInterface::UPDATE_END   ,        "UPDATE_END"           , PerformanceMarker::UPDATE,        PerformanceMarker::END_TIMED_EVENT   },
    { PerformanceInterface::RENDER_START ,        "RENDER_START"         , PerformanceMarker::RENDER,        PerformanceMarker::START_TIMED_EVENT },
    { PerformanceInterface::RENDER_END   ,        "RENDER_END"           , PerformanceMarker::RENDER,        PerformanceMarker::END_TIMED_EVENT   },
    { PerformanceInterface::SWAP_START   ,        "SWAP_START"           , PerformanceMarker::SWAP_BUFFERS,  PerformanceMarker::START_TIMED_EVENT },
    { PerformanceInterface::SWAP_END     ,        "SWAP_END"             , PerformanceMarker::SWAP_BUFFERS,  PerformanceMarker::END_TIMED_EVENT   },
    { PerformanceInterface::PROCESS_EVENTS_START, "PROCESS_EVENT_START"  , PerformanceMarker::EVENT_PROCESS, PerformanceMarker::START_TIMED_EVENT },
    { PerformanceInterface::PROCESS_EVENTS_END,   "PROCESS_EVENT_END"    , PerformanceMarker::EVENT_PROCESS, PerformanceMarker::END_TIMED_EVENT   },
    { PerformanceInterface::PAUSED       ,        "PAUSED"               , PerformanceMarker::LIFE_CYCLE_EVENTS, PerformanceMarker::SINGLE_EVENT  },
    { PerformanceInterface::RESUME       ,        "RESUMED"              , PerformanceMarker::LIFE_CYCLE_EVENTS, PerformanceMarker::SINGLE_EVENT  },
    { PerformanceInterface::START        ,        "START"                , PerformanceMarker::CUSTOM_EVENTS, PerformanceMarker::START_TIMED_EVENT  },
    { PerformanceInterface::END          ,        "END"                  , PerformanceMarker::CUSTOM_EVENTS, PerformanceMarker::END_TIMED_EVENT  }
};
} // un-named namespace



PerformanceMarker::PerformanceMarker( PerformanceInterface::MarkerType type )
:mType(type)
{
}

PerformanceMarker::PerformanceMarker( PerformanceInterface::MarkerType type, FrameTimeStamp frameInfo )
:mType(type),
 mTimeStamp(frameInfo)
{
}

const char* const PerformanceMarker::GetName( ) const
{
  return MARKER_LOOKUP[ mType ].name;
}

PerformanceMarker::MarkerEventType PerformanceMarker::GetEventType() const
{
  return MARKER_LOOKUP[ mType ].eventType;
}

PerformanceMarker::MarkerFilter PerformanceMarker::GetFilterType() const
{
  return MARKER_LOOKUP[ mType ].group;
}

unsigned int PerformanceMarker::MicrosecondDiff( const PerformanceMarker& start,const PerformanceMarker& end )
{
  return FrameTimeStamp::MicrosecondDiff( start.mTimeStamp, end.mTimeStamp );
}

bool PerformanceMarker::IsFilterEnabled( MarkerFilter filter ) const
{
  return  (filter & MARKER_LOOKUP[ mType ].group);
}


} // namespace Adaptor

} // namespace Internal

} // namespace Dali

