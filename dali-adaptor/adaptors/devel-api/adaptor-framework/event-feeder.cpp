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
#include <event-feeder.h>

// INTERNAL INCLUDES
#include <adaptor-impl.h>

namespace Dali
{

namespace EventFeeder
{

void FeedTouchPoint( TouchPoint& point, int timeStamp )
{
  if ( Adaptor::IsAvailable() )
  {
    Internal::Adaptor::Adaptor::GetImplementation( Adaptor::Get() ).FeedTouchPoint( point, timeStamp );
  }
}

void FeedWheelEvent( WheelEvent& wheelEvent )
{
  if ( Adaptor::IsAvailable() )
  {
    Internal::Adaptor::Adaptor::GetImplementation( Adaptor::Get() ).FeedWheelEvent( wheelEvent );
  }
}

void FeedKeyEvent( KeyEvent& keyEvent )
{
  if ( Adaptor::IsAvailable() )
  {
    Internal::Adaptor::Adaptor::GetImplementation( Adaptor::Get() ).FeedKeyEvent( keyEvent );
  }
}

} // namespace EventFeeder

} // namespace Dali
