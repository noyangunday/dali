#ifndef __DALI_INTERNAL_TEXT_ABSTRACTION_SEGMENTATION_IMPL_H__
#define __DALI_INTERNAL_TEXT_ABSTRACTION_SEGMENTATION_IMPL_H__

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
#include <dali/devel-api/text-abstraction/segmentation.h>

namespace Dali
{

namespace TextAbstraction
{

namespace Internal
{

/**
 * Implementation of the Segmentation
 */

class Segmentation : public BaseObject
{
public:

  /**
   * Constructor
   */
  Segmentation();

  /**
   * Destructor
   */
  ~Segmentation();

  /**
   * @copydoc Dali::Segmentation::Get()
   */
  static TextAbstraction::Segmentation Get();

  /**
   * @copydoc Dali::Segmentation::GetLineBreakPositions()
   */
  void GetLineBreakPositions( const Character* const text,
                              Length numberOfCharacters,
                              LineBreakInfo* breakInfo );


  /**
   * @copydoc Dali::Segmentation::GetWordBreakPositions()
   */
  void GetWordBreakPositions( const Character* const text,
                              Length numberOfCharacters,
                              WordBreakInfo* breakInfo );

private:

  /**
   * Helper for lazy initialization.
   */
  void CreatePlugin();

private:

  // Undefined copy constructor.
  Segmentation( const Segmentation& );

  // Undefined assignment constructor.
  Segmentation& operator=( Segmentation& );

private:

  struct Plugin;
  Plugin* mPlugin;

}; // class Segmentation

} // namespace Internal

} // namespace TextAbstraction

inline static TextAbstraction::Internal::Segmentation& GetImplementation( TextAbstraction::Segmentation& segmentation )
{
  DALI_ASSERT_ALWAYS( segmentation && "segmentation handle is empty" );
  BaseObject& handle = segmentation.GetBaseObject();
  return static_cast<TextAbstraction::Internal::Segmentation&>( handle );
}

inline static const TextAbstraction::Internal::Segmentation& GetImplementation( const TextAbstraction::Segmentation& segmentation )
{
  DALI_ASSERT_ALWAYS( segmentation && "segmentation handle is empty" );
  const BaseObject& handle = segmentation.GetBaseObject();
  return static_cast<const TextAbstraction::Internal::Segmentation&>( handle );
}

} // namespace Dali

#endif // __DALI_INTERNAL_TEXT_ABSTRACTION_SEGMENTATION_IMPL_H__
