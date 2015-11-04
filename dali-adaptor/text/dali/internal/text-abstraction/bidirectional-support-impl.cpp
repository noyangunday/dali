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

// CLASS  HEADER
#include "bidirectional-support-impl.h"

// INTERNAL INCLUDES
#include <singleton-service-impl.h>

// EXTERNAL INCLUDES
#include <memory.h>
#include <fribidi/fribidi.h>

namespace Dali
{

namespace TextAbstraction
{

namespace Internal
{

namespace
{
  typedef unsigned char BidiDirection;

  // Internal charcter's direction.
  const BidiDirection LEFT_TO_RIGHT = 0u;
  const BidiDirection NEUTRAL = 1u;
  const BidiDirection RIGHT_TO_LEFT = 2u;

  /**
   * @param[in] paragraphDirection The FriBiDi paragraph's direction.
   *
   * @return Whether the paragraph is right to left.
   */
  bool GetBidiParagraphDirection( FriBidiParType paragraphDirection )
  {
    switch( paragraphDirection )
    {
      case FRIBIDI_PAR_RTL:  // Right-To-Left paragraph.
      case FRIBIDI_PAR_WRTL: // Weak Right To Left paragraph.
      {
        return true;
      }
      case FRIBIDI_PAR_LTR:  // Left-To-Right paragraph.
      case FRIBIDI_PAR_ON:   // DirectiOn-Neutral paragraph.
      case FRIBIDI_PAR_WLTR: // Weak Left To Right paragraph.
      {
        return false;
      }
    }

    return false;
  }

  BidiDirection GetBidiCharacterDirection( FriBidiCharType characterDirection )
  {
    switch( characterDirection )
    {
      case FRIBIDI_TYPE_LTR: // Left-To-Right letter.
      case FRIBIDI_TYPE_EN:  // European Numeral.
      case FRIBIDI_TYPE_AN:  // Arabic Numeral.
      case FRIBIDI_TYPE_ES:  // European number Separator.
      case FRIBIDI_TYPE_ET:  // European number Terminator.
      {
        return LEFT_TO_RIGHT;
      }
      case FRIBIDI_TYPE_RTL: // Right-To-Left letter.
      case FRIBIDI_TYPE_AL:  // Arabic Letter.
      {
        return RIGHT_TO_LEFT;
      }
    }

    return NEUTRAL;
  }
}

struct BidirectionalSupport::Plugin
{
  /**
   * Stores bidirectional info per paragraph.
   */
  struct BidirectionalInfo
  {
    FriBidiCharType* characterTypes;      ///< The type of each character (right, left, neutral, ...)
    FriBidiLevel*    embeddedLevels;      ///< Embedded levels.
    FriBidiParType   paragraphDirection;  ///< The paragraph's direction.
  };

  Plugin()
  : mParagraphBidirectionalInfo(),
    mFreeIndices()
  {}

  ~Plugin()
  {
    // free all resources.
    for( Vector<BidirectionalInfo*>::Iterator it = mParagraphBidirectionalInfo.Begin(),
           endIt = mParagraphBidirectionalInfo.End();
         it != endIt;
         ++it )
    {
      BidirectionalInfo* info = *it;

      free( info->embeddedLevels );
      free( info->characterTypes );
      delete info;
    }
  }

  BidiInfoIndex CreateInfo( const Character* const paragraph,
                            Length numberOfCharacters )
  {
    // Reserve memory for the paragraph's bidirectional info.
    BidirectionalInfo* bidirectionalInfo = new BidirectionalInfo();

    bidirectionalInfo->characterTypes = reinterpret_cast<FriBidiCharType*>( malloc( numberOfCharacters * sizeof( FriBidiCharType ) ) );
    if( !bidirectionalInfo->characterTypes )
    {
      delete bidirectionalInfo;
      return 0;
    }

    bidirectionalInfo->embeddedLevels = reinterpret_cast<FriBidiLevel*>( malloc( numberOfCharacters * sizeof( FriBidiLevel ) ) );
    if( !bidirectionalInfo->embeddedLevels )
    {
      free( bidirectionalInfo->characterTypes );
      delete bidirectionalInfo;
      return 0;
    }

    // Retrieve the type of each character..
    fribidi_get_bidi_types( paragraph, numberOfCharacters, bidirectionalInfo->characterTypes );

    // Retrieve the paragraph's direction.
    bidirectionalInfo->paragraphDirection = fribidi_get_par_direction( bidirectionalInfo->characterTypes, numberOfCharacters );

    // Retrieve the embedding levels.
    fribidi_get_par_embedding_levels( bidirectionalInfo->characterTypes, numberOfCharacters, &bidirectionalInfo->paragraphDirection, bidirectionalInfo->embeddedLevels );

    // Store the bidirectional info and return the index.
    BidiInfoIndex index = 0u;
    if( 0u != mFreeIndices.Count() )
    {
      Vector<BidiInfoIndex>::Iterator it = mFreeIndices.End() - 1u;

      index = *it;

      mFreeIndices.Remove( it );

      *( mParagraphBidirectionalInfo.Begin() + index ) = bidirectionalInfo;
    }
    else
    {
      index = static_cast<BidiInfoIndex>( mParagraphBidirectionalInfo.Count() );

      mParagraphBidirectionalInfo.PushBack( bidirectionalInfo );
    }

    return index;
  }

  void DestroyInfo( BidiInfoIndex bidiInfoIndex )
  {
    if( bidiInfoIndex >= mParagraphBidirectionalInfo.Count() )
    {
      return;
    }

    // Retrieve the paragraph's bidirectional info.
    Vector<BidirectionalInfo*>::Iterator it = mParagraphBidirectionalInfo.Begin() + bidiInfoIndex;
    BidirectionalInfo* bidirectionalInfo = *it;

    if( NULL != bidirectionalInfo )
    {
      // Free resources and destroy the container.
      free( bidirectionalInfo->embeddedLevels );
      free( bidirectionalInfo->characterTypes );
      delete bidirectionalInfo;

      *it = NULL;
    }

    // Add the index to the free indices vector.
    mFreeIndices.PushBack( bidiInfoIndex );
  }

  void Reorder( BidiInfoIndex bidiInfoIndex,
                CharacterIndex firstCharacterIndex,
                Length numberOfCharacters,
                CharacterIndex* visualToLogicalMap )
  {
    const FriBidiFlags flags = FRIBIDI_FLAGS_DEFAULT | FRIBIDI_FLAGS_ARABIC;

    // Retrieve the paragraph's bidirectional info.
    const BidirectionalInfo* const bidirectionalInfo = *( mParagraphBidirectionalInfo.Begin() + bidiInfoIndex );

    // Initialize the visual to logical mapping table to the identity. Otherwise fribidi_reorder_line fails to retrieve a valid mapping table.
    for( CharacterIndex index = 0u; index < numberOfCharacters; ++index )
    {
      visualToLogicalMap[ index ] = index;
    }

    // Copy embedded levels as fribidi_reorder_line() may change them.
    const uint32_t embeddedLevelsSize = numberOfCharacters * sizeof( FriBidiLevel );
    FriBidiLevel* embeddedLevels = reinterpret_cast<FriBidiLevel*>( malloc( embeddedLevelsSize ) );
    if( embeddedLevels )
    {
      memcpy( embeddedLevels, bidirectionalInfo->embeddedLevels + firstCharacterIndex,  embeddedLevelsSize );

      // Reorder the line.
      fribidi_reorder_line( flags,
                            bidirectionalInfo->characterTypes + firstCharacterIndex,
                            numberOfCharacters,
                            0u,
                            bidirectionalInfo->paragraphDirection,
                            embeddedLevels,
                            NULL,
                            reinterpret_cast<FriBidiStrIndex*>( visualToLogicalMap ) );

      // Free resources.
      free( embeddedLevels );
    }
  }

  bool GetMirroredText( Character* text,
                        Length numberOfCharacters ) const
  {
    bool updated = false;

    for( CharacterIndex index = 0u; index < numberOfCharacters; ++index )
    {
      // Get a reference to the character inside the text.
      Character& character = *( text + index );

      // Retrieve the mirrored character.
      FriBidiChar mirroredCharacter = character;
      const bool mirrored = fribidi_get_mirror_char( character, &mirroredCharacter );
      updated = updated || mirrored;

      // Update the character inside the text.
      character = mirroredCharacter;
    }

    return updated;
  }

  bool GetParagraphDirection( BidiInfoIndex bidiInfoIndex ) const
  {
    // Retrieve the paragraph's bidirectional info.
    const BidirectionalInfo* const bidirectionalInfo = *( mParagraphBidirectionalInfo.Begin() + bidiInfoIndex );

    return GetBidiParagraphDirection( bidirectionalInfo->paragraphDirection );
  }

  void GetCharactersDirection( BidiInfoIndex bidiInfoIndex,
                               CharacterDirection* directions,
                               Length numberOfCharacters )
  {
    const BidirectionalInfo* const bidirectionalInfo = *( mParagraphBidirectionalInfo.Begin() + bidiInfoIndex );

    const CharacterDirection paragraphDirection = GetBidiParagraphDirection( bidirectionalInfo->paragraphDirection );
    CharacterDirection previousDirection = paragraphDirection;

    for( CharacterIndex index = 0u; index < numberOfCharacters; ++index )
    {
      CharacterDirection& characterDirection = *( directions + index );
      characterDirection = false;

      // Get the bidi direction.
      const BidiDirection bidiDirection = GetBidiCharacterDirection( *( bidirectionalInfo->characterTypes + index ) );

      if( RIGHT_TO_LEFT == bidiDirection )
      {
        characterDirection = true;
      }
      else if( NEUTRAL == bidiDirection )
      {
        // For neutral characters it check's the next and previous directions.
        // If they are equals set that direction. If they are not, sets the paragraph's direction.
        // If there is no next, sets the paragraph's direction.

        CharacterDirection nextDirection = paragraphDirection;

        // Look for the next non-neutral character.
        Length nextIndex = index + 1u;
        for( ; nextIndex < numberOfCharacters; ++nextIndex )
        {
          BidiDirection nextBidiDirection = GetBidiCharacterDirection( *( bidirectionalInfo->characterTypes + nextIndex ) );
          if( nextBidiDirection != NEUTRAL )
          {
            nextDirection = RIGHT_TO_LEFT == nextBidiDirection;
            break;
          }
        }

        // Calculate the direction for all the neutral characters.
        characterDirection = previousDirection == nextDirection ? previousDirection : paragraphDirection;

        // Set the direction to all the neutral characters.
        ++index;
        for( ; index < nextIndex; ++index )
        {
          CharacterDirection& nextCharacterDirection = *( directions + index );
          nextCharacterDirection = characterDirection;
        }

        // Set the direction of the next non-neutral character.
        if( nextIndex < numberOfCharacters )
        {
          *( directions + nextIndex ) = nextDirection;
        }
      }

      previousDirection = characterDirection;
    }
  }

  Vector<BidirectionalInfo*> mParagraphBidirectionalInfo; ///< Stores the bidirectional info per paragraph.
  Vector<BidiInfoIndex>      mFreeIndices;                ///< Stores indices of free positions in the bidirectional info vector.
};

BidirectionalSupport::BidirectionalSupport()
: mPlugin( NULL )
{
}

BidirectionalSupport::~BidirectionalSupport()
{
  delete mPlugin;
}

TextAbstraction::BidirectionalSupport BidirectionalSupport::Get()
{
  TextAbstraction::BidirectionalSupport bidirectionalSupportHandle;

  SingletonService service( SingletonService::Get() );
  if( service )
  {
    // Check whether the singleton is already created
    BaseHandle handle = service.GetSingleton( typeid( TextAbstraction::BidirectionalSupport ) );
    if(handle)
    {
      // If so, downcast the handle
      BidirectionalSupport* impl = dynamic_cast< Internal::BidirectionalSupport* >( handle.GetObjectPtr() );
      bidirectionalSupportHandle = TextAbstraction::BidirectionalSupport( impl );
    }
    else // create and register the object
    {
      bidirectionalSupportHandle = TextAbstraction::BidirectionalSupport( new BidirectionalSupport );
      service.Register( typeid( bidirectionalSupportHandle ), bidirectionalSupportHandle );
    }
  }

  return bidirectionalSupportHandle;
}

BidiInfoIndex BidirectionalSupport::CreateInfo( const Character* const paragraph,
                                                Length numberOfCharacters )
{
  CreatePlugin();

  return mPlugin->CreateInfo( paragraph,
                              numberOfCharacters );
}

void BidirectionalSupport::DestroyInfo( BidiInfoIndex bidiInfoIndex )
{
  CreatePlugin();

  mPlugin->DestroyInfo( bidiInfoIndex );
}

void BidirectionalSupport::Reorder( BidiInfoIndex bidiInfoIndex,
                                    CharacterIndex firstCharacterIndex,
                                    Length numberOfCharacters,
                                    CharacterIndex* visualToLogicalMap )
{
  CreatePlugin();

  mPlugin->Reorder( bidiInfoIndex,
                    firstCharacterIndex,
                    numberOfCharacters,
                    visualToLogicalMap );
}

bool BidirectionalSupport::GetMirroredText( Character* text,
                                            Length numberOfCharacters )
{
  CreatePlugin();

  return mPlugin->GetMirroredText( text, numberOfCharacters );
}

bool BidirectionalSupport::GetParagraphDirection( BidiInfoIndex bidiInfoIndex ) const
{
  if( !mPlugin )
  {
    return false;
  }

  return mPlugin->GetParagraphDirection( bidiInfoIndex );
}

void BidirectionalSupport::GetCharactersDirection( BidiInfoIndex bidiInfoIndex,
                                                   CharacterDirection* directions,
                                                   Length numberOfCharacters )
{
  CreatePlugin();

  mPlugin->GetCharactersDirection( bidiInfoIndex,
                                   directions,
                                   numberOfCharacters );
}

void BidirectionalSupport::CreatePlugin()
{
  if( !mPlugin )
  {
    mPlugin = new Plugin();
  }
}

} // namespace Internal

} // namespace TextAbstraction

} // namespace Dali
