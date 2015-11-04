#ifndef __DALI_PLATFORM_TEXT_ABSTRACTION_SHAPING_H__
#define __DALI_PLATFORM_TEXT_ABSTRACTION_SHAPING_H__

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

// INTERNAL INCLUDES
#include <dali/devel-api/text-abstraction/script.h>
#include <dali/devel-api/text-abstraction/text-abstraction-definitions.h>

// EXTERNAL INCLUDES
#include <dali/public-api/object/base-handle.h>

namespace Dali
{

namespace TextAbstraction
{

struct GlyphInfo;

namespace Internal DALI_INTERNAL
{

class Shaping;

} // Internal

/**
 * @brief Shaping provides an interface to retrieve glyphs from complex text.
 *
 * This module shapes text for a unique font id and script. If the text contains different fonts and scripts
 * it needs to be split in runs of consecutive characters with the same font id and script.
 *
 * @code
 * Shaping shaping = Shaping::Get();
 *
 * // Shapes a number of characters with the given font id and script.
 * const Length numberOfGlyphs = shaping.Shape( text, numberOfCharacters, fontId, script );
 *
 * // Allocate memory to retrieve the glyphs and the character to glyph conversion map.
 * GlyphInfo* glyphInfo = reinterpret_cast<GlyphInfo*>( malloc( numberOfGlyphs * sizeof( GlyphInfo ) ) );
 * CharacterIndex* glyphToCharacterMap = reinterpret_cast<CharacterIndex*>( malloc( numberOfGlyphs * sizeof( CharacterIndex ) ) );
 *
 * // Retrieve the glyphs and the conversion map.
 * shaping.GetGlyphs( glyphInfo, glyphToCharacterMap );
 * @endcode
 */
class DALI_IMPORT_API Shaping : public BaseHandle
{

public:

  /**
   * @brief Create an uninitialized Shaping handle.
   *
   */
  Shaping();

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~Shaping();

  /**
   * @brief This constructor is used by Shaping::Get().
   *
   * @param[in] implementation A pointer to the internal shaping object.
   */
  explicit DALI_INTERNAL Shaping( Internal::Shaping* implementation );

  /**
   * @brief Retrieve a handle to the Shaping instance.
   *
   * @return A handle to the Shaping.
   */
  static Shaping Get();

  /**
   * Shapes the text.
   *
   * Call GetGlyphs() to retrieve the glyphs.
   *
   * @param[in] text Pointer to the first character of the text coded in UTF32.
   * @param[in] numberOfCharacters The number of characters to be shaped
   * @param[in] fontId The font to be used to shape the text.
   * @param[in] script The text's script.
   *
   * @return The size of the buffer required to get the shaped text.
   */
  Length Shape( const Character* const text,
                Length numberOfCharacters,
                FontId fontId,
                Script script );

  /**
   * Gets the shaped text data.
   *
   * @pre @p glyphInfo and @p glyphToCharacterMap must have enough space allocated for the number of glyphs.
   * Call first Shape() to shape the text and get the number of glyphs.
   *
   * @param[out] glyphInfo Vector with indices to the glyph within the font, glyph's metrics and advance.
   * @param[out] glyphToCharacterMap The glyph to character conversion map.
   */
  void GetGlyphs( GlyphInfo* glyphInfo,
                  CharacterIndex* glyphToCharacterMap );
};

} // namespace TextAbstraction

} // namespace Dali

#endif // __DALI_PLATFORM_TEXT_ABSTRACTION_SHAPING_H__
