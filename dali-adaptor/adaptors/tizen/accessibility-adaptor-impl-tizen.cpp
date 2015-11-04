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
#include "accessibility-adaptor-impl.h"

// EXTERNAL INCLUDES
#include <vconf.h>

#include <dali/public-api/object/type-registry.h>
#include <dali/integration-api/debug.h>
#include <dali/integration-api/events/touch-event-integ.h>
#include <dali/integration-api/events/hover-event-integ.h>
#include <dali/integration-api/events/gesture-requests.h>

// INTERNAL INCLUDES
#include <singleton-service-impl.h>
#include <system-settings.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{

// TODO: Update vconf-key.h ?
const char * DALI_VCONFKEY_SETAPPL_ACCESSIBILITY_DBUS_TTS = "db/setting/accessibility/atspi";

bool GetEnabledVConf()
{
  int isEnabled = 0;
  vconf_get_bool( DALI_VCONFKEY_SETAPPL_ACCESSIBILITY_DBUS_TTS, &isEnabled );

  if( isEnabled == 0 )
  {
    vconf_get_bool( VCONFKEY_SETAPPL_ACCESSIBILITY_TTS, &isEnabled );
  }

  return (bool)isEnabled;
}

#if defined(DEBUG_ENABLED)
Debug::Filter* gAccessibilityAdaptorLogFilter = Debug::Filter::New( Debug::NoLogging, false, "LOG_ACCESSIBILITY_ADAPTOR" );
#endif

void AccessibilityOnOffNotification(keynode_t* node, void* data)
{
  AccessibilityAdaptor* adaptor = static_cast<AccessibilityAdaptor*>( data );

  bool isEnabled = GetEnabledVConf();

  DALI_LOG_INFO( gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, isEnabled ? "ENABLED" : "DISABLED" );

  if( isEnabled )
  {
    adaptor->EnableAccessibility();
  }
  else
  {
    adaptor->DisableAccessibility();
  }
}

BaseHandle Create()
{
  BaseHandle handle( AccessibilityAdaptor::Get() );

  if ( !handle )
  {
    Dali::SingletonService service( SingletonService::Get() );
    if ( service )
    {
      Dali::AccessibilityAdaptor adaptor = Dali::AccessibilityAdaptor( new AccessibilityAdaptor() );
      service.Register( typeid( adaptor ), adaptor );
      handle = adaptor;
    }
  }

  return handle;
}
TypeRegistration ACCESSIBILITY_ADAPTOR_TYPE( typeid(Dali::AccessibilityAdaptor), typeid(Dali::BaseHandle), Create, true /* Create Instance At Startup */ );

} // unnamed namespace

Dali::AccessibilityAdaptor AccessibilityAdaptor::Get()
{
  Dali::AccessibilityAdaptor adaptor;

  Dali::SingletonService service( SingletonService::Get() );
  if ( service )
  {
    // Check whether the singleton is already created
    Dali::BaseHandle handle = service.GetSingleton( typeid( Dali::AccessibilityAdaptor ) );
    if(handle)
    {
      // If so, downcast the handle
      adaptor = Dali::AccessibilityAdaptor( dynamic_cast< AccessibilityAdaptor* >( handle.GetObjectPtr() ) );
    }
  }

  return adaptor;
}

Vector2 AccessibilityAdaptor::GetReadPosition() const
{
  return mReadPosition;
}

void AccessibilityAdaptor::SetActionHandler(AccessibilityActionHandler& handler)
{
  mActionHandler = &handler;
}

void AccessibilityAdaptor::SetGestureHandler(AccessibilityGestureHandler& handler)
{
  if( mAccessibilityGestureDetector )
  {
    mAccessibilityGestureDetector->SetGestureHandler(handler);
  }
}

bool AccessibilityAdaptor::HandleActionClearFocusEvent()
{
  bool ret = false;

  if( mActionHandler )
  {
    ret = mActionHandler->ClearAccessibilityFocus();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionScrollEvent(const TouchPoint& point, unsigned long timeStamp)
{
  bool ret = false;

  // We always need to emit a scroll signal, whether it's only a hover or not,
  // so always send the action to the action handler.
  if( mActionHandler )
  {
    Dali::TouchEvent event(timeStamp);
    event.points.push_back(point);
    ret = mActionHandler->AccessibilityActionScroll( event );
  }

  Integration::TouchEvent touchEvent;
  Integration::HoverEvent hoverEvent;
  Integration::TouchEventCombiner::EventDispatchType type = mCombiner.GetNextTouchEvent(point, timeStamp, touchEvent, hoverEvent);
  if(type == Integration::TouchEventCombiner::DispatchTouch || type == Integration::TouchEventCombiner::DispatchBoth) // hover event is ignored
  {
    // Process the touch event in accessibility gesture detector
    if( mAccessibilityGestureDetector )
    {
      mAccessibilityGestureDetector->SendEvent(touchEvent);
      ret = true;
    }
  }

  return ret;
}

bool AccessibilityAdaptor::HandleActionTouchEvent(const TouchPoint& point, unsigned long timeStamp)
{
  bool ret = false;

  Dali::TouchEvent touchEvent(timeStamp);
  touchEvent.points.push_back(point);

  if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionTouch(touchEvent);
  }
  return ret;
}

bool AccessibilityAdaptor::HandleActionBackEvent()
{
  bool ret = false;

  if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionBack();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

void AccessibilityAdaptor::HandleActionEnableEvent()
{
  EnableAccessibility();
}

void AccessibilityAdaptor::HandleActionDisableEvent()
{
  DisableAccessibility();
}

void AccessibilityAdaptor::EnableAccessibility()
{
  if(mIsEnabled == false)
  {
    mIsEnabled = true;

    if( mActionHandler )
    {
      mActionHandler->ChangeAccessibilityStatus();
    }
  }
}

void AccessibilityAdaptor::DisableAccessibility()
{
  if(mIsEnabled == true)
  {
    mIsEnabled = false;

    if( mActionHandler )
    {
      mActionHandler->ChangeAccessibilityStatus();
    }

    // Destroy the TtsPlayer if exists.
    if ( Adaptor::IsAvailable() )
    {
      Dali::Adaptor& adaptor = Dali::Adaptor::Get();
      Adaptor& adaptorIpml = Adaptor::GetImplementation( adaptor );
      adaptorIpml.DestroyTtsPlayer( Dali::TtsPlayer::SCREEN_READER );
    }
  }
}

bool AccessibilityAdaptor::IsEnabled() const
{
  return mIsEnabled;
}

void AccessibilityAdaptor::SetIndicator(Indicator* indicator)
{
  mIndicator = indicator;
}

AccessibilityAdaptor::AccessibilityAdaptor()
: mIsEnabled( false ),
  mReadPosition(),
  mActionHandler( NULL ),
  mIndicator( NULL),
  mIndicatorFocused( false )
{
  mIsEnabled = GetEnabledVConf();
  DALI_LOG_INFO( gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, mIsEnabled ? "ENABLED" : "DISABLED" );

  vconf_notify_key_changed( DALI_VCONFKEY_SETAPPL_ACCESSIBILITY_DBUS_TTS, AccessibilityOnOffNotification, this );
  vconf_notify_key_changed( VCONFKEY_SETAPPL_ACCESSIBILITY_TTS, AccessibilityOnOffNotification, this );

  mAccessibilityGestureDetector = new AccessibilityGestureDetector();
}

AccessibilityAdaptor::~AccessibilityAdaptor()
{
  vconf_ignore_key_changed( VCONFKEY_SETAPPL_ACCESSIBILITY_TTS, AccessibilityOnOffNotification );
  vconf_ignore_key_changed( DALI_VCONFKEY_SETAPPL_ACCESSIBILITY_DBUS_TTS, AccessibilityOnOffNotification );
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
