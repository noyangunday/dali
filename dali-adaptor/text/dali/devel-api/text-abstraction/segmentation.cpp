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

// CLASS HEADER
#include <dali/devel-api/text-abstraction/segmentation.h>

// INTERNAL INCLUDES
#include <dali/internal/text-abstraction/segmentation-impl.h>

namespace Dali
{

namespace TextAbstraction
{

Segmentation::Segmentation()
{
}

Segmentation::~Segmentation()
{
}

Segmentation::Segmentation( Internal::Segmentation* implementation )
: BaseHandle( implementation )
{
}

Segmentation Segmentation::Get()
{
  return Internal::Segmentation::Get();
}

void Segmentation::GetLineBreakPositions( const Character* const text,
                                          Length numberOfCharacters,
                                          LineBreakInfo* breakInfo )
{
  GetImplementation( *this ).GetLineBreakPositions( text,
                                                    numberOfCharacters,
                                                    breakInfo );
}

void Segmentation::GetWordBreakPositions( const Character* const text,
                                          Length numberOfCharacters,
                                          WordBreakInfo* breakInfo )
{
  GetImplementation( *this ).GetWordBreakPositions( text,
                                                    numberOfCharacters,
                                                    breakInfo );
}

} // namespace TextAbstraction

} // namespace Dali
