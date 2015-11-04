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
#include <Ecore.h>
#include <Ecore_Wayland.h>
#include <dali/public-api/object/any.h>
#include <dali/public-api/object/type-registry.h>
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
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

  return handle;
}
TypeRegistration CLIPBOARD_TYPE( typeid(Dali::Clipboard), typeid(Dali::BaseHandle), Create, true /* Create Instance At Startup */ );

} // unnamed namespace

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
  return emptyString;
}

/*
 * Get number of items in clipboard
 */
unsigned int Clipboard::NumberOfItems()
{
  int count = -1;

  return count;
}

/**
 * Show clipboard window
 * Function to send message to show the Clipboard (cbhm) as no direct API available
 * Reference elementary/src/modules/ctxpopup_copypasteUI/cbhm_helper.c
 */
void Clipboard::ShowClipboard()
{
}

void Clipboard::HideClipboard()
{
}


} // namespace Adaptor

} // namespace Internal

} // namespace Dali
