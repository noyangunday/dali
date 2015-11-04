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
#include <window-extensions.h>

// EXTERNAL INCLUDES
#include <dali/public-api/object/any.h>
#include <dali/public-api/common/vector-wrapper.h>
#include <string>
#include <sstream>
#include <Ecore.h>
#include <Ecore_X.h>

// INTERNAL INCLUDES
#include <window.h>

namespace
{
typedef std::vector< std::string > HintContainer;

const char* HINT_EFFECT_NAME = "wm.comp.win.effect.enable";
const char* HINT_ENABLE_POSTFIX = ":1";
const char* HINT_DISABLE_POSTFIX = ":0";

void Tokenize(const std::string& str, HintContainer& hints, const std::string& delimiters = ",")
{
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  std::string::size_type pos = str.find_first_of(delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos)
  {
    hints.push_back(str.substr(lastPos, pos - lastPos));
    lastPos = str.find_first_not_of(delimiters, pos);
    pos = str.find_first_of(delimiters, lastPos);
  }
}

void GetAppliedHints( Dali::Window window, HintContainer& hints )
{
  Dali::Any nativeWindow = window.GetNativeHandle();
  if ( !nativeWindow.Empty() )
  {
    Ecore_X_Window ecoreWindow;
    nativeWindow.Get( ecoreWindow );

    unsigned char* data = NULL;
    int n = 0;
    int res = ecore_x_window_prop_property_get( ecoreWindow, ECORE_X_ATOM_E_WINDOW_AUX_HINT_SUPPORTED_LIST,
                                                ECORE_X_ATOM_STRING, 0, &data, &n );

    if ((res == 8) && (n > 0))
    {
      std::stringstream ss;
      ss << data;
      Tokenize(ss.str(), hints);
    }

    free(data);
  }
}

}

namespace Dali
{

namespace WindowExtensions
{

void EnableEffect( Window window, bool enable )
{
  Any nativeWindow = window.GetNativeHandle();

  DALI_ASSERT_ALWAYS( !nativeWindow.Empty() && "Empty window!!!" );

  HintContainer hints;
  GetAppliedHints( window, hints );

  std::stringstream ss;
  ss << hints.size() << ":" << HINT_EFFECT_NAME << (enable ? HINT_ENABLE_POSTFIX : HINT_DISABLE_POSTFIX);

  // Applied the window effect to the current window.
  Ecore_X_Window ecoreWindow;
  nativeWindow.Get(ecoreWindow);
  ecore_x_window_prop_property_set( ecoreWindow, ECORE_X_ATOM_E_WINDOW_AUX_HINT,
                                    ECORE_X_ATOM_STRING, 8,
                                    (void*)ss.str().c_str(), ss.str().size() + 1 );
}

bool IsEffectEnabled( Window window )
{
  Any nativeWindow = window.GetNativeHandle();

  DALI_ASSERT_ALWAYS( !nativeWindow.Empty() && "Empty window!!!" );

  HintContainer hints;
  GetAppliedHints( window, hints );

  HintContainer::iterator iter = std::find( hints.begin(), hints.end(), HINT_EFFECT_NAME );

  return iter != hints.end();
}

} // namespace WindowExtensions

} // namespace Dali


