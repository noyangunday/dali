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
#include "frame-time-stamp.h"

namespace
{
const unsigned int MICROSECONDS_PER_SECOND = 1000000; ///< 1000000 microseconds per second
}

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

FrameTimeStamp::FrameTimeStamp()
: frame(0),
  seconds(0),
  microseconds(0),
  bufferIndex(0)
{
}

FrameTimeStamp::FrameTimeStamp(unsigned int frame,
                               unsigned int seconds,
                               unsigned int microseconds,
                               unsigned int bufferIndex)
: frame( frame ),
  seconds( seconds ),
  microseconds( microseconds ),
  bufferIndex( bufferIndex )
{
}

FrameTimeStamp::FrameTimeStamp(unsigned int bufferIndex )
: frame( 0 ),
  seconds(  0 ),
  microseconds( 0 ),
  bufferIndex( bufferIndex )
{
}

unsigned int FrameTimeStamp::MicrosecondDiff( const FrameTimeStamp& start,const FrameTimeStamp& end )
{
  int microDiff = end.microseconds - start.microseconds;
  unsigned int secDiff = ( end.seconds - start.seconds ) * MICROSECONDS_PER_SECOND;
  return ( microDiff + secDiff);
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

