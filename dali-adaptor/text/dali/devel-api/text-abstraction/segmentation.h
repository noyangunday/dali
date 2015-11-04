#ifndef __DALI_PLATFORM_TEXT_ABSTRACTION_SEGMENTATION_H__
#define __DALI_PLATFORM_TEXT_ABSTRACTION_SEGMENTATION_H__

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
#include <dali/devel-api/text-abstraction/text-abstraction-definitions.h>

// EXTERNAL INCLUDES
#include <dali/public-api/object/base-handle.h>

namespace Dali
{

namespace TextAbstraction
{

namespace Internal DALI_INTERNAL
{

class Segmentation;

} // Internal

} // TextAbstraction

namespace TextAbstraction
{

/**
 *   Segmentation API
 *
 */
class DALI_IMPORT_API Segmentation : public BaseHandle
{

public:

  /**
   * @brief Create an uninitialized TextAbstraction handle.
   *
   */
  Segmentation();

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~Segmentation();

  /**
   * @brief This constructor is used by Segmentation::Get().
   *
   * @param[in] implementation A pointer to the internal segmentation object.
   */
  explicit DALI_INTERNAL Segmentation( Internal::Segmentation* implementation );

  /**
   * @brief Retrieve a handle to the Segmentation instance.
   *
   * @return A handle to the Segmentation
   */
  static Segmentation Get();

  /**
   * @brief Retrieves the line break info.
   *
   * @pre @p breakInfo must have enough space allocated for @p numberOfCharacters.
   *
   * Possible values for LineBreakInfo are:
   *
   *  - 0 is a LINE_MUST_BREAK.  Text must be broken into a new line.
   *  - 1 is a LINE_ALLOW_BREAK. Is possible to break the text into a new line.
   *  - 2 is a LINE_NO_BREAK.    Text can't be broken into a new line.
   *
     @verbatim
     i.e. Hello big\nworld produces:
          2222212220 22220
     @endverbatim
   *
   * @param[in] text Pointer to the first character of the text coded in UTF32.
   * @param[in] numberOfCharacters The number of characters.
   * @param[out] breakInfo The line break info.
   */
  void GetLineBreakPositions( const Character* const text,
                              Length numberOfCharacters,
                              LineBreakInfo* breakInfo );

  /**
   * @brief Retrieves the word break info.
   *
   * @pre @p breakInfo must have enough space allocated for @p numberOfCharacters.
   *
   * Possible values for WordBreakInfo are:
   *
   * - 0 is a WORD_BREAK.    Text can be broken into a new word.
   * - 1 is a WORD_NO_BREAK. Text can't be broken into a new word.
   *
     @verbatim
     i.e. Hello big\nworld produces:
          1111001100 11110
     @endverbatim
   *
   * @param[in] text Pointer to the first character of the text coded in UTF32.
   * @param[in] numberOfCharacters The number of characters.
   * @param[out] breakInfo The word break info.
   */
  void GetWordBreakPositions( const Character* const text,
                              Length numberOfCharacters,
                              WordBreakInfo* breakInfo );
};

} // namespace TextAbstraction

} // namespace Dali

#endif // __DALI_PLATFORM_TEXT_ABSTRACTION_SEGMENTATION_H__
