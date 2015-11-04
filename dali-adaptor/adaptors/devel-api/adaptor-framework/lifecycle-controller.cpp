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
#include <lifecycle-controller.h>

// INTERNAL INCLUDES
#include <lifecycle-controller-impl.h>

namespace Dali
{

LifecycleController::LifecycleController()
{
}

LifecycleController::LifecycleController(const LifecycleController& controller)
: BaseHandle(controller)
{
}

LifecycleController LifecycleController::Get()
{
  return Internal::Adaptor::LifecycleController::Get();
}

LifecycleController::~LifecycleController()
{
}

LifecycleController::LifecycleSignalType& LifecycleController::InitSignal()
{
  return GetImplementation(*this).InitSignal();
}

LifecycleController::LifecycleSignalType& LifecycleController::TerminateSignal()
{
  return GetImplementation(*this).TerminateSignal();
}

LifecycleController::LifecycleSignalType& LifecycleController::PauseSignal()
{
  return GetImplementation(*this).PauseSignal();
}

LifecycleController::LifecycleSignalType& LifecycleController::ResumeSignal()
{
  return GetImplementation(*this).ResumeSignal();
}

LifecycleController::LifecycleSignalType& LifecycleController::ResetSignal()
{
  return GetImplementation(*this).ResetSignal();
}

LifecycleController::LifecycleSignalType& LifecycleController::ResizeSignal()
{
  return GetImplementation(*this).ResizeSignal();
}

LifecycleController::LifecycleSignalType& LifecycleController::LanguageChangedSignal()
{
  return GetImplementation(*this).LanguageChangedSignal();
}

LifecycleController& LifecycleController::operator=(const LifecycleController& monitor)
{
  if( *this != monitor )
  {
    BaseHandle::operator=(monitor);
  }
  return *this;
}

LifecycleController::LifecycleController(Internal::Adaptor::LifecycleController* internal)
: BaseHandle(internal)
{
}

} // namespace Dali
