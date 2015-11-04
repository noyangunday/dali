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
#include <dali/devel-api/text-abstraction/bidirectional-support.h>

// INTERNAL INCLUDES
#include <dali/internal/text-abstraction/bidirectional-support-impl.h>

namespace Dali
{

namespace TextAbstraction
{

BidirectionalSupport::BidirectionalSupport()
{
}

BidirectionalSupport::~BidirectionalSupport()
{
}

BidirectionalSupport::BidirectionalSupport( Internal::BidirectionalSupport* implementation )
: BaseHandle( implementation )
{
}

BidirectionalSupport BidirectionalSupport::Get()
{
  return Internal::BidirectionalSupport::Get();
}

BidiInfoIndex BidirectionalSupport::CreateInfo( const Character* const paragraph,
                                                Length numberOfCharacters )
{
  return GetImplementation( *this ).CreateInfo( paragraph,
                                                numberOfCharacters );
}

void BidirectionalSupport::DestroyInfo( BidiInfoIndex bidiInfoIndex )
{
  GetImplementation( *this ).DestroyInfo( bidiInfoIndex );
}

void BidirectionalSupport::Reorder( BidiInfoIndex bidiInfoIndex,
                                    CharacterIndex firstCharacterIndex,
                                    Length numberOfCharacters,
                                    CharacterIndex* visualToLogicalMap )
{
  GetImplementation( *this ).Reorder( bidiInfoIndex,
                                      firstCharacterIndex,
                                      numberOfCharacters,
                                      visualToLogicalMap );
}

bool BidirectionalSupport::GetMirroredText( Character* text,
                                            Length numberOfCharacters )
{
  return GetImplementation( *this ).GetMirroredText( text,
                                                     numberOfCharacters );
}

bool BidirectionalSupport::GetParagraphDirection( BidiInfoIndex bidiInfoIndex ) const
{
  return GetImplementation( *this ).GetParagraphDirection( bidiInfoIndex );
}

void BidirectionalSupport::GetCharactersDirection( BidiInfoIndex bidiInfoIndex,
                                                   CharacterDirection* directions,
                                                   Length numberOfCharacters )
{
  GetImplementation( *this ).GetCharactersDirection( bidiInfoIndex,
                                                     directions,
                                                     numberOfCharacters );
}

} // namespace TextAbstraction

} // namespace Dali
