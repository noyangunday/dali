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

// STRUCT HEADER
#include "frame-time-stats.h"

// EXTERNAL INCLUDES
#include <cmath>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{
const float EPSILON = 0.9f; // rolling average = (average * epsilon) + (current * epsilon)
const float ONE_OVER_MICROSECONDS_TO_SECONDS = 1.f / 1000000.f; ///< microseconds per second
}

FrameTimeStats::FrameTimeStats()
: mTotal( 0.f)
{
  mSamples.Reserve( 16 );   // Fill out a little to avoid early reallocations

  Reset();
}

FrameTimeStats::~FrameTimeStats()
{
}

void FrameTimeStats::StartTime( const FrameTimeStamp& timeStamp )
{
  // check to make sure we don't get 2 start times in a row
  if( mTimeState != WAITING_FOR_START_TIME )
  {
    Reset();
  }

  mStart = timeStamp;
  mTimeState = WAITING_FOR_END_TIME;
}

void FrameTimeStats::EndTime( const FrameTimeStamp& timeStamp )
{
  if( mTimeState != WAITING_FOR_END_TIME )
  {
    Reset();
    return;
  }

  mTimeState = WAITING_FOR_START_TIME;
  mRunCount++;

  // frame time in seconds
  unsigned int elapsedTime = FrameTimeStamp::MicrosecondDiff( mStart, timeStamp);

  mSamples.PushBack( elapsedTime );

  // if the min and max times haven't been set, do that now.
  if( !mMinMaxTimeSet )
  {
    mMin = elapsedTime;
    mMax = elapsedTime;
    mMinMaxTimeSet = true;
  }
  else
  {
    if (elapsedTime < mMin)
    {
      mMin= elapsedTime;
    }
    else if (elapsedTime > mMax)
    {
      mMax = elapsedTime;
    }
  }

  mTotal += elapsedTime;
}

void FrameTimeStats::Reset()
{
  mTimeState = WAITING_FOR_START_TIME;
  mMinMaxTimeSet = false;
  mMin = 0.f;
  mMax = 0.f;
  mRunCount = 0;
  mSamples.Clear();
}

float FrameTimeStats::GetMaxTime() const
{
  return mMax * ONE_OVER_MICROSECONDS_TO_SECONDS;
}

float FrameTimeStats::GetMinTime() const
{
  return mMin * ONE_OVER_MICROSECONDS_TO_SECONDS;
}

float FrameTimeStats::GetTotalTime() const
{
  return mTotal * ONE_OVER_MICROSECONDS_TO_SECONDS;
}

unsigned int FrameTimeStats::GetRunCount() const
{
  return mRunCount;
}

void FrameTimeStats::CalculateMean( float& meanOut, float& standardDeviationOut ) const
{
  if( mSamples.Size() > 0 )
  {
    // Mean
    unsigned int sum = 0;
    for( Samples::ConstIterator it = mSamples.Begin(), itEnd = mSamples.End(); it != itEnd; ++it )
    {
      unsigned int value = *it;

      sum += value;
    }

    meanOut = static_cast<float>(sum) / mSamples.Size();

    // Variance
    float variance = 0.0f;
    for( Samples::ConstIterator it = mSamples.Begin(), itEnd = mSamples.End(); it != itEnd; ++it )
    {
      unsigned int value = *it;

      float difference = static_cast<float>(value) - meanOut;

      variance += difference * difference;
    }

    variance /= mSamples.Size();

    // Standard deviation
    standardDeviationOut = sqrtf( variance );

    meanOut *= ONE_OVER_MICROSECONDS_TO_SECONDS;
    standardDeviationOut *= ONE_OVER_MICROSECONDS_TO_SECONDS;
  }
  else
  {
    meanOut = 0.0f;
    standardDeviationOut = 0.0f;
  }
}


} // namespace Adaptor

} // namespace Internal

} // namespace Dali
