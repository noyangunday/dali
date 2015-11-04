#ifndef __DALI_INTERNAL_ADAPTOR_FRAME_TIME_STATS_H__
#define __DALI_INTERNAL_ADAPTOR_FRAME_TIME_STATS_H__

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
#include <base/performance-logging/frame-time-stamp.h>
#include <dali/public-api/common/dali-vector.h>


namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * Used to get statistics about time stamps over a period of time.
 * E.g. the min, max, total and average time spent inside two markers,
 * such as UPDATE_START and UPDATE_END
 */
struct FrameTimeStats
{
  /**
   * Constructor
   */
  FrameTimeStats();

  /**
   * Destructor, not intended as a base class
   */
  ~FrameTimeStats();

  /**
   * Timer start time
   * @param timeStamp time stamp
   */
   void StartTime( const FrameTimeStamp& timeStamp );

   /**
    * Timer end time
    * @param timeStamp time stamp
    */
   void EndTime( const FrameTimeStamp& timeStamp );

   /**
    * Reset all internal counters / state except total time.
    */
   void Reset();

   /**
    * @return maximum time in seconds
    */
   float GetMaxTime() const;

   /**
    * @return minimum time in seconds
    */
   float GetMinTime() const;

   /**
    * @return total time in second
    */
   float GetTotalTime() const;

   /**
    * Get how many times the timer has been started /stopped
    */
   unsigned int GetRunCount() const;

   /**
    * Calculate the mean and standard deviation
    *
    * @param[out] mean The return mean value
    * @param[out] standardDeviation The return standard deviation value
    */
   void CalculateMean( float& meanOut, float& standardDeviationOut ) const;

private:

   /**
    * internal time state.
    */
   enum TimeState
   {
     WAITING_FOR_START_TIME,    ///< waiting for start time marker
     WAITING_FOR_END_TIME       ///< waiting for end time marker
   };

   typedef Dali::Vector< unsigned int > Samples;
   Samples mSamples;

   unsigned int mMin;                  ///< current minimum value in microseconds
   unsigned int mMax;                  ///< current maximum value in microseconds
   unsigned int mTotal;                ///< current total in in microseconds
   unsigned int mRunCount;      ///< how many times the timer has been start / stopped
   FrameTimeStamp mStart;       ///< start time stamp, to calculate the diff
   TimeState mTimeState:1;      ///< time state
   bool mMinMaxTimeSet:1;       ///< whether the min-max values have been configured

};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_ADAPTOR_FRAME_TIME_STATS_H__

