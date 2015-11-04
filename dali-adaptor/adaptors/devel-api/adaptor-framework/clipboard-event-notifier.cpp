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
#include <clipboard-event-notifier.h>

// INTERNAL INCLUDES
#include <clipboard-event-notifier-impl.h>

namespace Dali
{

ClipboardEventNotifier::ClipboardEventNotifier()
{
}

ClipboardEventNotifier ClipboardEventNotifier::Get()
{
  return Internal::Adaptor::ClipboardEventNotifier::Get();
}

ClipboardEventNotifier::~ClipboardEventNotifier()
{
}

const std::string& ClipboardEventNotifier::GetContent() const
{
  return Internal::Adaptor::ClipboardEventNotifier::GetImplementation(*this).GetContent();
}

void ClipboardEventNotifier::SetContent( const std::string& content )
{
  Internal::Adaptor::ClipboardEventNotifier::GetImplementation(*this).SetContent(content);
}

void ClipboardEventNotifier::ClearContent()
{
  Internal::Adaptor::ClipboardEventNotifier::GetImplementation(*this).ClearContent();
}

void ClipboardEventNotifier::EmitContentSelectedSignal()
{
  Internal::Adaptor::ClipboardEventNotifier::GetImplementation(*this).EmitContentSelectedSignal();
}

ClipboardEventNotifier::ClipboardEventSignalType& ClipboardEventNotifier::ContentSelectedSignal()
{
  return Internal::Adaptor::ClipboardEventNotifier::GetImplementation(*this).ContentSelectedSignal();
}

ClipboardEventNotifier::ClipboardEventNotifier( Internal::Adaptor::ClipboardEventNotifier* notifier )
: BaseHandle( notifier )
{
}

} // namespace Dali
