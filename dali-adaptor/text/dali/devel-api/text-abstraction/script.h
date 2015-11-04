#ifndef __DALI_TOOLKIT_TEXT_ABSTRACTION_SCRIPT_H__
#define __DALI_TOOLKIT_TEXT_ABSTRACTION_SCRIPT_H__

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
#include <dali/public-api/common/dali-common.h>

// INTERNAL INCLUDES
#include <dali/devel-api/text-abstraction/text-abstraction-definitions.h>

namespace Dali
{

namespace TextAbstraction
{

/**
 * @brief Script is the writing system used by a language.
 * Typically one script can be used to write different languages although one language could be written in different scrips.
 */
enum Script
{
  COMMON,     ///< Valid for all scripts. i.e white space or '\n'.

  CYRILLIC,   ///< The Cyrillic script. Used by Russian, Bulgarian, Ukrainian, Macedonian, ...
  GREEK,      ///< The Greek script. Used by Greek.
  LATIN,      ///< The latin script. Used by many western languages and others around the world.

  ARABIC,     ///< The arabic script. Used by Arab and Urdu among others.
  HEBREW,     ///< The Hebrew script. Used by the Hebrew, Yiddish, Ladino, and Judeo-Arabic.

  ARMENIAN,   ///< The Armenian script. Used by Armenian.
  GEORGIAN,   ///< The Georgian script. Used by Georgian.

  CJK,        ///< The CJK script. Used by Chinese, Japanese, Korean and Vietnamese(old writing system).
  HANGUL,     ///< The Hangul jamo script. Used by Korean.
  HIRAGANA,   ///< The Hiragana script. Used by the Japanese.
  KATAKANA,   ///< The Katakana script. Used by the Japanese.
  BOPOMOFO,   ///< The Bopomofo script. Also called Zhuyin fuhao or Zhuyin. A phonetic notation used for the transcription of spoken Chinese.

  BENGALI,    ///< The Bengali script. Used by Bangla, Assamese, Bishnupriya Manipuri, Daphla, Garo, Hallam, Khasi, Mizo, Munda, Naga, Rian, and Santali.
  BURMESE,    ///< The Burmese script. Used by the Burmese (Myanmar) language.
  DEVANAGARI, ///< The devanagari script. Used by Hindi, Marathi, Sindhi, Nepali and Sanskrit.
  GUJARATI,   ///< The Gujarati script. Used by Gujarati.
  GURMUKHI,   ///< The Gurmukhi script. Used by Punjabi.
  KANNADA,    ///< The Kannada script. Used by Kannada and Tulu.
  MALAYALAM,  ///< The Malayalam script. Used by Malayalam.
  ORIYA,      ///< The Oriya script. Used by Oriya (Odia), Khondi, and Santali.
  SINHALA,    ///< The Sinhala script. Used by Sinhala and Pali.
  TAMIL,      ///< The Tamil script. Used by Tamil, Badaga, and Saurashtra.
  TELUGU,     ///< The Telugu script. Used by Telugu, Gondi, and Lambadi.

  LAO,        ///< The Lao script. Used by the Lao language.
  THAI,       ///< The Thai script. Used by the Thai language
  KHMER,      ///< The Khmer script. Used by the Khmer language.

  EMOJI,      ///< The Emoji which map to standardized Unicode characters.

  UNKNOWN     ///< The script is unknown.
};

const char* const ScriptName[] =
{
  "COMMON",     ///< Valid for all scripts. i.e white space or '\n'.

  "CYRILLIC",   ///< The Cyrillic script. Used by Russian, Bulgarian, Ukrainian, Macedonian, ...
  "GREEK",      ///< The Greek script. Used by Greek.
  "LATIN",      ///< The latin script. Used by many western languages and others around the world.

  "ARABIC",     ///< The arabic script. Used by Arab and Urdu among others.
  "HEBREW",     ///< The Hebrew script. Used by the Hebrew, Yiddish, Ladino, and Judeo-Arabic.

  "ARMENIAN",   ///< The Armenian script. Used by Armenian.
  "GEORGIAN",   ///< The Georgian script. Used by Georgian.

  "CJK",        ///< The CJK script. Used by Chinese, Japanese, Korean and Vietnamese(old writing system).
  "HANGUL",     ///< The Hangul jamo script. Used by Korean.
  "HIRAGANA",   ///< The Hiragana script. Used by the Japanese.
  "KATAKANA",   ///< The Katakana script. Used by the Japanese.
  "BOPOMOFO",   ///< The Bopomofo script. Also called Zhuyin fuhao or Zhuyin. A phonetic notation used for the transcription of spoken Chinese.

  "BENGALI",    ///< The Bengali script. Used by Bangla, Assamese, Bishnupriya Manipuri, Daphla, Garo, Hallam, Khasi, Mizo, Munda, Naga, Rian, and Santali.
  "BURMESE",    ///< The Burmese script. Used by the Burmese (Myanmar) language.
  "DEVANAGARI", ///< The devanagari script. Used by Hindi, Marathi, Sindhi, Nepali and Sanskrit.
  "GUJARATI",   ///< The Gujarati script. Used by Gujarati.
  "GURMUKHI",   ///< The Gurmukhi script. Used by Punjabi.
  "KANNADA",    ///< The Kannada script. Used by Kannada and Tulu.
  "MALAYALAM",  ///< The Malayalam script. Used by Malayalam.
  "ORIYA",      ///< The Oriya script. Used by Oriya (Odia), Khondi, and Santali.
  "SINHALA",    ///< The Sinhala script. Used by Sinhala and Pali.
  "TAMIL",      ///< The Tamil script. Used by Tamil, Badaga, and Saurashtra.
  "TELUGU",     ///< The Telugu script. Used by Telugu, Gondi, and Lambadi.

  "LAO",        ///< The Lao script. Used by the Lao language.
  "THAI",       ///< The Thai script. Used by the Thai language
  "KHMER",      ///< The Khmer script. Used by the Khmer language.

  "EMOJI",      ///< The Emoji which map to standardized Unicode characters.

  "UNKNOWN"     ///< The script is unknown.
};

/**
 * @brief Whether the script is a right to left script.
 *
 * @param[in] script The script.
 *
 * @return @e true if the script is right to left.
 */
DALI_IMPORT_API bool IsRightToLeftScript( Script script );

/**
 * @brief Retrieves a character's script.
 *
 * @param[in] character The character.
 *
 * @return The chraracter's script.
 */
DALI_IMPORT_API Script GetCharacterScript( Character character );

/**
 * @brief Whether the character is a white space.
 *
 * @param[in] character The character.
 *
 * @return @e true if the character is a white space.
 */
DALI_IMPORT_API bool IsWhiteSpace( Character character );

/**
 * @brief Whether the character is a new paragraph character.
 *
 * @param[in] character The character.
 *
 * @return @e true if the character is a new paragraph character.
 */
DALI_IMPORT_API bool IsNewParagraph( Character character );

/**
 * @brief Whether the character is a zero width non joiner.
 *
 * @param[in] character The character.
 *
 * @return @e true if the character is a zero width non joiner.
 */
DALI_IMPORT_API bool IsZeroWidthNonJoiner( Character character );

/**
 * @brief Whether the character is a zero width joiner.
 *
 * @param[in] character The character.
 *
 * @return @e true if the character is a zero width joiner.
 */
DALI_IMPORT_API bool IsZeroWidthJoiner( Character character );

/**
 * @brief Whether the character is a zero width space.
 *
 * @param[in] character The character.
 *
 * @return @e true if the character is a zero width space.
 */
DALI_IMPORT_API bool IsZeroWidthSpace( Character character );

/**
 * @brief Whether the character is a left to right mark.
 *
 * @param[in] character The character.
 *
 * @return @e true if the character is a left to right mark.
 */
DALI_IMPORT_API bool IsLeftToRightMark( Character character );

/**
 * @brief Whether the character is a right to left mark.
 *
 * @param[in] character The character.
 *
 * @return @e true if the character is a right to left mark.
 */
DALI_IMPORT_API bool IsRightToLeftMark( Character character );

/**
 * @brief Whether the character is a thin space.
 *
 * @param[in] character The character.
 *
 * @return @e true if the character is a thin space.
 */
DALI_IMPORT_API bool IsThinSpace( Character character );

/**
 * @brief Whether the character is common within all scripts.
 *
 * @param[in] character The character.
 *
 * @return @e true if the character is common within all scripts.
 */
DALI_IMPORT_API bool IsCommonScript( Character character );

/**
 * @brief Whether the script contains ligatures that must be 'broken' for selection or cursor position.
 *
 * i.e The latin script has the 'ff' or 'fi' ligatures that need to be broken to position the cursor
 * between the two characters. Equally the arabic script has the 'ﻻ' ligature that needs to be broken.
 *
 * @param[in] script The script.
 *
 * @return @e true if the script has ligatures that must be 'broken'.
 */
DALI_IMPORT_API bool HasLigatureMustBreak( Script script );
} // namespace TextAbstraction

} // namespace Dali

#endif // __DALI_TOOLKIT_TEXT_ABSTRACTION_SCRIPT_H__
