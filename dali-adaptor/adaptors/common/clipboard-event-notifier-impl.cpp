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
#include "clipboard-event-notifier-impl.h"

// EXTERNAL INCLUDES
#include <dali/public-api/object/type-registry.h>

// INTERNAL INCLUDES
#include <singleton-service-impl.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{
BaseHandle Create()
{
  BaseHandle handle( ClipboardEventNotifier::Get() );

  if ( !handle )
  {
    Dali::SingletonService service( SingletonService::Get() );
    if ( service )
    {
      Dali::ClipboardEventNotifier notifier( ClipboardEventNotifier::New() );
      service.Register( typeid( notifier ), notifier );
      handle = notifier;
    }
  }

  return handle;
}
TypeRegistration CLIPBOARD_EVENT_NOTIFIER_TYPE( typeid(Dali::ClipboardEventNotifier), typeid(Dali::BaseHandle), Create, true /* Create Instance At Startup */ );

} // unnamed namespace

Dali::ClipboardEventNotifier ClipboardEventNotifier::New()
{
  Dali::ClipboardEventNotifier notifier = Dali::ClipboardEventNotifier(new ClipboardEventNotifier());

  return notifier;
}

Dali::ClipboardEventNotifier ClipboardEventNotifier::Get()
{
  Dali::ClipboardEventNotifier notifier;

  Dali::SingletonService service( SingletonService::Get() );
  if ( service )
  {
    // Check whether the singleton is already created
    Dali::BaseHandle handle = service.GetSingleton( typeid( Dali::ClipboardEventNotifier ) );
    if(handle)
    {
      // If so, downcast the handle
      notifier = Dali::ClipboardEventNotifier( dynamic_cast< ClipboardEventNotifier* >( handle.GetObjectPtr() ) );
    }
  }

  return notifier;
}

const std::string& ClipboardEventNotifier::GetContent() const
{
  return mContent;
}

void ClipboardEventNotifier::SetContent( const std::string& content )
{
  mContent = content;
}

void ClipboardEventNotifier::ClearContent()
{
  mContent.clear();
}

void ClipboardEventNotifier::EmitContentSelectedSignal()
{
  if ( !mContentSelectedSignal.Empty() )
  {
    Dali::ClipboardEventNotifier handle( this );
    mContentSelectedSignal.Emit( handle );
  }
}

ClipboardEventNotifier::ClipboardEventNotifier()
: mContent()
{
}

ClipboardEventNotifier::~ClipboardEventNotifier()
{
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
