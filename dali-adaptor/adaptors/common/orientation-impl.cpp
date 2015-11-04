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
#include "orientation-impl.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <window-impl.h>
#include <adaptor-impl.h>

namespace Dali
{
namespace Internal
{
namespace Adaptor
{

Orientation* Orientation::New(Window* window)
{
  Orientation* orientation = new Orientation(window);

  return orientation;
}

Orientation::Orientation(Window* window)
: mWindow(window),
  mOrientation(0),
  mWindowWidth(0),
  mWindowHeight(0)
{
}

Orientation::~Orientation()
{
  // Note, there is only one orientation object that's owned by window,
  // so it will live longer than adaptor. (hence, no need to remove rotation observer)
}

void Orientation::SetAdaptor(Dali::Adaptor& adaptor)
{
  Adaptor& adaptorImpl = Adaptor::GetImplementation(adaptor);
  adaptorImpl.SetRotationObserver(this);
}

int Orientation::GetDegrees() const
{
  return mOrientation;
}

float Orientation::GetRadians() const
{
  return Math::PI * (float)mOrientation / 180.0f;
}

Orientation::OrientationSignalType& Orientation::ChangedSignal()
{
  return mChangedSignal;
}

void Orientation::OnRotationPrepare( const RotationEvent& rotation )
{
  mOrientation  = rotation.angle;
  mWindowWidth  = rotation.width;
  mWindowHeight = rotation.height;
}

void Orientation::OnRotationRequest()
{
  // Emit signal
  if( !mChangedSignal.Empty() )
  {
    Dali::Orientation handle( this );
    mChangedSignal.Emit( handle );
  }

  if( mWindow != NULL )
  {
    mWindow->RotationDone( mOrientation, mWindowWidth, mWindowHeight );
  }
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
