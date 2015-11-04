#ifndef __DALI_KEY_H__
#define __DALI_KEY_H__

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

// EXTERNAL INCLUDES

#include <dali/public-api/events/key-event.h>

namespace Dali
{
/**
 * @addtogroup dali_adaptor_framework
 * @{
 */

/**
 * @brief Mapping of keyboard and mouse button event keycodes to platform specific codes.
 */

enum KEY
{
  DALI_KEY_INVALID          = -1,
  DALI_KEY_ESCAPE           = 9,
  DALI_KEY_BACKSPACE        = 22,
  DALI_KEY_SHIFT_LEFT       = 50,
  DALI_KEY_SHIFT_RIGHT      = 62,
  DALI_KEY_CURSOR_UP        = 111,
  DALI_KEY_CURSOR_LEFT      = 113,
  DALI_KEY_CURSOR_RIGHT     = 114,
  DALI_KEY_CURSOR_DOWN      = 116,
  DALI_KEY_BACK             = 166,
  DALI_KEY_CAMERA           = 167,
  DALI_KEY_CONFIG           = 168,
  DALI_KEY_POWER            = 169,
  DALI_KEY_PAUSE            = 170,
  DALI_KEY_CANCEL           = 171,
  DALI_KEY_PLAY_CD          = 172,
  DALI_KEY_STOP_CD          = 173,
  DALI_KEY_PAUSE_CD         = 174,
  DALI_KEY_NEXT_SONG        = 175,
  DALI_KEY_PREVIOUS_SONG    = 176,
  DALI_KEY_REWIND           = 177,
  DALI_KEY_FASTFORWARD      = 178,
  DALI_KEY_MEDIA            = 179,
  DALI_KEY_PLAY_PAUSE       = 180,
  DALI_KEY_MUTE             = 181,
  DALI_KEY_MENU             = 182,
  DALI_KEY_HOME             = 183,
  DALI_KEY_HOMEPAGE         = 187,
  DALI_KEY_WEBPAGE          = 188,
  DALI_KEY_MAIL             = 189,
  DALI_KEY_SCREENSAVER      = 190,
  DALI_KEY_BRIGHTNESS_UP    = 191,
  DALI_KEY_BRIGHTNESS_DOWN  = 192,
  DALI_KEY_SOFT_KBD         = 193,
  DALI_KEY_QUICK_PANEL      = 194,
  DALI_KEY_TASK_SWITCH      = 195,
  DALI_KEY_APPS             = 196,
  DALI_KEY_SEARCH           = 197,
  DALI_KEY_VOICE            = 198,
  DALI_KEY_LANGUAGE         = 199,
  DALI_KEY_VOLUME_UP        = 200,
  DALI_KEY_VOLUME_DOWN      = 201
};

/**
 * @brief Check if a key event is for a specific DALI KEY.
 *
 * @param keyEvent reference to a keyEvent structure
 * @param daliKey dali key enum
 * @return true if the key is matched, false if not
 */
DALI_IMPORT_API bool IsKey( const KeyEvent& keyEvent, KEY daliKey);

/**
 * @}
 */
} // namespace Dali

#endif // __DALI_KEY_H__
