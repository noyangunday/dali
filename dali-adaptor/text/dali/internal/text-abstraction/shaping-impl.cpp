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
#include "shaping-impl.h"

// INTERNAL INCLUDES
#include <singleton-service-impl.h>
#include <dali/devel-api/text-abstraction/font-client.h>
#include <dali/devel-api/text-abstraction/glyph-info.h>
#include <dali/integration-api/debug.h>

// EXTERNAL INCLUDES
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

#include <ft2build.h>

namespace Dali
{

namespace TextAbstraction
{

namespace Internal
{

const char*        DEFAULT_LANGUAGE = "en";
const unsigned int DEFAULT_LANGUAGE_LENGTH = 2u;
const float        FROM_266 = 1.0f / 64.0f;

const hb_script_t SCRIPT_TO_HARFBUZZ[] =
{
  HB_SCRIPT_COMMON,

  HB_SCRIPT_CYRILLIC,
  HB_SCRIPT_GREEK,
  HB_SCRIPT_LATIN,

  HB_SCRIPT_ARABIC,
  HB_SCRIPT_HEBREW,

  HB_SCRIPT_ARMENIAN,
  HB_SCRIPT_GEORGIAN,

  HB_SCRIPT_HAN,
  HB_SCRIPT_HANGUL,
  HB_SCRIPT_HIRAGANA,
  HB_SCRIPT_KATAKANA,
  HB_SCRIPT_BOPOMOFO,

  HB_SCRIPT_BENGALI,
  HB_SCRIPT_MYANMAR,
  HB_SCRIPT_DEVANAGARI,
  HB_SCRIPT_GUJARATI,
  HB_SCRIPT_GURMUKHI,
  HB_SCRIPT_KANNADA,
  HB_SCRIPT_MALAYALAM,
  HB_SCRIPT_ORIYA,
  HB_SCRIPT_SINHALA,
  HB_SCRIPT_TAMIL,
  HB_SCRIPT_TELUGU,

  HB_SCRIPT_LAO,
  HB_SCRIPT_THAI,
  HB_SCRIPT_KHMER,

  HB_SCRIPT_UNKNOWN, // EMOJI
  HB_SCRIPT_UNKNOWN
};

struct Shaping::Plugin
{
  Plugin()
  : mFreeTypeLibrary( NULL ),
    mIndices(),
    mAdvance(),
    mCharacterMap(),
    mFontId( 0u )
  {
  }

  ~Plugin()
  {
    FT_Done_FreeType( mFreeTypeLibrary );
  }

  void Initialize()
  {
    int error = FT_Init_FreeType( &mFreeTypeLibrary );
    if( FT_Err_Ok != error )
    {
      DALI_LOG_ERROR( "FreeType Init error: %d\n", error );
    }
  }

  Length Shape( const Character* const text,
                Length numberOfCharacters,
                FontId fontId,
                Script script )
  {
    // Clear previoursly shaped texts.
    mIndices.Clear();
    mAdvance.Clear();
    mCharacterMap.Clear();
    mOffset.Clear();
    mFontId = fontId;

    // Reserve some space to avoid reallocations.
    const Length numberOfGlyphs = static_cast<Length>( 1.3f * static_cast<float>( numberOfCharacters ) );
    mIndices.Reserve( numberOfGlyphs );
    mAdvance.Reserve( numberOfGlyphs );
    mCharacterMap.Reserve( numberOfGlyphs );
    mOffset.Reserve( 2u * numberOfGlyphs );

    TextAbstraction::FontClient fontClient = TextAbstraction::FontClient::Get();

    // Get the font's path file name from the font Id.
    FontDescription fontDescription;
    fontClient.GetDescription( fontId, fontDescription );

    // Create a FreeType font's face.
    FT_Face face;
    FT_Error retVal = FT_New_Face( mFreeTypeLibrary, fontDescription.path.c_str(), 0u, &face );
    if( FT_Err_Ok != retVal )
    {
      DALI_LOG_ERROR( "Failed to open face: %s\n", fontDescription.path.c_str() );
      return 0u;
    }

    unsigned int horizontalDpi = 0u;
    unsigned int verticalDpi = 0u;
    fontClient.GetDpi( horizontalDpi, verticalDpi );

    FT_Set_Char_Size( face,
                      0u,
                      fontClient.GetPointSize( fontId ),
                      horizontalDpi,
                      verticalDpi );

    /* Get our harfbuzz font struct */
    hb_font_t* harfBuzzFont;
    harfBuzzFont = hb_ft_font_create( face, NULL );

    /* Create a buffer for harfbuzz to use */
    hb_buffer_t* harfBuzzBuffer = hb_buffer_create();

    const bool rtlDirection = IsRightToLeftScript( script );
    hb_buffer_set_direction( harfBuzzBuffer,
                             rtlDirection ? HB_DIRECTION_RTL : HB_DIRECTION_LTR ); /* or LTR */

    hb_buffer_set_script( harfBuzzBuffer,
                          SCRIPT_TO_HARFBUZZ[ script ] ); /* see hb-unicode.h */

    hb_buffer_set_language( harfBuzzBuffer,
                            hb_language_from_string( DEFAULT_LANGUAGE,
                                                     DEFAULT_LANGUAGE_LENGTH ) );

    /* Layout the text */
    hb_buffer_add_utf32( harfBuzzBuffer, text, numberOfCharacters, 0u, numberOfCharacters );

    hb_shape( harfBuzzFont, harfBuzzBuffer, NULL, 0u );

    /* Get glyph data */
    unsigned int glyphCount;
    hb_glyph_info_t* glyphInfo = hb_buffer_get_glyph_infos( harfBuzzBuffer, &glyphCount );
    hb_glyph_position_t *glyphPositions = hb_buffer_get_glyph_positions( harfBuzzBuffer, &glyphCount );
    const GlyphIndex lastGlyphIndex = glyphCount - 1u;
    for( GlyphIndex i = 0u; i < glyphCount; )
    {
      if( rtlDirection )
      {
        // If the direction is right to left, Harfbuzz retrieves the glyphs in the visual order.
        // The glyphs are needed in the logical order to layout the text in lines.
        // Do not change the order of the glyphs if they belong to the same cluster.
        GlyphIndex rtlIndex = lastGlyphIndex - i;

        unsigned int cluster = glyphInfo[rtlIndex].cluster;
        unsigned int previousCluster = cluster;
        Length numberOfGlyphsInCluster = 0u;

        while( ( cluster == previousCluster ) )
        {
          ++numberOfGlyphsInCluster;
          previousCluster = cluster;

          if( rtlIndex > 0u )
          {
            --rtlIndex;

            cluster = glyphInfo[rtlIndex].cluster;
          }
          else
          {
            break;
          }
        }

        rtlIndex = lastGlyphIndex - ( i + ( numberOfGlyphsInCluster - 1u ) );

        for( GlyphIndex j = 0u; j < numberOfGlyphsInCluster; ++j )
        {
          const GlyphIndex index = rtlIndex + j;

          mIndices.PushBack( glyphInfo[index].codepoint );
          mAdvance.PushBack( floor( glyphPositions[index].x_advance * FROM_266 ) );
          mCharacterMap.PushBack( glyphInfo[index].cluster );
          mOffset.PushBack( floor( glyphPositions[index].x_offset * FROM_266 ) );
          mOffset.PushBack( floor( glyphPositions[index].y_offset * FROM_266 ) );
        }

        i += numberOfGlyphsInCluster;
      }
      else
      {
        mIndices.PushBack( glyphInfo[i].codepoint );
        mAdvance.PushBack( floor( glyphPositions[i].x_advance * FROM_266 ) );
        mCharacterMap.PushBack( glyphInfo[i].cluster );
        mOffset.PushBack( floor( glyphPositions[i].x_offset * FROM_266 ) );
        mOffset.PushBack( floor( glyphPositions[i].y_offset * FROM_266 ) );

        ++i;
      }
    }

    /* Cleanup */
    hb_buffer_destroy( harfBuzzBuffer );
    hb_font_destroy( harfBuzzFont );
    FT_Done_Face( face );

    return mIndices.Count();
  }

  void GetGlyphs( GlyphInfo* glyphInfo,
                  CharacterIndex* glyphToCharacterMap )
  {
    Vector<CharacterIndex>::ConstIterator indicesIt = mIndices.Begin();
    Vector<float>::ConstIterator advanceIt = mAdvance.Begin();
    Vector<float>::ConstIterator offsetIt = mOffset.Begin();
    Vector<CharacterIndex>::ConstIterator characterMapIt = mCharacterMap.Begin();

    for( GlyphIndex index = 0u, size = mIndices.Count(); index < size; ++index )
    {
      GlyphInfo& glyph = *( glyphInfo + index );
      CharacterIndex& glyphToCharacter = *( glyphToCharacterMap + index );

      glyph.fontId = mFontId;
      glyph.index = *( indicesIt + index );
      glyph.advance = *( advanceIt + index );

      const GlyphIndex offsetIndex = 2u * index;
      glyph.xBearing = *( offsetIt + offsetIndex );
      glyph.yBearing = *( offsetIt + offsetIndex + 1u );

      glyphToCharacter = *( characterMapIt + index );
    }
  }

  FT_Library             mFreeTypeLibrary;

  Vector<CharacterIndex> mIndices;
  Vector<float>          mAdvance;
  Vector<float>          mOffset;
  Vector<CharacterIndex> mCharacterMap;
  FontId                 mFontId;
};

Shaping::Shaping()
: mPlugin( NULL )
{
}

Shaping::~Shaping()
{
  delete mPlugin;
}

TextAbstraction::Shaping Shaping::Get()
{
  TextAbstraction::Shaping shapingHandle;

  SingletonService service( SingletonService::Get() );
  if( service )
  {
    // Check whether the singleton is already created
    Dali::BaseHandle handle = service.GetSingleton( typeid( TextAbstraction::Shaping ) );
    if( handle )
    {
      // If so, downcast the handle
      Shaping* impl = dynamic_cast< Internal::Shaping* >( handle.GetObjectPtr() );
      shapingHandle = TextAbstraction::Shaping( impl );
    }
    else // create and register the object
    {
      shapingHandle = TextAbstraction::Shaping( new Shaping );
      service.Register( typeid( shapingHandle ), shapingHandle );
    }
  }

  return shapingHandle;
}

Length Shaping::Shape( const Character* const text,
                       Length numberOfCharacters,
                       FontId fontId,
                       Script script )
{
  CreatePlugin();

  return mPlugin->Shape( text,
                         numberOfCharacters,
                         fontId,
                         script );
}

void Shaping::GetGlyphs( GlyphInfo* glyphInfo,
                         CharacterIndex* glyphToCharacterMap )
{
  CreatePlugin();

  mPlugin->GetGlyphs( glyphInfo,
                      glyphToCharacterMap );
}

void Shaping::CreatePlugin()
{
  if( !mPlugin )
  {
    mPlugin = new Plugin();
    mPlugin->Initialize();
  }
}

} // namespace Internal

} // namespace TextAbstraction

} // namespace Dali
