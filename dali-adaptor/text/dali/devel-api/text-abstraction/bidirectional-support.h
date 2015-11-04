#ifndef __DALI_PLATFORM_TEXT_ABSTRACTION_BIDIRECTIONAL_SUPPORT_H__
#define __DALI_PLATFORM_TEXT_ABSTRACTION_BIDIRECTIONAL_SUPPORT_H__

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

class BidirectionalSupport;

} // Internal

/**
 * BidirectionalSupport API
 *
 */
class DALI_IMPORT_API BidirectionalSupport : public BaseHandle
{

public:

  /**
   * @brief Create an uninitialized TextAbstraction handle.
   *
   */
  BidirectionalSupport();

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~BidirectionalSupport();

  /**
   * @brief This constructor is used by BidirectionalSupport::Get().
   *
   * @param[in] implementation a pointer to the internal bidirectional support object.
   */
  explicit DALI_INTERNAL BidirectionalSupport( Internal::BidirectionalSupport* implementation );

  /**
   * @brief Retrieve a handle to the BidirectionalSupport instance.
   *
   * @return A handle to the BidirectionalSupport
   */
  static BidirectionalSupport Get();

  /**
   * @brief Creates bidirectional data for the whole paragraph.
   *
   * @param[in] paragraph Pointer to the first character of the paragraph coded in UTF32.
   * @param[in] numberOfCharacters The number of characters of the paragraph.
   *
   * @return An index of an object inside a table storing the bidirectional data.
   */
  BidiInfoIndex CreateInfo( const Character* const paragraph,
                            Length numberOfCharacters );

  /**
   * @brief Destroys the bidirectional data.
   *
   * @param[in] bidiInfoIndex The index to the of the object inside the table storing the bidirectional data for the current paragraph.
   */
  void DestroyInfo( BidiInfoIndex bidiInfoIndex );

  /**
   * @brief Reorders a line of a paragraph.
   *
   * @pre visualToLogicalMap must have enough space allocated for @p numberOfCharacters.
   *
   * @param[in] bidiInfoIndex The index to the of the object inside the table storing the bidirectional data for the current paragraph.
   * @param[in] firstCharacterIndex The first character of the line within the whole paragraph.
   * @param[in] numberOfCharacters The number of characters of the line.
   * @param[out] visualToLogicalMap The visual to logical conversion map.
   */
  void Reorder( BidiInfoIndex bidiInfoIndex,
                CharacterIndex firstCharacterIndex,
                Length numberOfCharacters,
                CharacterIndex* visualToLogicalMap );

  /**
   * @brief Replaces any character which could be mirrored.
   *
   * @param[in,out] text The text.
   * @param[in] numberOfCharacters The number of characters.
   *
   * @return @e true if a character has been replaced.
   */
  bool GetMirroredText( Character* text,
                        Length numberOfCharacters );

  /**
   * @brief Retrieves the paragrpah's direction.
   *
   * @param[in] bidiInfoIndex The index to the of the object inside the table storing the bidirectional data for the current paragraph.
   *
   * @return @e true if the paragraph is right to left, otherwise @e false.
   */
  bool GetParagraphDirection( BidiInfoIndex bidiInfoIndex ) const;

  /**
   * @brief Retrieves the character's directions.
   *
   * @param[in] bidiInfoIndex The index to the of the object inside the table storing the bidirectional data for the current paragraph.
   * @param[out] directions The direction, @e false is left to right and @e true is right to left, of each character of the paragraph.
   * @param[in] numberOfCharacters The number of characters.
   */
  void GetCharactersDirection( BidiInfoIndex bidiInfoIndex,
                               CharacterDirection* directions,
                               Length numberOfCharacters );
};

} // namespace TextAbstraction

} // namespace Dali

#endif // __DALI_PLATFORM_TEXT_ABSTRACTION_BIDIRECTIONAL_SUPPORT_H__
