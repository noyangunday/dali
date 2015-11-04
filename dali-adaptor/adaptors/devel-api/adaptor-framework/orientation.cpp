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
#include <orientation.h>

// INTERNAL INCLUDES
#include <orientation-impl.h>

namespace Dali
{

Orientation::Orientation()
{
}

Orientation::~Orientation()
{
}

Orientation::Orientation(const Orientation& handle)
: BaseHandle(handle)
{
}

Orientation& Orientation::operator=(const Orientation& rhs)
{
  BaseHandle::operator=(rhs);
  return *this;
}

int Orientation::GetDegrees() const
{
  return Internal::Adaptor::GetImplementation(*this).GetDegrees();
}

float Orientation::GetRadians() const
{
  return Internal::Adaptor::GetImplementation(*this).GetRadians();
}

Orientation::OrientationSignalType& Orientation::ChangedSignal()
{
  return Internal::Adaptor::GetImplementation(*this).ChangedSignal();
}

Orientation::Orientation( Internal::Adaptor::Orientation* orientation )
: BaseHandle(orientation)
{
}

} // namespace Dali
