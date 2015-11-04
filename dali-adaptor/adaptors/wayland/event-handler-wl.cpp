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
#include <events/event-handler.h>

// EXTERNAL INCLUDES
#include <Ecore.h>
#include <Ecore_Input.h>
#include <ecore-wl-render-surface.h>
#include <cstring>

#include <sys/time.h>

#ifndef DALI_PROFILE_UBUNTU
#include <vconf.h>
#include <vconf-keys.h>
#endif // DALI_PROFILE_UBUNTU

#include <dali/public-api/common/vector-wrapper.h>
#include <dali/public-api/events/touch-point.h>
#include <dali/public-api/events/key-event.h>
#include <dali/public-api/events/wheel-event.h>
#include <dali/integration-api/debug.h>
#include <dali/integration-api/events/key-event-integ.h>
#include <dali/integration-api/events/touch-event-integ.h>
#include <dali/integration-api/events/hover-event-integ.h>
#include <dali/integration-api/events/wheel-event-integ.h>

// INTERNAL INCLUDES
#include <events/gesture-manager.h>
#include <window-render-surface.h>
#include <clipboard-impl.h>
#include <key-impl.h>
#include <physical-keyboard-impl.h>
#include <style-monitor-impl.h>
#include <base/core-event-interface.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

#if defined(DEBUG_ENABLED)
namespace
{
Integration::Log::Filter* gTouchEventLogFilter  = Integration::Log::Filter::New(Debug::NoLogging, false, "LOG_ADAPTOR_EVENTS_TOUCH");
Integration::Log::Filter* gClientMessageLogFilter  = Integration::Log::Filter::New(Debug::NoLogging, false, "LOG_ADAPTOR_EVENTS_CLIENT_MESSAGE");
Integration::Log::Filter* gDragAndDropLogFilter = Integration::Log::Filter::New(Debug::NoLogging, false, "LOG_ADAPTOR_EVENTS_DND");
Integration::Log::Filter* gImfLogging  = Integration::Log::Filter::New(Debug::NoLogging, false, "LOG_ADAPTOR_EVENTS_IMF");
Integration::Log::Filter* gSelectionEventLogFilter = Integration::Log::Filter::New(Debug::NoLogging, false, "LOG_ADAPTOR_EVENTS_SELECTION");
} // unnamed namespace
#endif


namespace
{
const unsigned int PRIMARY_TOUCH_BUTTON_ID( 1 );

const unsigned int BYTES_PER_CHARACTER_FOR_ATTRIBUTES = 3;

/**
 * Ecore_Event_Modifier enums in Ecore_Input.h do not match Ecore_IMF_Keyboard_Modifiers in Ecore_IMF.h.
 * This function converts from Ecore_Event_Modifier to Ecore_IMF_Keyboard_Modifiers enums.
 * @param[in] ecoreModifier the Ecore_Event_Modifier input.
 * @return the Ecore_IMF_Keyboard_Modifiers output.
 */
Ecore_IMF_Keyboard_Modifiers EcoreInputModifierToEcoreIMFModifier(unsigned int ecoreModifier)
{
   int modifier( ECORE_IMF_KEYBOARD_MODIFIER_NONE );  // If no other matches returns NONE.


   if ( ecoreModifier & ECORE_EVENT_MODIFIER_SHIFT )  // enums from ecore_input/Ecore_Input.h
   {
     modifier |= ECORE_IMF_KEYBOARD_MODIFIER_SHIFT;  // enums from ecore_imf/ecore_imf.h
   }

   if ( ecoreModifier & ECORE_EVENT_MODIFIER_ALT )
   {
     modifier |= ECORE_IMF_KEYBOARD_MODIFIER_ALT;
   }

   if ( ecoreModifier & ECORE_EVENT_MODIFIER_CTRL )
   {
     modifier |= ECORE_IMF_KEYBOARD_MODIFIER_CTRL;
   }

   if ( ecoreModifier & ECORE_EVENT_MODIFIER_WIN )
   {
     modifier |= ECORE_IMF_KEYBOARD_MODIFIER_WIN;
   }

   if ( ecoreModifier & ECORE_EVENT_MODIFIER_ALTGR )
   {
     modifier |= ECORE_IMF_KEYBOARD_MODIFIER_ALTGR;
   }

   return static_cast<Ecore_IMF_Keyboard_Modifiers>( modifier );
}


// Copied from x server
static unsigned int GetCurrentMilliSeconds(void)
{
  struct timeval tv;

  struct timespec tp;
  static clockid_t clockid;

  if (!clockid)
  {
#ifdef CLOCK_MONOTONIC_COARSE
    if (clock_getres(CLOCK_MONOTONIC_COARSE, &tp) == 0 &&
      (tp.tv_nsec / 1000) <= 1000 && clock_gettime(CLOCK_MONOTONIC_COARSE, &tp) == 0)
    {
      clockid = CLOCK_MONOTONIC_COARSE;
    }
    else
#endif
    if (clock_gettime(CLOCK_MONOTONIC, &tp) == 0)
    {
      clockid = CLOCK_MONOTONIC;
    }
    else
    {
      clockid = ~0L;
    }
  }
  if (clockid != ~0L && clock_gettime(clockid, &tp) == 0)
  {
    return (tp.tv_sec * 1000) + (tp.tv_nsec / 1000000L);
  }

  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

#ifndef DALI_PROFILE_UBUNTU
const char * DALI_VCONFKEY_SETAPPL_ACCESSIBILITY_FONT_SIZE = "db/setting/accessibility/font_name";  // It will be update at vconf-key.h and replaced.
#endif // DALI_PROFILE_UBUNTU

} // unnamed namespace

// Impl to hide EFL implementation.
struct EventHandler::Impl
{
  // Construction & Destruction

  /**
   * Constructor
   */
  Impl( EventHandler* handler, Ecore_Wl_Window* window )
  : mHandler( handler ),
    mEcoreEventHandler(),
    mWindow( window )
  {
    // Only register for touch and key events if we have a window
    if ( window != 0 )
    {
      // Register Touch events
      mEcoreEventHandler.push_back( ecore_event_handler_add( ECORE_EVENT_MOUSE_BUTTON_DOWN,  EcoreEventMouseButtonDown, handler ) );
      mEcoreEventHandler.push_back( ecore_event_handler_add( ECORE_EVENT_MOUSE_BUTTON_UP,    EcoreEventMouseButtonUp,   handler ) );
      mEcoreEventHandler.push_back( ecore_event_handler_add( ECORE_EVENT_MOUSE_MOVE,         EcoreEventMouseButtonMove, handler ) );
      mEcoreEventHandler.push_back( ecore_event_handler_add( ECORE_EVENT_MOUSE_OUT,          EcoreEventMouseButtonUp,   handler ) ); // process mouse out event like up event

      // Register Mouse wheel events
      mEcoreEventHandler.push_back( ecore_event_handler_add( ECORE_EVENT_MOUSE_WHEEL,        EcoreEventMouseWheel,      handler ) );

      // Register Key events
      mEcoreEventHandler.push_back( ecore_event_handler_add( ECORE_EVENT_KEY_DOWN,           EcoreEventKeyDown,         handler ) );
      mEcoreEventHandler.push_back( ecore_event_handler_add( ECORE_EVENT_KEY_UP,             EcoreEventKeyUp,           handler ) );

#ifndef DALI_PROFILE_UBUNTU
      // Register Vconf notify - font name and size
      vconf_notify_key_changed( DALI_VCONFKEY_SETAPPL_ACCESSIBILITY_FONT_SIZE, VconfNotifyFontNameChanged, handler );
      vconf_notify_key_changed( VCONFKEY_SETAPPL_ACCESSIBILITY_FONT_SIZE, VconfNotifyFontSizeChanged, handler );
#endif // DALI_PROFILE_UBUNTU
    }
  }

  /**
   * Destructor
   */
  ~Impl()
  {
#ifndef DALI_PROFILE_UBUNTU
    vconf_ignore_key_changed( VCONFKEY_SETAPPL_ACCESSIBILITY_FONT_SIZE, VconfNotifyFontSizeChanged );
    vconf_ignore_key_changed( DALI_VCONFKEY_SETAPPL_ACCESSIBILITY_FONT_SIZE, VconfNotifyFontNameChanged );
#endif // DALI_PROFILE_UBUNTU

    for( std::vector<Ecore_Event_Handler*>::iterator iter = mEcoreEventHandler.begin(), endIter = mEcoreEventHandler.end(); iter != endIter; ++iter )
    {
      ecore_event_handler_del( *iter );
    }
  }

  // Static methods

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Touch Callbacks
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /**
   * Called when a touch down is received.
   */
  static Eina_Bool EcoreEventMouseButtonDown( void* data, int type, void* event )
  {
    Ecore_Event_Mouse_Button *touchEvent( (Ecore_Event_Mouse_Button*)event );
    EventHandler* handler( (EventHandler*)data );

    if ( touchEvent->window == (unsigned int)ecore_wl_window_id_get(handler->mImpl->mWindow) )
    {
      TouchPoint::State state ( TouchPoint::Down );

      // Check if the buttons field is set and ensure it's the primary touch button.
      // If this event was triggered by buttons other than the primary button (used for touch), then
      // just send an interrupted event to Core.
      if ( touchEvent->buttons && (touchEvent->buttons != PRIMARY_TOUCH_BUTTON_ID ) )
      {
        state = TouchPoint::Interrupted;
      }

      TouchPoint point( touchEvent->multi.device, state, touchEvent->x, touchEvent->y );
      handler->SendEvent( point, touchEvent->timestamp );
    }

    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when a touch up is received.
   */
  static Eina_Bool EcoreEventMouseButtonUp( void* data, int type, void* event )
  {
    Ecore_Event_Mouse_Button *touchEvent( (Ecore_Event_Mouse_Button*)event );
    EventHandler* handler( (EventHandler*)data );

    if ( touchEvent->window == (unsigned int)ecore_wl_window_id_get(handler->mImpl->mWindow) )
    {
      TouchPoint point( touchEvent->multi.device, TouchPoint::Up, touchEvent->x, touchEvent->y );
      handler->SendEvent( point, touchEvent->timestamp );
    }

    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when a touch up is received.
   */
  static Eina_Bool EcoreEventMouseWheel( void* data, int type, void* event )
  {
    Ecore_Event_Mouse_Wheel *mouseWheelEvent( (Ecore_Event_Mouse_Wheel*)event );

    DALI_LOG_INFO( gImfLogging, Debug::General, "EVENT Ecore_Event_Mouse_Wheel: direction: %d, modifiers: %d, x: %d, y: %d, z: %d\n", mouseWheelEvent->direction, mouseWheelEvent->modifiers, mouseWheelEvent->x, mouseWheelEvent->y, mouseWheelEvent->z);

    EventHandler* handler( (EventHandler*)data );
    if ( mouseWheelEvent->window == (unsigned int)ecore_wl_window_id_get(handler->mImpl->mWindow) )
    {
      WheelEvent wheelEvent( WheelEvent::MOUSE_WHEEL, mouseWheelEvent->direction, mouseWheelEvent->modifiers, Vector2(mouseWheelEvent->x, mouseWheelEvent->y), mouseWheelEvent->z, mouseWheelEvent->timestamp );
      handler->SendWheelEvent( wheelEvent );
    }
    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when a touch motion is received.
   */
  static Eina_Bool EcoreEventMouseButtonMove( void* data, int type, void* event )
  {
    Ecore_Event_Mouse_Move *touchEvent( (Ecore_Event_Mouse_Move*)event );
    EventHandler* handler( (EventHandler*)data );

    if ( touchEvent->window == (unsigned int)ecore_wl_window_id_get(handler->mImpl->mWindow) )
    {
      TouchPoint point( touchEvent->multi.device, TouchPoint::Motion, touchEvent->x, touchEvent->y );
      handler->SendEvent( point, touchEvent->timestamp );
    }

    return ECORE_CALLBACK_PASS_ON;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Key Callbacks
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /**
   * Called when a key down is received.
   */
  static Eina_Bool EcoreEventKeyDown( void* data, int type, void* event )
  {
    DALI_LOG_INFO( gImfLogging, Debug::General, "EVENT >>EcoreEventKeyDown \n" );

    EventHandler* handler( (EventHandler*)data );
    Ecore_Event_Key *keyEvent( (Ecore_Event_Key*)event );
    bool eventHandled( false );

    // If a device key then skip ecore_imf_context_filter_event.
    if ( ! KeyLookup::IsDeviceButton( keyEvent->keyname ) )
    {
      Ecore_IMF_Context* imfContext = NULL;
      Dali::ImfManager imfManager( ImfManager::Get() );
      if ( imfManager )
      {
        imfContext = ImfManager::GetImplementation( imfManager ).GetContext();
      }

      if ( imfContext )
      {
        // We're consuming key down event so we have to pass to IMF so that it can parse it as well.
        Ecore_IMF_Event_Key_Down ecoreKeyDownEvent;
        ecoreKeyDownEvent.keyname   = keyEvent->keyname;
        ecoreKeyDownEvent.key       = keyEvent->key;
        ecoreKeyDownEvent.string    = keyEvent->string;
        ecoreKeyDownEvent.compose   = keyEvent->compose;
        ecoreKeyDownEvent.timestamp = keyEvent->timestamp;
        ecoreKeyDownEvent.modifiers = EcoreInputModifierToEcoreIMFModifier ( keyEvent->modifiers );
        ecoreKeyDownEvent.locks     = (Ecore_IMF_Keyboard_Locks) ECORE_IMF_KEYBOARD_LOCK_NONE;

        eventHandled = ecore_imf_context_filter_event( imfContext,
                                                       ECORE_IMF_EVENT_KEY_DOWN,
                                                       (Ecore_IMF_Event *) &ecoreKeyDownEvent );

        // If the event has not been handled by IMF then check if we should reset our IMF context
        if( !eventHandled )
        {
          if ( !strcmp( keyEvent->keyname, "Escape"   ) ||
               !strcmp( keyEvent->keyname, "Return"   ) ||
               !strcmp( keyEvent->keyname, "KP_Enter" ) )
          {
            ecore_imf_context_reset( imfContext );
          }
        }
      }
    }

    // If the event wasn't handled then we should send a key event.
    if ( !eventHandled )
    {
      if ( keyEvent->window == (unsigned int)ecore_wl_window_id_get(handler->mImpl->mWindow) )
      {
        std::string keyName( keyEvent->keyname );
        std::string keyString( "" );
        int keyCode = 0/*ecore_x_keysym_keycode_get(keyEvent->keyname)*/;
        int modifier( keyEvent->modifiers );
        unsigned long time = keyEvent->timestamp;

        if (!strncmp(keyEvent->keyname, "Keycode-", 8))
          keyCode = atoi(keyEvent->keyname + 8);

        // Ensure key event string is not NULL as keys like SHIFT have a null string.
        if ( keyEvent->string )
        {
          keyString = keyEvent->string;
        }

        KeyEvent keyEvent(keyName, keyString, keyCode, modifier, time, KeyEvent::Down);
        handler->SendEvent( keyEvent );
      }
    }

    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when a key up is received.
   */
  static Eina_Bool EcoreEventKeyUp( void* data, int type, void* event )
  {
    DALI_LOG_INFO( gImfLogging, Debug::General, "EVENT >>EcoreEventKeyUp \n" );

    EventHandler* handler( (EventHandler*)data );
    Ecore_Event_Key *keyEvent( (Ecore_Event_Key*)event );
    bool eventHandled( false );

    // Menu, home, back button must skip ecore_imf_context_filter_event.
    static const char* menuKeyName = KeyLookup::GetKeyName( DALI_KEY_MENU );
    static const char* homeKeyName = KeyLookup::GetKeyName( DALI_KEY_HOME );
    static const char* backKeyName = KeyLookup::GetKeyName( DALI_KEY_BACK );
    if ( ( menuKeyName && strcmp( keyEvent->keyname, menuKeyName ) != 0 ) &&
         ( homeKeyName && strcmp( keyEvent->keyname, homeKeyName ) != 0 ) &&
         ( backKeyName && strcmp( keyEvent->keyname, backKeyName ) != 0 ) )
    {
      Ecore_IMF_Context* imfContext = NULL;
      Dali::ImfManager imfManager( ImfManager::Get() );
      if ( imfManager )
      {
        imfContext = ImfManager::GetImplementation( imfManager ).GetContext();
      }

      if ( imfContext )
      {
        // We're consuming key up event so we have to pass to IMF so that it can parse it as well.
        Ecore_IMF_Event_Key_Up ecoreKeyUpEvent;
        ecoreKeyUpEvent.keyname   = keyEvent->keyname;
        ecoreKeyUpEvent.key       = keyEvent->key;
        ecoreKeyUpEvent.string    = keyEvent->string;
        ecoreKeyUpEvent.compose   = keyEvent->compose;
        ecoreKeyUpEvent.timestamp = keyEvent->timestamp;
        ecoreKeyUpEvent.modifiers = EcoreInputModifierToEcoreIMFModifier ( keyEvent->modifiers );
        ecoreKeyUpEvent.locks     = (Ecore_IMF_Keyboard_Locks) ECORE_IMF_KEYBOARD_LOCK_NONE;

        eventHandled = ecore_imf_context_filter_event( imfContext,
                                                       ECORE_IMF_EVENT_KEY_UP,
                                                       (Ecore_IMF_Event *) &ecoreKeyUpEvent );
      }
    }

    // If the event wasn't handled then we should send a key event.
    if ( !eventHandled )
    {
      if ( keyEvent->window == (unsigned int)ecore_wl_window_id_get(handler->mImpl->mWindow) )
      {
        std::string keyName( keyEvent->keyname );
        std::string keyString( "" );
        int keyCode = 0/*ecore_x_keysym_keycode_get(keyEvent->keyname)*/;
        int modifier( keyEvent->modifiers );
        unsigned long time( keyEvent->timestamp );

        if (!strncmp(keyEvent->keyname, "Keycode-", 8))
          keyCode = atoi(keyEvent->keyname + 8);

        // Ensure key event string is not NULL as keys like SHIFT have a null string.
        if ( keyEvent->string )
        {
          keyString = keyEvent->string;
        }

        KeyEvent keyEvent(keyName, keyString, keyCode, modifier, time, KeyEvent::Up);
        handler->SendEvent( keyEvent );
      }
    }

    return ECORE_CALLBACK_PASS_ON;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Window Callbacks
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /**
   * Called when the window gains focus.
   */
  static Eina_Bool EcoreEventWindowFocusIn( void* data, int type, void* event )
  {
    Ecore_Wl_Event_Focus_In* focusInEvent( (Ecore_Wl_Event_Focus_In*)event );
    EventHandler* handler( (EventHandler*)data );

    DALI_LOG_INFO( gImfLogging, Debug::General, "EVENT >>EcoreEventWindowFocusIn \n" );

    // If the window gains focus and we hid the keyboard then show it again.
    if ( focusInEvent->win == (unsigned int)ecore_wl_window_id_get(handler->mImpl->mWindow) )
    {
      DALI_LOG_INFO( gImfLogging, Debug::General, "EVENT EcoreEventWindowFocusIn - >>WindowFocusGained \n" );

      if ( ImfManager::IsAvailable() /* Only get the ImfManager if it's available as we do not want to create it */ )
      {
        Dali::ImfManager imfManager( ImfManager::Get() );
        if ( imfManager )
        {
          ImfManager& imfManagerImpl( ImfManager::GetImplementation( imfManager ) );
          if( imfManagerImpl.RestoreAfterFocusLost() )
          {
            imfManagerImpl.Activate();
          }
        }
      }
      // No need to connect callbacks as KeyboardStatusChanged will be called.
    }

    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when the window loses focus.
   */
  static Eina_Bool EcoreEventWindowFocusOut( void* data, int type, void* event )
  {
    Ecore_Wl_Event_Focus_Out* focusOutEvent( (Ecore_Wl_Event_Focus_Out*)event );
    EventHandler* handler( (EventHandler*)data );

    DALI_LOG_INFO( gImfLogging, Debug::General, "EVENT >>EcoreEventWindowFocusOut \n" );

    // If the window loses focus then hide the keyboard.
    if ( focusOutEvent->win == (unsigned int)ecore_wl_window_id_get(handler->mImpl->mWindow) )
    {
      if ( ImfManager::IsAvailable() /* Only get the ImfManager if it's available as we do not want to create it */ )
      {
        Dali::ImfManager imfManager( ImfManager::Get() );
        if ( imfManager )
        {
          ImfManager& imfManagerImpl( ImfManager::GetImplementation( imfManager ) );
          if( imfManagerImpl.RestoreAfterFocusLost() )
          {
            imfManagerImpl.Deactivate();
          }
        }
      }

      // Clipboard don't support that whether clipboard is shown or not. Hide clipboard.
      Dali::Clipboard clipboard = Clipboard::Get();
      clipboard.HideClipboard();
    }

    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when the window is damaged.
   */
  static Eina_Bool EcoreEventWindowDamaged(void *data, int type, void *event)
  {
    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when the window properties are changed.
   * We are only interested in the font change.
   */


  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Drag & Drop Callbacks
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /**
   * Called when a dragged item enters our window's bounds.
   * This is when items are dragged INTO our window.
   */
  static Eina_Bool EcoreEventDndEnter( void* data, int type, void* event )
  {
    DALI_LOG_INFO( gDragAndDropLogFilter, Debug::Concise, "EcoreEventDndEnter\n" );

    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when a dragged item is moved within our window.
   * This is when items are dragged INTO our window.
   */
  static Eina_Bool EcoreEventDndPosition( void* data, int type, void* event )
  {
    DALI_LOG_INFO(gDragAndDropLogFilter, Debug::Concise, "EcoreEventDndPosition\n" );

    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when a dragged item leaves our window's bounds.
   * This is when items are dragged INTO our window.
   */
  static Eina_Bool EcoreEventDndLeave( void* data, int type, void* event )
  {
    DALI_LOG_INFO(gDragAndDropLogFilter, Debug::Concise, "EcoreEventDndLeave\n" );

    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when the dragged item is dropped within our window's bounds.
   * This is when items are dragged INTO our window.
   */
  static Eina_Bool EcoreEventDndDrop( void* data, int type, void* event )
  {
    DALI_LOG_INFO(gDragAndDropLogFilter, Debug::Concise, "EcoreEventDndDrop\n" );

    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when a dragged item is moved from our window and the target window has done processing it.
   * This is when items are dragged FROM our window.
   */
  static Eina_Bool EcoreEventDndFinished( void* data, int type, void* event )
  {
    DALI_LOG_INFO(gDragAndDropLogFilter, Debug::Concise, "EcoreEventDndFinished\n" );
    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when a dragged item is moved from our window and the target window has sent us a status.
   * This is when items are dragged FROM our window.
   */
  static Eina_Bool EcoreEventDndStatus( void* data, int type, void* event )
  {
    DALI_LOG_INFO(gDragAndDropLogFilter, Debug::Concise, "EcoreEventDndStatus\n" );
    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when the client messages (i.e. the accessibility events) are received.
   */
  static Eina_Bool EcoreEventClientMessage( void* data, int type, void* event )
  {
    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when the source window notifies us the content in clipboard is selected.
   */
  static Eina_Bool EcoreEventSelectionClear( void* data, int type, void* event )
  {
    DALI_LOG_INFO(gSelectionEventLogFilter, Debug::Concise, "EcoreEventSelectionClear\n" );
    return ECORE_CALLBACK_PASS_ON;
  }

  /**
   * Called when the source window sends us about the selected content.
   * For example, when dragged items are dragged INTO our window or when items are selected in the clipboard.
   */
  static Eina_Bool EcoreEventSelectionNotify( void* data, int type, void* event )
  {
    DALI_LOG_INFO(gSelectionEventLogFilter, Debug::Concise, "EcoreEventSelectionNotify\n" );
    return ECORE_CALLBACK_PASS_ON;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Font Callbacks
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /**
   * Called when a font name is changed.
   */
  static void VconfNotifyFontNameChanged( keynode_t* node, void* data )
  {
    EventHandler* handler = static_cast<EventHandler*>( data );
    handler->SendEvent( StyleChange::DEFAULT_FONT_CHANGE );
  }

  /**
   * Called when a font size is changed.
   */
  static void VconfNotifyFontSizeChanged( keynode_t* node, void* data )
  {
    EventHandler* handler = static_cast<EventHandler*>( data );
    handler->SendEvent( StyleChange::DEFAULT_FONT_SIZE_CHANGE );
  }

  // Data
  EventHandler* mHandler;
  std::vector<Ecore_Event_Handler*> mEcoreEventHandler;
  Ecore_Wl_Window* mWindow;
};

EventHandler::EventHandler( RenderSurface* surface, CoreEventInterface& coreEventInterface, GestureManager& gestureManager, DamageObserver& damageObserver, DragAndDropDetectorPtr dndDetector )
: mCoreEventInterface(coreEventInterface),
  mGestureManager( gestureManager ),
  mStyleMonitor( StyleMonitor::Get() ),
  mDamageObserver( damageObserver ),
  mRotationObserver( NULL ),
  mDragAndDropDetector( dndDetector ),
  mAccessibilityAdaptor( AccessibilityAdaptor::Get() ),
  mClipboardEventNotifier( ClipboardEventNotifier::Get() ),
  mClipboard(Clipboard::Get()),
  mImpl( NULL )
{
  Ecore_Wl_Window* window = 0;

  // this code only works with the Ecore RenderSurface so need to downcast
  ECore::WindowRenderSurface* ecoreSurface = dynamic_cast< ECore::WindowRenderSurface* >( surface );
  if( ecoreSurface )
  {
    window = ecoreSurface->GetWlWindow();
  }

  mImpl = new Impl(this, window);
}

EventHandler::~EventHandler()
{
  if(mImpl)
  {
    delete mImpl;
  }

  mGestureManager.Stop();
}

void EventHandler::SendEvent(TouchPoint& point, unsigned long timeStamp)
{
  if(timeStamp < 1)
  {
    timeStamp = GetCurrentMilliSeconds();
  }

  Integration::TouchEvent touchEvent;
  Integration::HoverEvent hoverEvent;
  Integration::TouchEventCombiner::EventDispatchType type = mCombiner.GetNextTouchEvent(point, timeStamp, touchEvent, hoverEvent);
  if(type != Integration::TouchEventCombiner::DispatchNone )
  {
    DALI_LOG_INFO(gTouchEventLogFilter, Debug::General, "%d: Device %d: Button state %d (%.2f, %.2f)\n", timeStamp, point.deviceId, point.state, point.local.x, point.local.y);

    // First the touch and/or hover event & related gesture events are queued
    if(type == Integration::TouchEventCombiner::DispatchTouch || type == Integration::TouchEventCombiner::DispatchBoth)
    {
      mCoreEventInterface.QueueCoreEvent( touchEvent );
      mGestureManager.SendEvent(touchEvent);
    }

    if(type == Integration::TouchEventCombiner::DispatchHover || type == Integration::TouchEventCombiner::DispatchBoth)
    {
      mCoreEventInterface.QueueCoreEvent( hoverEvent );
    }

    // Next the events are processed with a single call into Core
    mCoreEventInterface.ProcessCoreEvents();
  }
}

void EventHandler::SendEvent(KeyEvent& keyEvent)
{
  Dali::PhysicalKeyboard physicalKeyboard = PhysicalKeyboard::Get();
  if ( physicalKeyboard )
  {
    if ( ! KeyLookup::IsDeviceButton( keyEvent.keyPressedName.c_str() ) )
    {
      GetImplementation( physicalKeyboard ).KeyReceived( keyEvent.time > 1 );
    }
  }

  // Create KeyEvent and send to Core.
  Integration::KeyEvent event(keyEvent.keyPressedName, keyEvent.keyPressed, keyEvent.keyCode,
  keyEvent.keyModifier, keyEvent.time, static_cast<Integration::KeyEvent::State>(keyEvent.state));
  mCoreEventInterface.QueueCoreEvent( event );
  mCoreEventInterface.ProcessCoreEvents();
}

void EventHandler::SendWheelEvent( WheelEvent& wheelEvent )
{
  // Create WheelEvent and send to Core.
  Integration::WheelEvent event( static_cast< Integration::WheelEvent::Type >(wheelEvent.type), wheelEvent.direction, wheelEvent.modifiers, wheelEvent.point, wheelEvent.z, wheelEvent.timeStamp );
  mCoreEventInterface.QueueCoreEvent( event );
  mCoreEventInterface.ProcessCoreEvents();
}

void EventHandler::SendEvent( StyleChange::Type styleChange )
{
  DALI_ASSERT_DEBUG( mStyleMonitor && "StyleMonitor Not Available" );
  GetImplementation( mStyleMonitor ).StyleChanged(styleChange);
}

void EventHandler::SendEvent( const DamageArea& area )
{
  mDamageObserver.OnDamaged( area );
}

void EventHandler::SendRotationPrepareEvent( const RotationEvent& event )
{
  if( mRotationObserver != NULL )
  {
    mRotationObserver->OnRotationPrepare( event );
  }
}

void EventHandler::SendRotationRequestEvent( )
{
  if( mRotationObserver != NULL )
  {
    mRotationObserver->OnRotationRequest( );
  }
}

void EventHandler::FeedTouchPoint( TouchPoint& point, int timeStamp)
{
  SendEvent(point, timeStamp);
}

void EventHandler::FeedWheelEvent( WheelEvent& wheelEvent )
{
  SendWheelEvent( wheelEvent );
}

void EventHandler::FeedKeyEvent( KeyEvent& event )
{
  SendEvent( event );
}

void EventHandler::FeedEvent( Integration::Event& event )
{
  mCoreEventInterface.QueueCoreEvent( event );
  mCoreEventInterface.ProcessCoreEvents();
}

void EventHandler::Reset()
{
  mCombiner.Reset();

  // Any touch listeners should be told of the interruption.
  Integration::TouchEvent event;
  TouchPoint point(0, TouchPoint::Interrupted, 0, 0);
  event.AddPoint( point );

  // First the touch event & related gesture events are queued
  mCoreEventInterface.QueueCoreEvent( event );
  mGestureManager.SendEvent( event );

  // Next the events are processed with a single call into Core
  mCoreEventInterface.ProcessCoreEvents();
}

void EventHandler::Pause()
{
  mPaused = true;
  Reset();
}

void EventHandler::Resume()
{
  mPaused = false;
  Reset();
}

void EventHandler::SetDragAndDropDetector( DragAndDropDetectorPtr detector )
{
  mDragAndDropDetector = detector;
}

void EventHandler::SetRotationObserver( RotationObserver* observer )
{
  mRotationObserver = observer;
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
