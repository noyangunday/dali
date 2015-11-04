#ifndef __DALI_INTERNAL_ADAPTOR_PERFORMANCE_MARKER_H__
#define __DALI_INTERNAL_ADAPTOR_PERFORMANCE_MARKER_H__

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

// INTERNAL INCLUDES
#include <base/interfaces/performance-interface.h>
#include <base/performance-logging/frame-time-stamp.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * Marker used to record an event with a time stamp in Dali
 */
class PerformanceMarker
{
public:


  /**
   * Bitmask used to filter different types of markers based
   * on what group they belong to.
   */
  enum MarkerFilter
  {
    FILTERING_DISABLED   = 0,      ///< disabled
    V_SYNC_EVENTS        = 1 << 0, ///< v-sync
    UPDATE               = 1 << 1, ///< update start / end
    RENDER               = 1 << 2, ///< render start / end
    EVENT_PROCESS        = 1 << 3, ///< process events start / end
    SWAP_BUFFERS         = 1 << 4, ///< swap buffers start / end
    LIFE_CYCLE_EVENTS    = 1 << 5, ///< pause / resume
    RESOURCE_EVENTS      = 1 << 6, ///< resource events
    CUSTOM_EVENTS        = 1 << 7
  };

  /**
   * Marker event type
   */
  enum MarkerEventType
  {
    SINGLE_EVENT,           ///< event is something that has no duration associated with it
    START_TIMED_EVENT,      ///< start of a timed event
    END_TIMED_EVENT         ///< end of a timed event

  };


  /**
   * @brief Constructor
   * @param[in] type marker type
   */
  PerformanceMarker( PerformanceInterface::MarkerType type );

  /**
   * @brief Constructor
   * @param[in] type marker type
   * @param[in] time time stamp
   */
  PerformanceMarker( PerformanceInterface::MarkerType type,  FrameTimeStamp time );

  /**
   * @return the time stamp
   */
  const FrameTimeStamp& GetTimeStamp() const
  {
    return mTimeStamp;
  }

  /**
   * @return the type of marker
   */
  PerformanceInterface::MarkerType GetType() const
  {
    return mType;
  }

  /**
   * @return the event type of marker
   */
  MarkerEventType GetEventType() const;

  /**
   * @return the filter type of marker
   */
  MarkerFilter GetFilterType() const;


  /**
   * @return marker name
   */
  const char* const GetName( ) const;

  /**
   * @param start the start marker
   * @param end the end marker
   * @return difference in microseconds between two markers
   */
  static unsigned int MicrosecondDiff( const PerformanceMarker& start, const PerformanceMarker& end  );

  /**
   * @return if a marker is enabled as part of a group
   */
  bool IsFilterEnabled( MarkerFilter filter ) const;
private:

  PerformanceInterface::MarkerType mType;         ///< marker type
  FrameTimeStamp mTimeStamp;                      ///< frame time stamp

};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_ADAPTOR_PERFORMANCE_MARKER_H__
