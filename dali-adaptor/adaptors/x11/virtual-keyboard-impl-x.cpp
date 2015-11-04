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
#include "virtual-keyboard-impl.h"

// EXTERNAL INCLUDES
#include <X11/Xlib.h>
#include <Ecore_X.h>
#include <algorithm>

#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <adaptor.h>
#include <locale-utils.h>
#include <imf-manager-impl.h>


namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace VirtualKeyboard
{

Dali::InputMethod::ActionButton gActionButtonFunction = Dali::InputMethod::ACTION_DEFAULT;

Ecore_IMF_Input_Panel_Return_Key_Type actionButtonMapping(Dali::InputMethod::ActionButton actionButton )
{
  switch( actionButton )
  {
    case InputMethod::ACTION_DEFAULT:     return ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_DEFAULT;
    case InputMethod::ACTION_DONE:        return ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_DONE;
    case InputMethod::ACTION_GO:          return ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_GO;
    case InputMethod::ACTION_JOIN:        return ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_JOIN;
    case InputMethod::ACTION_LOGIN:       return ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_LOGIN;
    case InputMethod::ACTION_NEXT:        return ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_NEXT;
    case InputMethod::ACTION_PREVIOUS:    return ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_DEFAULT;
    case InputMethod::ACTION_SEARCH:      return ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_SEARCH;
    case InputMethod::ACTION_SEND:        return ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_SEND;
    case InputMethod::ACTION_SIGNIN:      return ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_DEFAULT;
    case InputMethod::ACTION_UNSPECIFIED: return ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_DEFAULT;
    case InputMethod::ACTION_NONE:        return ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_DEFAULT;
    default:                              return ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_DEFAULT;
  }
}

void RotateTo(int angle)
{
  // Get focus window used by Keyboard and rotate it
  Display* display = XOpenDisplay(0);
  if (display)
  {
    ::Window focusWindow;
    int revert;
    // Get Focus window
    XGetInputFocus(display, &focusWindow, &revert);

    ecore_x_window_prop_property_set( focusWindow,
                                      ECORE_X_ATOM_E_ILLUME_ROTATE_WINDOW_ANGLE,
                                      ECORE_X_ATOM_CARDINAL, 32, &angle, 1 );
    XCloseDisplay(display);
  }
}

void SetReturnKeyType( const InputMethod::ActionButton type )
{
  Dali::ImfManager imfManager = ImfManager::Get(); // Create ImfManager instance (if required) when setting values
  Ecore_IMF_Context* imfContext = ImfManager::GetImplementation( imfManager ).GetContext();

  if( imfContext )
  {
    gActionButtonFunction = type;
    ecore_imf_context_input_panel_return_key_type_set( imfContext, actionButtonMapping( type ) );
  }
}

Dali::InputMethod::ActionButton GetReturnKeyType()
{
  return gActionButtonFunction;
}

} // namespace VirtualKeyboard

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
