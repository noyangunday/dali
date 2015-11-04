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
#include <dali/internal/text-abstraction/font-client-plugin-impl.h>

// INTERNAL INCLUDES
#include <dali/devel-api/text-abstraction/font-list.h>
#include <dali/public-api/common/dali-vector.h>
#include <dali/public-api/common/vector-wrapper.h>
#include <dali/integration-api/debug.h>
#include <dali/integration-api/platform-abstraction.h>
#include <dali/internal/text-abstraction/font-client-helper.h>
#include <adaptor-impl.h>

// EXTERNAL INCLUDES
#include <fontconfig/fontconfig.h>

namespace
{

#if defined(DEBUG_ENABLED)
Dali::Integration::Log::Filter* gLogFilter = Dali::Integration::Log::Filter::New(Debug::NoLogging, false, "LOG_FONT_CLIENT");
#endif

/**
 * Conversion from Fractional26.6 to float
 */
const float FROM_266 = 1.0f / 64.0f;

const std::string FONT_FORMAT( "TrueType" );
const std::string DEFAULT_FONT_FAMILY_NAME( "Tizen" );
const int DEFAULT_FONT_WIDTH  = 100; // normal
const int DEFAULT_FONT_WEIGHT =  80; // normal
const int DEFAULT_FONT_SLANT  =   0; // normal

const uint32_t ELLIPSIS_CHARACTER = 0x2026;

const bool FONT_FIXED_SIZE_BITMAP( true );

// http://www.freedesktop.org/software/fontconfig/fontconfig-user.html

// ULTRA_CONDENSED 50
// EXTRA_CONDENSED 63
// CONDENSED       75
// SEMI_CONDENSED  87
// NORMAL         100
// SEMI_EXPANDED  113
// EXPANDED       125
// EXTRA_EXPANDED 150
// ULTRA_EXPANDED 200
const int FONT_WIDTH_TYPE_TO_INT[] = { 50, 63, 75, 87, 100, 113, 125, 150, 200 };
const unsigned int NUM_FONT_WIDTH_TYPE = sizeof( FONT_WIDTH_TYPE_TO_INT ) / sizeof( int );

// THIN                        0
// ULTRA_LIGHT, EXTRA_LIGHT   40
// LIGHT                      50
// DEMI_LIGHT, SEMI_LIGHT     55
// BOOK                       75
// NORMAL, REGULAR            80
// MEDIUM                    100
// DEMI_BOLD, SEMI_BOLD      180
// BOLD                      200
// ULTRA_BOLD, EXTRA_BOLD    205
// BLACK, HEAVY, EXTRA_BLACK 210
const int FONT_WEIGHT_TYPE_TO_INT[] = { 0, 40, 50, 55, 75, 80, 100, 180, 200, 205, 210 };
const unsigned int NUM_FONT_WEIGHT_TYPE = sizeof( FONT_WEIGHT_TYPE_TO_INT ) / sizeof( int );

// NORMAL, ROMAN   0
// ITALIC        100
// OBLIQUE       110
const int FONT_SLANT_TYPE_TO_INT[] = { 0, 100, 110 };
const unsigned int NUM_FONT_SLANT_TYPE = sizeof( FONT_SLANT_TYPE_TO_INT ) / sizeof( int );

} // namespace

using Dali::Vector;

namespace Dali
{

namespace TextAbstraction
{

namespace Internal
{

/**
 * @brief Returns the FontWidth's enum index for the given width value.
 *
 * @param[in] width The width value.
 *
 * @return The FontWidth's enum index.
 */
FontWidth::Type IntToWidthType( int width )
{
  return static_cast<FontWidth::Type>( ValueToIndex( width, FONT_WIDTH_TYPE_TO_INT, NUM_FONT_WIDTH_TYPE - 1u ) );
}

/**
 * @brief Returns the FontWeight's enum index for the given weight value.
 *
 * @param[in] weight The weight value.
 *
 * @return The FontWeight's enum index.
 */
FontWeight::Type IntToWeightType( int weight )
{
  return static_cast<FontWeight::Type>( ValueToIndex( weight, FONT_WEIGHT_TYPE_TO_INT, NUM_FONT_WEIGHT_TYPE - 1u ) );
}

/**
 * @brief Returns the FontSlant's enum index for the given slant value.
 *
 * @param[in] slant The slant value.
 *
 * @return The FontSlant's enum index.
 */
FontSlant::Type IntToSlantType( int slant )
{
  return static_cast<FontSlant::Type>( ValueToIndex( slant, FONT_SLANT_TYPE_TO_INT, NUM_FONT_SLANT_TYPE - 1u ) );
}

FontClient::Plugin::FallbackCacheItem::FallbackCacheItem( const FontDescription& font, FontList* list )
: fontDescription( font ),
  fallbackFonts( list )
{
}

FontClient::Plugin::FontDescriptionCacheItem::FontDescriptionCacheItem( const FontDescription& fontDescription,
                                                                        FontDescriptionId index )
: fontDescription( fontDescription ),
  index( index )
{
}

FontClient::Plugin::FontIdCacheItem::FontIdCacheItem( FontDescriptionId validatedFontId,
                                                      PointSize26Dot6 pointSize,
                                                      FontId fontId )
: validatedFontId( validatedFontId ),
  pointSize( pointSize ),
  fontId( fontId )
{
}

FontClient::Plugin::CacheItem::CacheItem( FT_Face ftFace,
                                          const FontPath& path,
                                          PointSize26Dot6 pointSize,
                                          FaceIndex face,
                                          const FontMetrics& metrics )
: mFreeTypeFace( ftFace ),
  mPath( path ),
  mPointSize( pointSize ),
  mFaceIndex( face ),
  mMetrics( metrics ),
  mFixedWidthPixels( 0.0f ),
  mFixedHeightPixels( 0.0f ),
  mIsFixedSizeBitmap( false )
{
}

FontClient::Plugin::CacheItem::CacheItem( FT_Face ftFace,
                                          const FontPath& path,
                                          PointSize26Dot6 pointSize,
                                          FaceIndex face,
                                          const FontMetrics& metrics,
                                          float fixedWidth,
                                          float fixedHeight )
: mFreeTypeFace( ftFace ),
  mPath( path ),
  mPointSize( pointSize ),
  mFaceIndex( face ),
  mMetrics( metrics ),
  mFixedWidthPixels( fixedWidth ),
  mFixedHeightPixels( fixedHeight ),
  mIsFixedSizeBitmap( true )
{
}

FontClient::Plugin::Plugin( unsigned int horizontalDpi,
                            unsigned int verticalDpi )
: mFreeTypeLibrary( NULL ),
  mDpiHorizontal( horizontalDpi ),
  mDpiVertical( verticalDpi ),
  mSystemFonts(),
  mDefaultFonts(),
  mFontCache(),
  mValidatedFontCache(),
  mFontDescriptionCache( 1u ),
  mFontIdCache(),
  mEllipsisCache()
{
  int error = FT_Init_FreeType( &mFreeTypeLibrary );
  if( FT_Err_Ok != error )
  {
    DALI_LOG_ERROR( "FreeType Init error: %d\n", error );
  }
}

FontClient::Plugin::~Plugin()
{
  for( std::vector<FallbackCacheItem>::iterator it = mFallbackCache.begin(), endIt = mFallbackCache.end();
       it != endIt;
       ++it )
  {
    FallbackCacheItem& item = *it;

    if( item.fallbackFonts )
    {
      delete item.fallbackFonts;
      item.fallbackFonts = NULL;
    }
  }

  FT_Done_FreeType( mFreeTypeLibrary );
}

void FontClient::Plugin::SetDpi( unsigned int horizontalDpi,
                                 unsigned int verticalDpi )
{
  mDpiHorizontal = horizontalDpi;
  mDpiVertical = verticalDpi;
}

void FontClient::Plugin::SetFontList( const FontDescription& fontDescription, FontList& fontList )
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::SetFontList family(%s)\n", fontDescription.family.c_str() );

  fontList.clear();

  FcPattern* fontFamilyPattern = CreateFontFamilyPattern( fontDescription );

  FcResult result = FcResultMatch;

  // Match the pattern.
  FcFontSet* fontSet = FcFontSort( NULL /* use default configure */,
                                   fontFamilyPattern,
                                   false /* don't trim */,
                                   NULL,
                                   &result );

  if( NULL != fontSet )
  {
    // Reserve some space to avoid reallocations.
    fontList.reserve( fontSet->nfont );

    for( int i = 0u; i < fontSet->nfont; ++i )
    {
      FcPattern* fontPattern = fontSet->fonts[i];

      FontPath path;

      // Skip fonts with no path
      if( GetFcString( fontPattern, FC_FILE, path ) )
      {
        fontList.push_back( FontDescription() );
        FontDescription& newFontDescription = fontList.back();

        newFontDescription.path = path;

        int width = 0;
        int weight = 0;
        int slant = 0;
        GetFcString( fontPattern, FC_FAMILY, newFontDescription.family );
        GetFcInt( fontPattern, FC_WIDTH, width );
        GetFcInt( fontPattern, FC_WEIGHT, weight );
        GetFcInt( fontPattern, FC_SLANT, slant );
        newFontDescription.width = IntToWidthType( width );
        newFontDescription.weight = IntToWeightType( weight );
        newFontDescription.slant = IntToSlantType( slant );
      }
    }

    FcFontSetDestroy( fontSet );
  }

  FcPatternDestroy( fontFamilyPattern );
}

void FontClient::Plugin::SetDefaultFont( const FontDescription& fontDescription )
{
  SetFontList( fontDescription, mDefaultFonts );
}

void FontClient::Plugin::GetDefaultFonts( FontList& defaultFonts )
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::GetDefaultFonts mDefaultFonts(%s)\n", ( mDefaultFonts.empty()?"empty":"valid" ) );

  if( mDefaultFonts.empty() )
  {
    FontDescription fontDescription;
    fontDescription.family = DEFAULT_FONT_FAMILY_NAME;  // todo This could be set to the Platform font
    fontDescription.width = IntToWidthType( DEFAULT_FONT_WIDTH );
    fontDescription.weight = IntToWeightType( DEFAULT_FONT_WEIGHT );
    fontDescription.slant = IntToSlantType( DEFAULT_FONT_SLANT );
    SetFontList( fontDescription, mDefaultFonts );
  }

  defaultFonts = mDefaultFonts;
}

void FontClient::Plugin::GetDefaultPlatformFontDescription( FontDescription& fontDescription )
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::GetDefaultPlatformFontDescription\n");

  FcInitReinitialize(); // FcInitBringUptoDate did not seem to reload config file as was still getting old default font.

  FcPattern* matchPattern = FcPatternCreate();
  FcConfigSubstitute(NULL, matchPattern, FcMatchPattern);
  FcDefaultSubstitute( matchPattern );

  MatchFontDescriptionToPattern( matchPattern, fontDescription );
  FcPatternDestroy( matchPattern );
}

void FontClient::Plugin::GetSystemFonts( FontList& systemFonts )
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::GetSystemFonts\n");

  if( mSystemFonts.empty() )
  {
    InitSystemFonts();
  }

  systemFonts = mSystemFonts;
}

void FontClient::Plugin::GetDescription( FontId id,
                                         FontDescription& fontDescription ) const
{
  for( std::vector<FontIdCacheItem>::const_iterator it = mFontIdCache.begin(),
         endIt = mFontIdCache.end();
       it != endIt;
       ++it )
  {
    const FontIdCacheItem& item = *it;

    if( item.fontId == id )
    {
      fontDescription = *( mFontDescriptionCache.begin() + item.validatedFontId );
      return;
    }
  }

  DALI_LOG_ERROR( "FontClient::Plugin::GetDescription. No description found for the font ID %d\n", id );
}

PointSize26Dot6 FontClient::Plugin::GetPointSize( FontId id )
{
  const FontId index = id - 1u;

  if( id > 0u &&
      index < mFontCache.size() )
  {
    return ( *( mFontCache.begin() + index ) ).mPointSize;
  }
  else
  {
    DALI_LOG_ERROR( "FontClient::Plugin::GetPointSize. Invalid font ID %d\n", id );
  }

  return TextAbstraction::FontClient::DEFAULT_POINT_SIZE;
}

FontId FontClient::Plugin::FindFontForCharacter( const FontList& fontList,
                                                 Character charcode,
                                                 PointSize26Dot6 requestedSize,
                                                 bool preferColor )
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::FindFontForCharacter\n");

  FontId fontId(0);
  bool foundColor(false);

  // Traverse the list of fonts.
  // Check for each default font if supports the character.

  for( FontList::const_iterator it = fontList.begin(), endIt = fontList.end();
       it != endIt;
       ++it )
  {
    const FontDescription& description = *it;

    FcPattern* pattern = CreateFontFamilyPattern( description );

    FcResult result = FcResultMatch;
    FcPattern* match = FcFontMatch( NULL /* use default configure */, pattern, &result );

    FcCharSet* charSet = NULL;
    FcPatternGetCharSet( match, FC_CHARSET, 0u, &charSet );

    if( FcCharSetHasChar( charSet, charcode ) )
    {
      Vector< PointSize26Dot6 > fixedSizes;
      GetFixedSizes( description,
                     fixedSizes );

      const Vector< PointSize26Dot6 >::SizeType count = fixedSizes.Count();
      if( 0 != count )
      {
        // If the font is not scalable, pick the largest size <= requestedSize
        PointSize26Dot6 size = fixedSizes[0];
        for( unsigned int i=1; i<count; ++i )
        {
          if( fixedSizes[i] <= requestedSize &&
              fixedSizes[i] > size )
          {
            size = fixedSizes[i];
          }
        }
        requestedSize = size;
      }

      fontId = GetFontId( description,
                          requestedSize,
                          0u );

      if( preferColor )
      {
        BufferImage bitmap = CreateBitmap( fontId, GetGlyphIndex(fontId,charcode) );
        if( bitmap &&
            Pixel::BGRA8888 == bitmap.GetPixelFormat() )
        {
          foundColor = true;
        }
      }

      // Keep going unless we prefer a different (color) font
      if( !preferColor || foundColor )
      {
        FcPatternDestroy( match );
        FcPatternDestroy( pattern );
        break;
      }
    }

    FcPatternDestroy( match );
    FcPatternDestroy( pattern );
  }

  return fontId;
}

FontId FontClient::Plugin::FindDefaultFont( Character charcode,
                                            PointSize26Dot6 requestedSize,
                                            bool preferColor )
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::FindDefaultFont DefaultFontsList(%s)\n", (mDefaultFonts.empty()?"empty":"created") );

  FontId fontId(0);

  // Create the list of default fonts if it has not been created.
  if( mDefaultFonts.empty() )
  {
    FontDescription fontDescription;
    fontDescription.family = DEFAULT_FONT_FAMILY_NAME;
    fontDescription.width = IntToWidthType( DEFAULT_FONT_WIDTH );
    fontDescription.weight = IntToWeightType( DEFAULT_FONT_WEIGHT );
    fontDescription.slant = IntToSlantType( DEFAULT_FONT_SLANT );
    SetFontList( fontDescription, mDefaultFonts );
  }

  // Traverse the list of default fonts.
  // Check for each default font if supports the character.
  fontId = FindFontForCharacter( mDefaultFonts, charcode, requestedSize, preferColor );

  return fontId;
}

FontId FontClient::Plugin::FindFallbackFont( FontId preferredFont,
                                             Character charcode,
                                             PointSize26Dot6 requestedSize,
                                             bool preferColor )
{
  // The font id to be returned.
  FontId fontId = 0u;

  FontDescription fontDescription;
  GetDescription( preferredFont, fontDescription );

  // Check first if the font's description has been queried before.
  FontList* fontList( NULL );

  if( !FindFallbackFontList( fontDescription, fontList ) )
  {
    fontList = new FontList;
    SetFontList( fontDescription, *fontList );

    // Add the font-list to the cache.
    mFallbackCache.push_back( FallbackCacheItem(fontDescription, fontList) );
  }

  if( fontList )
  {
    fontId = FindFontForCharacter( *fontList, charcode, requestedSize, preferColor );
  }

  return fontId;
}

FontId FontClient::Plugin::GetFontId( const FontPath& path,
                                      PointSize26Dot6 pointSize,
                                      FaceIndex faceIndex,
                                      bool cacheDescription )
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::GetFontId fontPatch:%s\n", path.c_str() );

  FontId id( 0 );

  if( NULL != mFreeTypeLibrary )
  {
    FontId foundId(0);
    if( FindFont( path, pointSize, faceIndex, foundId ) )
    {
      id = foundId;
    }
    else
    {
      id = CreateFont( path, pointSize, faceIndex, cacheDescription );
    }
  }

  return id;
}

FontId FontClient::Plugin::GetFontId( const FontDescription& fontDescription,
                                      PointSize26Dot6 pointSize,
                                      FaceIndex faceIndex )
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::GetFontId font family(%s)\n", fontDescription.family.c_str() );

  // This method uses three vectors which caches:
  // * Pairs of non validated font descriptions and an index to a vector with paths to font file names.
  // * The path to font file names.
  // * The font ids of pairs 'font point size, index to the vector with paths to font file names'.

  // 1) Checks in the cache if the font's description has been validated before.
  //    If it was it gets an index to the vector with paths to font file names. Otherwise,
  //    retrieves using font config a path to a font file name which matches with the
  //    font's description. The path is stored in the cache.
  //
  // 2) Checks in the cache if the pair 'font point size, index to the vector with paths to
  //    font file names' exists. If exists, it gets the font id. If it doesn't it calls
  //    the GetFontId() method with the path to the font file name and the point size to
  //    get the font id.

  // The font id to be returned.
  FontId fontId = 0u;

  // Check first if the font's description have been validated before.
  FontDescriptionId validatedFontId = 0u;

  if( !FindValidatedFont( fontDescription,
                          validatedFontId ) )
  {
    DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::GetFontId Validating Font\n");

    // Use font config to validate the font's description.
    ValidateFont( fontDescription,
                  validatedFontId );
  }

  // Check if exists a pair 'validatedFontId, pointSize' in the cache.
  if( !FindFont( validatedFontId, pointSize, fontId ) )
  {
    // Retrieve the font file name path.
    const FontDescription& description = *( mFontDescriptionCache.begin() + validatedFontId );

    // Retrieve the font id. Do not cache the description as it has been already cached.
    fontId = GetFontId( description.path,
                        pointSize,
                        faceIndex,
                        false );

    // Cache the pair 'validatedFontId, pointSize' to improve the following queries.
    mFontIdCache.push_back( FontIdCacheItem( validatedFontId,
                                             pointSize,
                                             fontId ) );
  }

  return fontId;
}

void FontClient::Plugin::ValidateFont( const FontDescription& fontDescription,
                                       FontDescriptionId& validatedFontId )
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::ValidateFont Validating Font family(%s) \n", fontDescription.family.c_str() );

  // Create a font pattern.
  FcPattern* fontFamilyPattern = CreateFontFamilyPattern( fontDescription );

  FontDescription description;

  bool matched = MatchFontDescriptionToPattern( fontFamilyPattern, description );
  FcPatternDestroy( fontFamilyPattern );

  if( matched )
  {
    // Set the index to the vector of paths to font file names.
    validatedFontId = mFontDescriptionCache.size();

    // Add the path to the cache.
    mFontDescriptionCache.push_back( description );

    // Cache the index and the font's description.
    FontDescriptionCacheItem item( description,
                                   validatedFontId );

    mValidatedFontCache.push_back( item );
  }
  else
  {
    DALI_LOG_ERROR( "FontClient::Plugin::ValidateFont failed for font %s %d %d %d\n",
                    fontDescription.family.c_str(),
                    fontDescription.width,
                    fontDescription.weight,
                    fontDescription.slant );
  }

  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::ValidateFont validatedFontId(%u) font family(%s)\n", validatedFontId, fontDescription.family.c_str() );
}

void FontClient::Plugin::GetFontMetrics( FontId fontId,
                                         FontMetrics& metrics,
                                         int maxFixedSize )
{
  if( fontId > 0 &&
      fontId-1 < mFontCache.size() )
  {
    const CacheItem& font = mFontCache[fontId-1];

    metrics = font.mMetrics;

    // Adjust the metrics if the fixed-size font should be down-scaled
    if( font.mIsFixedSizeBitmap &&
        ( maxFixedSize > 0 ) &&
        ( font.mFixedHeightPixels > maxFixedSize ) )
    {
      float scaleFactor = static_cast<float>(maxFixedSize) / static_cast<float>(font.mFixedHeightPixels);

      metrics.ascender           *= scaleFactor;
      metrics.descender          *= scaleFactor;
      metrics.height             *= scaleFactor;
      metrics.underlinePosition  *= scaleFactor;
      metrics.underlineThickness *= scaleFactor;
    }
  }
  else
  {
    DALI_LOG_ERROR( "Invalid font ID %d\n", fontId );
  }
}

GlyphIndex FontClient::Plugin::GetGlyphIndex( FontId fontId,
                                              Character charcode )
{
  GlyphIndex index( 0 );

  if( fontId > 0 &&
      fontId-1 < mFontCache.size() )
  {
    FT_Face ftFace = mFontCache[fontId-1].mFreeTypeFace;

    index = FT_Get_Char_Index( ftFace, charcode );
  }

  return index;
}

bool FontClient::Plugin::GetGlyphMetrics( GlyphInfo* array,
                                          uint32_t size,
                                          bool horizontal,
                                          int maxFixedSize )
{
  bool success( true );

  for( unsigned int i=0; i<size; ++i )
  {
    FontId fontId = array[i].fontId;

    if( fontId > 0 &&
        fontId-1 < mFontCache.size() )
    {
      const CacheItem& font = mFontCache[fontId-1];

      FT_Face ftFace = font.mFreeTypeFace;

#ifdef FREETYPE_BITMAP_SUPPORT
      // Check to see if we should be loading a Fixed Size bitmap?
      if ( font.mIsFixedSizeBitmap )
      {
        int error = FT_Load_Glyph( ftFace, array[i].index, FT_LOAD_COLOR );
        if ( FT_Err_Ok == error )
        {
          array[i].width = font.mFixedWidthPixels;
          array[i].height = font.mFixedHeightPixels;
          array[i].advance = font.mFixedWidthPixels;
          array[i].xBearing = 0.0f;
          array[i].yBearing = font.mFixedHeightPixels;

          // Adjust the metrics if the fixed-size font should be down-scaled
          if( ( maxFixedSize > 0 ) &&
              ( font.mFixedHeightPixels > maxFixedSize ) )
          {
            float scaleFactor = static_cast<float>(maxFixedSize) / static_cast<float>(font.mFixedHeightPixels);

            array[i].width    *= scaleFactor;
            array[i].height   *= scaleFactor;
            array[i].advance  *= scaleFactor;
            array[i].xBearing *= scaleFactor;
            array[i].yBearing *= scaleFactor;

            array[i].scaleFactor = scaleFactor;
          }
        }
        else
        {
          DALI_LOG_ERROR( "FreeType Bitmap Load_Glyph error %d\n", error );
          success = false;
        }
      }
      else
#endif
      {
        int error = FT_Load_Glyph( ftFace, array[i].index, FT_LOAD_DEFAULT );

        if( FT_Err_Ok == error )
        {
          array[i].width  = static_cast< float >( ftFace->glyph->metrics.width ) * FROM_266;
          array[i].height = static_cast< float >( ftFace->glyph->metrics.height ) * FROM_266 ;
          if( horizontal )
          {
            array[i].xBearing += static_cast< float >( ftFace->glyph->metrics.horiBearingX ) * FROM_266;
            array[i].yBearing += static_cast< float >( ftFace->glyph->metrics.horiBearingY ) * FROM_266;
          }
          else
          {
            array[i].xBearing += static_cast< float >( ftFace->glyph->metrics.vertBearingX ) * FROM_266;
            array[i].yBearing += static_cast< float >( ftFace->glyph->metrics.vertBearingY ) * FROM_266;
          }
        }
        else
        {
          success = false;
        }
      }
    }
    else
    {
      success = false;
    }
  }

  return success;
}

BufferImage FontClient::Plugin::CreateBitmap( FontId fontId,
                                              GlyphIndex glyphIndex )
{
  BufferImage bitmap;

  if( fontId > 0 &&
      fontId-1 < mFontCache.size() )
  {
    FT_Face ftFace = mFontCache[fontId-1].mFreeTypeFace;

    FT_Error error;

#ifdef FREETYPE_BITMAP_SUPPORT
    // Check to see if this is fixed size bitmap
    if ( mFontCache[fontId-1].mIsFixedSizeBitmap )
    {
      error = FT_Load_Glyph( ftFace, glyphIndex, FT_LOAD_COLOR );
    }
    else
#endif
    {
      error = FT_Load_Glyph( ftFace, glyphIndex, FT_LOAD_DEFAULT );
    }
    if( FT_Err_Ok == error )
    {
      FT_Glyph glyph;
      error = FT_Get_Glyph( ftFace->glyph, &glyph );

      // Convert to bitmap if necessary
      if ( FT_Err_Ok == error )
      {
        if( glyph->format != FT_GLYPH_FORMAT_BITMAP )
        {
          error = FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 );
          if ( FT_Err_Ok == error )
          {
            FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
            ConvertBitmap( bitmap, bitmapGlyph->bitmap );
          }
          else
          {
            DALI_LOG_ERROR( "FT_Get_Glyph Failed with error: %d\n", error );
          }
        }
        else
        {
          ConvertBitmap( bitmap, ftFace->glyph->bitmap );
        }

        // Created FT_Glyph object must be released with FT_Done_Glyph
        FT_Done_Glyph( glyph );
      }
    }
    else
    {
      DALI_LOG_ERROR( "FT_Load_Glyph Failed with error: %d\n", error );
    }
  }

  return bitmap;
}

const GlyphInfo& FontClient::Plugin::GetEllipsisGlyph( PointSize26Dot6 pointSize )
{
  // First look into the cache if there is an ellipsis glyph for the requested point size.
  for( Vector<EllipsisItem>::ConstIterator it = mEllipsisCache.Begin(),
         endIt = mEllipsisCache.End();
       it != endIt;
       ++it )
  {
    const EllipsisItem& item = *it;

    if( fabsf( item.size - pointSize ) < Math::MACHINE_EPSILON_1000 )
    {
      // Use the glyph in the cache.
      return item.glyph;
    }
  }

  // No glyph has been found. Create one.
  mEllipsisCache.PushBack( EllipsisItem() );
  EllipsisItem& item = *( mEllipsisCache.End() - 1u );

  item.size = pointSize;

  // Find a font for the ellipsis glyph.
  item.glyph.fontId = FindDefaultFont( ELLIPSIS_CHARACTER,
                                       pointSize,
                                       false );

  // Set the character index to access the glyph inside the font.
  item.glyph.index = FT_Get_Char_Index( mFontCache[item.glyph.fontId-1].mFreeTypeFace,
                                        ELLIPSIS_CHARACTER );

  GetGlyphMetrics( &item.glyph, 1u, true, 0 );

  return item.glyph;
}

void FontClient::Plugin::InitSystemFonts()
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::InitSystemFonts \n");

  FcFontSet* fontSet = GetFcFontSet();

  if( fontSet )
  {
    // Reserve some space to avoid reallocations.
    mSystemFonts.reserve( fontSet->nfont );

    for( int i = 0u; i < fontSet->nfont; ++i )
    {
      FcPattern* fontPattern = fontSet->fonts[i];

      FontPath path;

      // Skip fonts with no path
      if( GetFcString( fontPattern, FC_FILE, path ) )
      {
        mSystemFonts.push_back( FontDescription() );
        FontDescription& fontDescription = mSystemFonts.back();

        fontDescription.path = path;

        int width = 0;
        int weight = 0;
        int slant = 0;
        GetFcString( fontPattern, FC_FAMILY, fontDescription.family );
        GetFcInt( fontPattern, FC_WIDTH, width );
        GetFcInt( fontPattern, FC_WEIGHT, weight );
        GetFcInt( fontPattern, FC_SLANT, slant );
        fontDescription.width = IntToWidthType( width );
        fontDescription.weight = IntToWeightType( weight );
        fontDescription.slant = IntToSlantType( slant );
        DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::InitSystemFonts font family(%s)\n", fontDescription.family.c_str() );

      }
    }

    FcFontSetDestroy( fontSet );
  }
}

bool FontClient::Plugin::MatchFontDescriptionToPattern( FcPattern* pattern, Dali::TextAbstraction::FontDescription& fontDescription )
{
  FcResult result = FcResultMatch;
  FcPattern* match = FcFontMatch( NULL /* use default configure */, pattern, &result );

  bool ret = false;

  if( match )
  {
    int width = 0;
    int weight = 0;
    int slant = 0;
    GetFcString( match, FC_FILE, fontDescription.path );
    GetFcString( match, FC_FAMILY, fontDescription.family );
    DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::MatchFontDescriptionToPattern matched:%s \n", fontDescription.family.c_str());
    GetFcInt( match, FC_WIDTH, width );
    GetFcInt( match, FC_WEIGHT, weight );
    GetFcInt( match, FC_SLANT, slant );
    fontDescription.width = IntToWidthType( width );
    fontDescription.weight = IntToWeightType( weight );
    fontDescription.slant = IntToSlantType( slant );
    // destroyed the matched pattern
    FcPatternDestroy( match );
    ret = true;
  }
  return ret;
}


FcPattern* FontClient::Plugin::CreateFontFamilyPattern( const FontDescription& fontDescription )
{
  // create the cached font family lookup pattern
  // a pattern holds a set of names, each name refers to a property of the font
  FcPattern* fontFamilyPattern = FcPatternCreate();

  // add a property to the pattern for the font family
  FcPatternAddString( fontFamilyPattern, FC_FAMILY, reinterpret_cast<const FcChar8*>( fontDescription.family.c_str() ) );

  FcPatternAddInteger( fontFamilyPattern, FC_WIDTH, FONT_WIDTH_TYPE_TO_INT[fontDescription.width] );
  FcPatternAddInteger( fontFamilyPattern, FC_WEIGHT, FONT_WEIGHT_TYPE_TO_INT[fontDescription.weight] );
  FcPatternAddInteger( fontFamilyPattern, FC_SLANT, FONT_SLANT_TYPE_TO_INT[fontDescription.slant] );

  // Add a property of the pattern, to say we want to match TrueType fonts
  FcPatternAddString( fontFamilyPattern , FC_FONTFORMAT, reinterpret_cast<const FcChar8*>( FONT_FORMAT.c_str() ) );

  // modify the config, with the mFontFamilyPatterm
  FcConfigSubstitute( NULL /* use default configure */, fontFamilyPattern, FcMatchPattern );

  // provide default values for unspecified properties in the font pattern
  // e.g. patterns without a specified style or weight are set to Medium
  FcDefaultSubstitute( fontFamilyPattern );

  return fontFamilyPattern;
}

_FcFontSet* FontClient::Plugin::GetFcFontSet() const
{
  // create a new pattern.
  // a pattern holds a set of names, each name refers to a property of the font
  FcPattern* pattern = FcPatternCreate();

  // create an object set used to define which properties are to be returned in the patterns from FcFontList.
  FcObjectSet* objectSet = FcObjectSetCreate();

  // build an object set from a list of property names
  FcObjectSetAdd( objectSet, FC_FILE );
  FcObjectSetAdd( objectSet, FC_FAMILY );
  FcObjectSetAdd( objectSet, FC_WIDTH );
  FcObjectSetAdd( objectSet, FC_WEIGHT );
  FcObjectSetAdd( objectSet, FC_SLANT );

  // get a list of fonts
  // creates patterns from those fonts containing only the objects in objectSet and returns the set of unique such patterns
  FcFontSet* fontset = FcFontList( NULL /* the default configuration is checked to be up to date, and used */, pattern, objectSet );

  // clear up the object set
  if( objectSet )
  {
    FcObjectSetDestroy( objectSet );
  }
  // clear up the pattern
  if( pattern )
  {
    FcPatternDestroy( pattern );
  }

  return fontset;
}

bool FontClient::Plugin::GetFcString( const FcPattern* const pattern,
                                      const char* const n,
                                      std::string& string )
{
  FcChar8* file = NULL;
  const FcResult retVal = FcPatternGetString( pattern, n, 0u, &file );

  if( FcResultMatch == retVal )
  {
    // Have to use reinterpret_cast because FcChar8 is unsigned char*, not a const char*.
    string.assign( reinterpret_cast<const char*>( file ) );

    return true;
  }

  return false;
}

bool FontClient::Plugin::GetFcInt( const _FcPattern* const pattern, const char* const n, int& intVal )
{
  const FcResult retVal = FcPatternGetInteger( pattern, n, 0u, &intVal );

  if( FcResultMatch == retVal )
  {
    return true;
  }

  return false;
}

FontId FontClient::Plugin::CreateFont( const FontPath& path,
                                       PointSize26Dot6 pointSize,
                                       FaceIndex faceIndex,
                                       bool cacheDescription )
{
  FontId id( 0 );

  // Create & cache new font face
  FT_Face ftFace;
  int error = FT_New_Face( mFreeTypeLibrary,
                           path.c_str(),
                           0,
                           &ftFace );

  if( FT_Err_Ok == error )
  {
    // Check to see if the font contains fixed sizes?
    if ( ftFace->num_fixed_sizes && ftFace->available_sizes )
    {
      // Ensure this size is available
      for ( int i = 0; i < ftFace->num_fixed_sizes; ++i )
      {
        if ( static_cast<FT_Pos>(pointSize) == ftFace->available_sizes[ i ].size )
        {
          // Tell Freetype to use this size
          error = FT_Select_Size( ftFace, i );
          if ( FT_Err_Ok != error )
          {
            DALI_LOG_ERROR( "FreeType Select_Size error: %d\n", error );
          }
          else
          {
            float fixedWidth  = static_cast< float >( ftFace->available_sizes[ i ].width );
            float fixedHeight = static_cast< float >( ftFace->available_sizes[ i ].height );

            // Indicate that the font is a fixed sized bitmap
            FontMetrics metrics( fixedHeight, // The ascender in pixels.
                                 0.0f,
                                 fixedHeight, // The height in pixels.
                                 0.0f,
                                 0.0f );

            mFontCache.push_back( CacheItem( ftFace, path, pointSize, faceIndex, metrics, fixedWidth, fixedHeight ) );
            id = mFontCache.size();

            if( cacheDescription )
            {
              FontDescription description;
              description.path = path;
              description.family = FontFamily( ftFace->family_name );

              // Note FreeType doesn't give too much info to build a proper font style.
              if( ftFace->style_flags & FT_STYLE_FLAG_ITALIC )
              {
                description.slant = FontSlant::ITALIC;
              }
              if( ftFace->style_flags & FT_STYLE_FLAG_BOLD )
              {
                description.weight = FontWeight::BOLD;
              }

              mFontDescriptionCache.push_back( description );
            }
            return id;
          }
        }
      }

      // Can't find this size
      std::stringstream sizes;
      for ( int i = 0; i < ftFace->num_fixed_sizes; ++i )
      {
        if ( i )
        {
          sizes << ", ";
        }
        sizes << ftFace->available_sizes[ i ].size;
      }
      DALI_LOG_ERROR( "FreeType Font: %s, does not contain Bitmaps of size: %d. Available sizes are: %s\n",
                       path.c_str(), pointSize, sizes.str().c_str() );
    }
    else
    {
      error = FT_Set_Char_Size( ftFace,
                              0,
                              pointSize,
                              mDpiHorizontal,
                              mDpiVertical );

      if( FT_Err_Ok == error )
      {

        FT_Size_Metrics& ftMetrics = ftFace->size->metrics;

        FontMetrics metrics( static_cast< float >( ftMetrics.ascender  ) * FROM_266,
                             static_cast< float >( ftMetrics.descender ) * FROM_266,
                             static_cast< float >( ftMetrics.height    ) * FROM_266,
                             static_cast< float >( ftFace->underline_position ) * FROM_266,
                             static_cast< float >( ftFace->underline_thickness ) * FROM_266 );

        mFontCache.push_back( CacheItem( ftFace, path, pointSize, faceIndex, metrics ) );
        id = mFontCache.size();

        if( cacheDescription )
        {
          FontDescription description;
          description.path = path;
          description.family = FontFamily( ftFace->family_name );

          // Note FreeType doesn't give too much info to build a proper font style.
          if( ftFace->style_flags & FT_STYLE_FLAG_ITALIC )
          {
            description.slant = FontSlant::ITALIC;
          }
          if( ftFace->style_flags & FT_STYLE_FLAG_BOLD )
          {
            description.weight = FontWeight::BOLD;
          }

          mFontDescriptionCache.push_back( description );
        }
      }
      else
      {
        DALI_LOG_ERROR( "FreeType Set_Char_Size error: %d for pointSize %d\n", error, pointSize );
      }
    }
  }
  else
  {
    DALI_LOG_ERROR( "FreeType New_Face error: %d for %s\n", error, path.c_str() );
  }

  return id;
}

void FontClient::Plugin::ConvertBitmap( BufferImage& destBitmap,
                                        FT_Bitmap srcBitmap )
{
  if( srcBitmap.width*srcBitmap.rows > 0 )
  {
    switch( srcBitmap.pixel_mode )
    {
      case FT_PIXEL_MODE_GRAY:
      {
        if( srcBitmap.pitch == static_cast< int >( srcBitmap.width ) )
        {
          destBitmap = BufferImage::New( srcBitmap.width, srcBitmap.rows, Pixel::L8 );

          PixelBuffer* destBuffer = destBitmap.GetBuffer();
          if( destBuffer )
          {
            memcpy( destBuffer, srcBitmap.buffer, srcBitmap.width*srcBitmap.rows );
          }
          else
          {
            DALI_LOG_ERROR( "GetBuffer returns null\n" );
          }
        }
        break;
      }

#ifdef FREETYPE_BITMAP_SUPPORT
      case FT_PIXEL_MODE_BGRA:
      {
        if ( srcBitmap.pitch == static_cast< int >( srcBitmap.width << 2 ) )
        {
          destBitmap = BufferImage::New( srcBitmap.width, srcBitmap.rows, Pixel::BGRA8888 );

          PixelBuffer* destBuffer = destBitmap.GetBuffer();
          if( destBuffer )
          {
            memcpy( destBuffer, srcBitmap.buffer, srcBitmap.width*srcBitmap.rows*4 );
          }
          else
          {
            DALI_LOG_ERROR( "GetBuffer returns null\n" );
          }
        }
        break;
      }
#endif
      default:
      {
        DALI_LOG_ERROR( "FontClient Unable to create Bitmap of this PixelType\n" );
        break;
      }
    }
  }
}

bool FontClient::Plugin::FindFont( const FontPath& path,
                                   PointSize26Dot6 pointSize,
                                   FaceIndex faceIndex,
                                   FontId& fontId ) const
{
  fontId = 0u;
  for( std::vector<CacheItem>::const_iterator it = mFontCache.begin(),
         endIt = mFontCache.end();
       it != endIt;
       ++it, ++fontId )
  {
    const CacheItem& cacheItem = *it;

    if( cacheItem.mPointSize == pointSize &&
        cacheItem.mFaceIndex == faceIndex &&
        cacheItem.mPath == path )
    {
      ++fontId;
      return true;
    }
  }

  return false;
}

bool FontClient::Plugin::FindValidatedFont( const FontDescription& fontDescription,
                                            FontDescriptionId& validatedFontId )
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::FindValidatedFont fontDescription family(%s)\n", fontDescription.family.c_str() );

  validatedFontId = 0u;

  for( std::vector<FontDescriptionCacheItem>::const_iterator it = mValidatedFontCache.begin(),
         endIt = mValidatedFontCache.end();
       it != endIt;
       ++it )
  {
    const FontDescriptionCacheItem& item = *it;

    if( !fontDescription.family.empty() &&
        ( fontDescription.family == item.fontDescription.family ) &&
        ( fontDescription.width == item.fontDescription.width ) &&
        ( fontDescription.weight == item.fontDescription.weight ) &&
        ( fontDescription.slant == item.fontDescription.slant ) )
    {
      validatedFontId = item.index;

      DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::FindValidatedFont validated font family(%s) font id (%u) \n", fontDescription.family.c_str(), validatedFontId );

      return true;
    }
  }

  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::FindValidatedFont NOT VALIDATED return false\n" );

  return false;
}

bool FontClient::Plugin::FindFallbackFontList( const FontDescription& fontDescription,
                                               FontList*& fontList )
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::FindFallbackFontList fontDescription family(%s)\n", fontDescription.family.c_str() );

  fontList = NULL;

  for( std::vector<FallbackCacheItem>::const_iterator it = mFallbackCache.begin(), endIt = mFallbackCache.end();
       it != endIt;
       ++it )
  {
    const FallbackCacheItem& item = *it;

    if( !fontDescription.family.empty() &&
        ( fontDescription.family == item.fontDescription.family ) &&
        ( fontDescription.width == item.fontDescription.width ) &&
        ( fontDescription.weight == item.fontDescription.weight ) &&
        ( fontDescription.slant == item.fontDescription.slant ) )
    {
      fontList = item.fallbackFonts;

      DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::FindFallbackFontList font family(%s) font-list (%p) \n", fontDescription.family.c_str(), fontList );

      return true;
    }
  }

  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::FindFallbackFontList NOT FOUND return false\n" );

  return false;
}

bool FontClient::Plugin::FindFont( FontDescriptionId validatedFontId,
                                   PointSize26Dot6 pointSize,
                                   FontId& fontId )
{
  fontId = 0u;

  for( std::vector<FontIdCacheItem>::const_iterator it = mFontIdCache.begin(),
         endIt = mFontIdCache.end();
       it != endIt;
       ++it )
  {
    const FontIdCacheItem& item = *it;

    if( ( validatedFontId == item.validatedFontId ) &&
        ( pointSize == item.pointSize ) )
    {
      fontId = item.fontId;
      return true;
    }
  }

  return false;
}

bool FontClient::Plugin::IsScalable( const FontPath& path )
{
  FT_Face ftFace;
  int error = FT_New_Face( mFreeTypeLibrary,
                           path.c_str(),
                           0,
                           &ftFace );
  if( FT_Err_Ok != error )
  {
    DALI_LOG_ERROR( "FreeType Cannot check font: %s\n", path.c_str() );
  }
  return ( ftFace->num_fixed_sizes == 0 );
}

bool FontClient::Plugin::IsScalable( const FontDescription& fontDescription )
{
  // Create a font pattern.
  FcPattern* fontFamilyPattern = CreateFontFamilyPattern( fontDescription );

  FcResult result = FcResultMatch;

  // match the pattern
  FcPattern* match = FcFontMatch( NULL /* use default configure */, fontFamilyPattern, &result );
  bool isScalable = true;

  if( match )
  {
    // Get the path to the font file name.
    FontPath path;
    GetFcString( match, FC_FILE, path );
    isScalable = IsScalable( path );
  }
  else
  {
    DALI_LOG_ERROR( "FreeType Cannot check font: %s %d %d %d\n",
                    fontDescription.family.c_str(),
                    fontDescription.width,
                    fontDescription.weight,
                    fontDescription.slant );
  }
  FcPatternDestroy( fontFamilyPattern );
  FcPatternDestroy( match );
  return isScalable;
}

void FontClient::Plugin::GetFixedSizes( const FontPath& path, Vector< PointSize26Dot6 >& sizes )
{
  // Empty the caller container
  sizes.Clear();

  FT_Face ftFace;
  int error = FT_New_Face( mFreeTypeLibrary,
                           path.c_str(),
                           0,
                           &ftFace );
  if( FT_Err_Ok != error )
  {
    DALI_LOG_ERROR( "FreeType Cannot check font: %s\n", path.c_str() );
  }

  // Fetch the number of fixed sizes available
  if ( ftFace->num_fixed_sizes && ftFace->available_sizes )
  {
    for ( int i = 0; i < ftFace->num_fixed_sizes; ++i )
    {
      sizes.PushBack( ftFace->available_sizes[ i ].size );
    }
  }
}

void FontClient::Plugin::GetFixedSizes( const FontDescription& fontDescription,
                                        Vector< PointSize26Dot6 >& sizes )
{
  // Create a font pattern.
  FcPattern* fontFamilyPattern = CreateFontFamilyPattern( fontDescription );

  FcResult result = FcResultMatch;

  // match the pattern
  FcPattern* match = FcFontMatch( NULL /* use default configure */, fontFamilyPattern, &result );

  if( match )
  {
    // Get the path to the font file name.
    FontPath path;
    GetFcString( match, FC_FILE, path );
    GetFixedSizes( path, sizes );
  }
  else
  {
    DALI_LOG_ERROR( "FreeType Cannot check font: %s %d %d %d\n",
                    fontDescription.family.c_str(),
                    fontDescription.width,
                    fontDescription.weight,
                    fontDescription.slant );
  }
  FcPatternDestroy( match );
  FcPatternDestroy( fontFamilyPattern );
}

} // namespace Internal

} // namespace TextAbstraction

} // namespace Dali
