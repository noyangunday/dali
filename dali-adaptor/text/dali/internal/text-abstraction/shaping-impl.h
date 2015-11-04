#ifndef __DALI_INTERNAL_TEXT_ABSTRACTION_SHAPING_IMPL_H__
#define __DALI_INTERNAL_TEXT_ABSTRACTION_SHAPING_IMPL_H__

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
#include <dali/public-api/object/base-object.h>

// INTERNAL INCLUDES
#include <dali/public-api/common/dali-vector.h>
#include <dali/devel-api/text-abstraction/shaping.h>

namespace Dali
{

namespace TextAbstraction
{

namespace Internal
{

/**
 * Implementation of the Shaping
 */
class Shaping : public BaseObject
{
public:

  /**
   * Constructor
   */
  Shaping();

  /**
   * Destructor
   */
  ~Shaping();

  /**
   * @copydoc Dali::Shaping::Get()
   */
  static TextAbstraction::Shaping Get();

  /**
   * @copydoc Dali::Shaping::Shape()
   */
  Length Shape( const Character* const text,
                Length numberOfCharacters,
                FontId fontId,
                Script script );

  /**
   * @copydoc Dali::Shaping::GetGlyphs()
   */
  void GetGlyphs( GlyphInfo* glyphInfo,
                  CharacterIndex* glyphToCharacterMap );

private:

  /**
   * Helper for lazy initialization.
   */
  void CreatePlugin();

private:

  // Undefined copy constructor.
  Shaping( const Shaping& );

  // Undefined assignment constructor.
  Shaping& operator=( Shaping& );

  struct Plugin;
  Plugin* mPlugin;

}; // class Shaping

} // namespace Internal

} // namespace TextAbstraction

inline static TextAbstraction::Internal::Shaping& GetImplementation( TextAbstraction::Shaping& shaping )
{
  DALI_ASSERT_ALWAYS( shaping && "shaping handle is empty" );
  BaseObject& handle = shaping.GetBaseObject();
  return static_cast<TextAbstraction::Internal::Shaping&>( handle );
}

inline static const TextAbstraction::Internal::Shaping& GetImplementation( const TextAbstraction::Shaping& shaping )
{
  DALI_ASSERT_ALWAYS( shaping && "shaping handle is empty" );
  const BaseObject& handle = shaping.GetBaseObject();
  return static_cast<const TextAbstraction::Internal::Shaping&>( handle );
}

} // namespace Dali

#endif // __DALI_INTERNAL_TEXT_ABSTRACTION_SHAPING_IMPL_H__
