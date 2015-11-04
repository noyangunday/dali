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
#include <physical-keyboard.h>

// INTERNAL INCLUDES
#include <physical-keyboard-impl.h>

namespace Dali
{

PhysicalKeyboard::PhysicalKeyboard()
{
}

PhysicalKeyboard::~PhysicalKeyboard()
{
}

PhysicalKeyboard PhysicalKeyboard::Get()
{
  // Get the physical keyboard handle
  PhysicalKeyboard handle = Internal::Adaptor::PhysicalKeyboard::Get();

  // If it's not been created then create one
  if ( !handle )
  {
    handle = Internal::Adaptor::PhysicalKeyboard::New();
  }

  return handle;
}

bool PhysicalKeyboard::IsAttached() const
{
  return GetImplementation( *this ).IsAttached();
}

PhysicalKeyboard::PhysicalKeyboardSignalType& PhysicalKeyboard::StatusChangedSignal()
{
  return GetImplementation( *this ).StatusChangedSignal();
}

PhysicalKeyboard::PhysicalKeyboard( Internal::Adaptor::PhysicalKeyboard *impl )
: BaseHandle(impl)
{
}

} // namespace Dali
