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
#include "key-impl.h"

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace KeyLookup
{

// matches a DALI_KEY enum, to a key name
KeyLookup KeyLookupTable[]=
{
  // more than one key name can be assigned to a single dali-key code
  // e.g. "Menu" and "XF86Menu" are both assigned to  DALI_KEY_MENU

  { "Escape",                DALI_KEY_ESCAPE,          false },
  { "Menu",                  DALI_KEY_MENU,            false },

  // Now literal strings are used as key names instead of defined symbols in utilX,
  // since these definition in utilX.h is deprecated
  { "XF86Camera",            DALI_KEY_CAMERA,          false },
  { "XF86Camera_Full",       DALI_KEY_CONFIG,          false },
  { "XF86PowerOff",          DALI_KEY_POWER,           true  },
  { "XF86Standby",           DALI_KEY_PAUSE,           false },
  { "Cancel",                DALI_KEY_CANCEL,          false },
  { "XF86AudioPlay",         DALI_KEY_PLAY_CD,         false },
  { "XF86AudioStop",         DALI_KEY_STOP_CD,         false },
  { "XF86AudioPause",        DALI_KEY_PAUSE_CD,        false },
  { "XF86AudioNext",         DALI_KEY_NEXT_SONG,       false },
  { "XF86AudioPrev",         DALI_KEY_PREVIOUS_SONG,   false },
  { "XF86AudioRewind",       DALI_KEY_REWIND,          false },
  { "XF86AudioForward",      DALI_KEY_FASTFORWARD,     false },
  { "XF86AudioMedia",        DALI_KEY_MEDIA,           false },
  { "XF86AudioPlayPause",    DALI_KEY_PLAY_PAUSE,      false },
  { "XF86AudioMute",         DALI_KEY_MUTE,            false },
  { "XF86Menu",              DALI_KEY_MENU,            true  },
  { "XF86Home",              DALI_KEY_HOME,            true  },
  { "XF86Back",              DALI_KEY_BACK,            true  },
  { "XF86HomePage",          DALI_KEY_HOMEPAGE,        false },
  { "XF86WWW",               DALI_KEY_WEBPAGE,         false },
  { "XF86Mail",              DALI_KEY_MAIL,            false },
  { "XF86ScreenSaver",       DALI_KEY_SCREENSAVER,     false },
  { "XF86MonBrightnessUp",   DALI_KEY_BRIGHTNESS_UP,   false },
  { "XF86MonBrightnessDown", DALI_KEY_BRIGHTNESS_DOWN, false },
  { "XF86SoftKBD",           DALI_KEY_SOFT_KBD,        false },
  { "XF86QuickPanel",        DALI_KEY_QUICK_PANEL,     false },
  { "XF86TaskPane",          DALI_KEY_TASK_SWITCH,     false },
  { "XF86Apps",              DALI_KEY_APPS,            false },
  { "XF86Search",            DALI_KEY_SEARCH,          false },
  { "XF86Voice",             DALI_KEY_VOICE,           false },
  { "Hangul",                DALI_KEY_LANGUAGE,        false },
  { "XF86AudioRaiseVolume",  DALI_KEY_VOLUME_UP,       true  },
  { "XF86AudioLowerVolume",  DALI_KEY_VOLUME_DOWN,     true  },

  { "BackSpace",             DALI_KEY_BACKSPACE,       false },
  { "Left",                  DALI_KEY_CURSOR_LEFT,     false },
  { "Right",                 DALI_KEY_CURSOR_RIGHT,    false },
  { "Shift_L",               DALI_KEY_SHIFT_LEFT,      false },
  { "Shift_R",               DALI_KEY_SHIFT_RIGHT,     false }
};

const std::size_t KEY_LOOKUP_COUNT = (sizeof( KeyLookupTable))/ (sizeof(KeyLookup));

} // namespace KeyLookup

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
