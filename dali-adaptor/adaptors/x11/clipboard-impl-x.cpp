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
#include "clipboard-impl.h"

// EXTERNAL INCLUDES
#include <Ecore_X.h>
#include <dali/public-api/object/any.h>
#include <dali/public-api/object/type-registry.h>
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <adaptor-impl.h>
#include <ecore-x-window-interface.h>
#include <singleton-service-impl.h>

namespace //unnamed namespace
{
const char* const CBHM_WINDOW = "CBHM_XWIN";
const char* const CBHM_MSG = "CBHM_MSG";
const char* const CBHM_ITEM = "CBHM_ITEM";
const char* const CBHM_cCOUNT = "CBHM_cCOUNT";
const char* const CBHM_ERROR = "CBHM_ERROR";
const char* const SET_ITEM = "SET_ITEM";
const char* const SHOW = "show0";
const char* const HIDE = "cbhm_hide";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Clipboard
///////////////////////////////////////////////////////////////////////////////////////////////////

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
  BaseHandle handle( Clipboard::Get() );

  if ( !handle && Adaptor::IsAvailable() )
  {
    Dali::SingletonService service( SingletonService::Get() );
    if ( service )
    {
      Adaptor& adaptorImpl( Adaptor::GetImplementation( Adaptor::Get() ) );
      Any nativewindow = adaptorImpl.GetNativeWindowHandle();

      // The Ecore_X_Window needs to use the Clipboard.
      // Only when the render surface is window, we can get the Ecore_X_Window.
      Ecore_X_Window ecoreXwin( AnyCast<Ecore_X_Window>(nativewindow) );
      if (ecoreXwin)
      {
        // If we fail to get Ecore_X_Window, we can't use the Clipboard correctly.
        // Thus you have to call "ecore_imf_context_client_window_set" somewhere.
        // In EvasPlugIn, this function is called in EvasPlugin::ConnectEcoreEvent().
        Dali::Clipboard clipboard = Dali::Clipboard( new Clipboard( ecoreXwin ) );
        service.Register( typeid( clipboard ), clipboard );
        handle = clipboard;
      }
      else
      {
        DALI_LOG_ERROR("Failed to get native window handle");
      }
    }
  }

  return handle;
}
TypeRegistration CLIPBOARD_TYPE( typeid(Dali::Clipboard), typeid(Dali::BaseHandle), Create, true /* Create Instance At Startup */ );

} // unnamed namespace

Clipboard::Clipboard( Ecore_X_Window ecoreXwin)
{
  mApplicationWindow = ecoreXwin;
}

Clipboard::~Clipboard()
{
}

Dali::Clipboard Clipboard::Get()
{
  Dali::Clipboard clipboard;

  Dali::SingletonService service( SingletonService::Get() );
  if ( service )
  {
    // Check whether the singleton is already created
    Dali::BaseHandle handle = service.GetSingleton( typeid( Dali::Clipboard ) );
    if(handle)
    {
      // If so, downcast the handle
      clipboard = Dali::Clipboard( dynamic_cast< Clipboard* >( handle.GetObjectPtr() ) );
    }
  }

  return clipboard;
}
bool Clipboard::SetItem(const std::string &itemData )
{
  Ecore_X_Window cbhmWin = ECore::WindowInterface::GetWindow();
  Ecore_X_Atom atomCbhmItem = ecore_x_atom_get( CBHM_ITEM );
  Ecore_X_Atom dataType = ECORE_X_ATOM_STRING;

  // Set item (property) to send
  ecore_x_window_prop_property_set( cbhmWin, atomCbhmItem, dataType, 8, const_cast<char*>( itemData.c_str() ), itemData.length() + 1 );
  ecore_x_sync();

  // Trigger sending of item (property)
  Ecore_X_Atom atomCbhmMsg = ecore_x_atom_get( CBHM_MSG );
  ECore::WindowInterface::SendXEvent(ecore_x_display_get(), cbhmWin, False, NoEventMask, atomCbhmMsg, 8, SET_ITEM );
  return true;
}

/*
 * Get string at given index of clipboard
 */
std::string Clipboard::GetItem( unsigned int index )  // change string to a Dali::Text object.
{
  if ( index >= NumberOfItems() )
  {
    return "";
  }

  std::string emptyString( "" );
  char sendBuf[20];

  snprintf( sendBuf, 20,  "%s%d", CBHM_ITEM, index );
  Ecore_X_Atom xAtomCbhmItem = ecore_x_atom_get( sendBuf );
  Ecore_X_Atom xAtomItemType = 0;

  std::string clipboardString( ECore::WindowInterface::GetWindowProperty(xAtomCbhmItem, &xAtomItemType, index ) );
  if ( !clipboardString.empty() )
  {
    Ecore_X_Atom xAtomCbhmError = ecore_x_atom_get( CBHM_ERROR );
    if ( xAtomItemType != xAtomCbhmError )
    {
      return clipboardString;
    }
   }
   return emptyString;
}

/*
 * Get number of items in clipboard
 */
unsigned int Clipboard::NumberOfItems()
{
  Ecore_X_Atom xAtomCbhmCountGet = ecore_x_atom_get( CBHM_cCOUNT );

  std::string ret( ECore::WindowInterface::GetWindowProperty( xAtomCbhmCountGet, NULL, 0 ) );
  int count = 0;

  if ( !ret.empty() )
  {
    count = atoi( ret.c_str() );
  }

  return count;
}

/**
 * Show clipboard window
 * Function to send message to show the Clipboard (cbhm) as no direct API available
 * Reference elementary/src/modules/ctxpopup_copypasteUI/cbhm_helper.c
 */
void Clipboard::ShowClipboard()
{
  // Claim the ownership of the SECONDARY selection.
  ecore_x_selection_secondary_set(mApplicationWindow, "", 1);
  Ecore_X_Window cbhmWin = ECore::WindowInterface::GetWindow();

  // Launch the clipboard window
  Ecore_X_Atom atomCbhmMsg = ecore_x_atom_get( CBHM_MSG );
  ECore::WindowInterface::SendXEvent( ecore_x_display_get(), cbhmWin, False, NoEventMask, atomCbhmMsg, 8, SHOW );
}

void Clipboard::HideClipboard()
{
  Ecore_X_Window cbhmWin = ECore::WindowInterface::GetWindow();
  // Launch the clipboard window
  Ecore_X_Atom atomCbhmMsg = ecore_x_atom_get( CBHM_MSG );
  ECore::WindowInterface::SendXEvent( ecore_x_display_get(), cbhmWin, False, NoEventMask, atomCbhmMsg, 8, HIDE );

  // release the ownership of SECONDARY selection
  ecore_x_selection_secondary_clear();
}


} // namespace Adaptor

} // namespace Internal

} // namespace Dali
