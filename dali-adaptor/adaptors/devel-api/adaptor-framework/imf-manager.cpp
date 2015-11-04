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
#include <imf-manager.h>

// INTERNAL INCLUDES
#include <imf-manager-impl.h>
#include <adaptor-impl.h>

namespace Dali
{

ImfManager::ImfManager()
{
}

ImfManager::~ImfManager()
{
}

ImfManager ImfManager::Get()
{
  return Internal::Adaptor::ImfManager::Get();
}

void ImfManager::Activate()
{
  Internal::Adaptor::ImfManager::GetImplementation(*this).Activate();
}

void ImfManager::Deactivate()
{
  Internal::Adaptor::ImfManager::GetImplementation(*this).Deactivate();
}

bool ImfManager::RestoreAfterFocusLost() const
{
  return Internal::Adaptor::ImfManager::GetImplementation(*this).RestoreAfterFocusLost();
}

void ImfManager::SetRestoreAfterFocusLost( bool toggle )
{
  Internal::Adaptor::ImfManager::GetImplementation(*this).SetRestoreAfterFocusLost( toggle );
}

void ImfManager::Reset()
{
  Internal::Adaptor::ImfManager::GetImplementation(*this).Reset();
}

void ImfManager::NotifyCursorPosition()
{
  Internal::Adaptor::ImfManager::GetImplementation(*this).NotifyCursorPosition();
}

void ImfManager::SetCursorPosition( unsigned int SetCursorPosition )
{
  Internal::Adaptor::ImfManager::GetImplementation(*this).SetCursorPosition( SetCursorPosition );
}

unsigned int ImfManager::GetCursorPosition() const
{
  return Internal::Adaptor::ImfManager::GetImplementation(*this).GetCursorPosition();
}

void ImfManager::SetSurroundingText( const std::string& text )
{
  Internal::Adaptor::ImfManager::GetImplementation(*this).SetSurroundingText( text );
}

const std::string& ImfManager::GetSurroundingText() const
{
  return Internal::Adaptor::ImfManager::GetImplementation(*this).GetSurroundingText();
}

ImfManager::ImfManagerSignalType& ImfManager::ActivatedSignal()
{
  return Internal::Adaptor::ImfManager::GetImplementation(*this).ActivatedSignal();
}

ImfManager::ImfEventSignalType& ImfManager::EventReceivedSignal()
{
  return Internal::Adaptor::ImfManager::GetImplementation(*this).EventReceivedSignal();
}

ImfManager::ImfManager(Internal::Adaptor::ImfManager *impl)
  : BaseHandle(impl)
{
}

} // namespace Dali
