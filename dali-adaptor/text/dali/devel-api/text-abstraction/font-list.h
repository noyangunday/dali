#ifndef __DALI_TEXT_ABSTRACTION_FONT_LIST_H__
#define __DALI_TEXT_ABSTRACTION_FONT_LIST_H__

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
#include <string>
#include <iostream>
#include <dali/public-api/common/vector-wrapper.h>

namespace Dali
{

namespace TextAbstraction
{
typedef std::string FontPath;
typedef std::string FontFamily;
typedef std::string FontStyle;

namespace FontWidth
{
  /**
   * @brief Enumeration type for the font's width
   */
  enum Type
  {
    ULTRA_CONDENSED,
    EXTRA_CONDENSED,
    CONDENSED,
    SEMI_CONDENSED,
    NORMAL,
    SEMI_EXPANDED,
    EXPANDED,
    EXTRA_EXPANDED,
    ULTRA_EXPANDED,
  };
} // namespace FontWidth

namespace FontWeight
{
  /**
   * @brief Enumeration type for the font's weight
   */
  enum Type
  {
    THIN,
    ULTRA_LIGHT,
    EXTRA_LIGHT = ULTRA_LIGHT,
    LIGHT,
    DEMI_LIGHT,
    SEMI_LIGHT = DEMI_LIGHT,
    BOOK,
    NORMAL,
    REGULAR = NORMAL,
    MEDIUM,
    DEMI_BOLD,
    SEMI_BOLD = DEMI_BOLD,
    BOLD,
    ULTRA_BOLD,
    EXTRA_BOLD = ULTRA_BOLD,
    BLACK,
    HEAVY = BLACK,
    EXTRA_BLACK = BLACK
  };
}

namespace FontSlant
{
  /**
   * @brief Enumeration type for the font's slant
   */
  enum Type
  {
    NORMAL,
    ROMAN = NORMAL,
    ITALIC,
    OBLIQUE
  };
} // namespace FontSlant

struct FontDescription
{
  FontDescription()
  : path(),
    family(),
    width( FontWidth::NORMAL ),
    weight( FontWeight::NORMAL ),
    slant( FontSlant::NORMAL )
  {}

  ~FontDescription()
  {}

  FontPath         path;   ///< The font's file name path.
  FontFamily       family; ///< The font's family name.
  FontWidth::Type  width;  ///< The font's width.
  FontWeight::Type weight; ///< The font's weight.
  FontSlant::Type  slant;  ///< The font's slant.
};

typedef std::vector<FontDescription> FontList;

DALI_IMPORT_API std::ostream& operator<<( std::ostream& o, const FontList& fontList );

} // namespace TextAbstraction

} // namespace Dali

#endif // __DALI_TEXT_ABSTRACTION_FONT_LIST_H__
