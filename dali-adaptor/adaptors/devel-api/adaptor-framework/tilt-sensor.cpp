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
#include "tilt-sensor.h"

// INTERNAL INCLUDES
#include <tilt-sensor-impl.h>
#include <adaptor-impl.h>

namespace Dali
{

const float TiltSensor::DEFAULT_UPDATE_FREQUENCY = 60.0f;

TiltSensor::TiltSensor()
{
}

TiltSensor TiltSensor::Get()
{
  return Internal::Adaptor::TiltSensor::Get();
}

TiltSensor::~TiltSensor()
{
}

bool TiltSensor::Enable()
{
  return GetImplementation(*this).Enable();
}

void TiltSensor::Disable()
{
  GetImplementation(*this).Disable();
}

bool TiltSensor::IsEnabled() const
{
  return GetImplementation(*this).IsEnabled();
}

float TiltSensor::GetRoll() const
{
  return GetImplementation(*this).GetRoll();
}

float TiltSensor::GetPitch() const
{
  return GetImplementation(*this).GetPitch();
}

Quaternion TiltSensor::GetRotation() const
{
  return GetImplementation(*this).GetRotation();
}

TiltSensor::TiltedSignalType& TiltSensor::TiltedSignal()
{
  return GetImplementation(*this).TiltedSignal();
}

void TiltSensor::SetUpdateFrequency( float frequencyHertz )
{
  GetImplementation(*this).SetUpdateFrequency( frequencyHertz );
}

float TiltSensor::GetUpdateFrequency() const
{
  return GetImplementation(*this).GetUpdateFrequency();
}

void TiltSensor::SetRotationThreshold(Radian rotationThreshold)
{
  GetImplementation(*this).SetRotationThreshold( rotationThreshold );
}

Radian TiltSensor::GetRotationThreshold() const
{
  return GetImplementation(*this).GetRotationThreshold();
}

TiltSensor::TiltSensor( Internal::Adaptor::TiltSensor* sensor )
: BaseHandle( sensor )
{
}

} // namespace Dali
