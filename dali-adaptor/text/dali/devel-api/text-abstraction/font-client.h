#ifndef __DALI_PLATFORM_TEXT_ABSTRACTION_FONT_CLIENT_H__
#define __DALI_PLATFORM_TEXT_ABSTRACTION_FONT_CLIENT_H__

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
#include <dali/public-api/common/dali-vector.h>
#include <dali/public-api/images/buffer-image.h>
#include <dali/public-api/object/base-handle.h>
#include <dali/devel-api/text-abstraction/font-list.h>
#include <dali/devel-api/text-abstraction/text-abstraction-definitions.h>

namespace Dali
{

namespace TextAbstraction
{

struct FontMetrics;
struct GlyphInfo;

namespace Internal DALI_INTERNAL
{
class FontClient;
}

/**
 * @brief FontClient provides access to font information and resources.
 *
 * <h3>Querying the System Fonts</h3>
 *
 * A "system font" is described by a "path" to a font file on the native filesystem, along with a "family" and "style".
 * For example on the Ubuntu system a "Regular" style font from the "Ubuntu Mono" family can be accessed from "/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf".
 *
 * <h3>Accessing Fonts</h3>
 *
 * A "font" is created from the system for a specific point size in 26.6 fractional points. A "FontId" is used to identify each font.
 * For example two different fonts with point sizes 10 & 12 can be created from the "Ubuntu Mono" family:
 * @code
 * FontClient fontClient   = FontClient::Get();
 * FontId ubuntuMonoTen    = fontClient.GetFontId( "/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf", 10*64 );
 * FontId ubuntuMonoTwelve = fontClient.GetFontId( "/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf", 12*64 );
 * @endcode
 * Glyph metrics and bitmap resources can then be retrieved using the FontId.
 */
class DALI_IMPORT_API FontClient : public BaseHandle
{
public:
  static const PointSize26Dot6 DEFAULT_POINT_SIZE; ///< The default point size.

public:

  /**
   * @brief Retrieve a handle to the FontClient instance.
   *
   * @return A handle to the FontClient
   */
  static FontClient Get();

  /**
   * @brief Create an uninitialized TextAbstraction handle.
   */
  FontClient();

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~FontClient();

  /**
   * @brief This copy constructor is required for (smart) pointer semantics.
   *
   * @param[in] handle A reference to the copied handle.
   */
  FontClient( const FontClient& handle );

  /**
   * @brief This assignment operator is required for (smart) pointer semantics.
   *
   * @param [in] handle  A reference to the copied handle.
   * @return A reference to this.
   */
  FontClient& operator=( const FontClient& handle );

  ////////////////////////////////////////
  // Font management and validation.
  ////////////////////////////////////////

  /**
   * @brief Set the DPI of the target window.
   *
   * @note Multiple windows are not currently supported.
   * @param[in] horizontalDpi The horizontal resolution in DPI.
   * @param[in] verticalDpi The vertical resolution in DPI.
   */
  void SetDpi( unsigned int horizontalDpi, unsigned int verticalDpi );

  /**
   * @brief Retrieves the DPI previously set to the target window.
   *
   * @note Multiple windows are not currently supported.
   * @param[out] horizontalDpi The horizontal resolution in DPI.
   * @param[out] verticalDpi The vertical resolution in DPI.
   */
  void GetDpi( unsigned int& horizontalDpi, unsigned int& verticalDpi );

  /**
   * Set the default font family and its style that should be used by the font client.
   * The style could be a pair 'font style, font width' or a cluster 'font width, font weight, font slant'.
   *
   * @param[in] fontDescription Description of the default font.
   */
  void SetDefaultFont( const FontDescription& fontDescription );

  /**
   * @brief Retrieve the list of default fonts supported by the system.
   *
   * @param[out] defaultFonts A list of default font paths, family & style strings.
   */
  void GetDefaultFonts( FontList& defaultFonts );

  /**
   * @brief Retrieve the active default font from the system
   *
   * @param[out] fontDescription font structure describing the default font
   */
  void GetDefaultPlatformFontDescription( FontDescription& fontDescription );

  /**
   * @brief Retrieve the list of fonts supported by the system.
   *
   * @param[out] systemFonts A list of font paths, family & style strings.
   */
  void GetSystemFonts( FontList& systemFonts );

  /**
   * @brief Retrieves the font description of a given font @p id.
   *
   * @param[in] id The font id.
   * @param[out] fontDescription The path, family & style describing the font.
   */
  void GetDescription( FontId id, FontDescription& fontDescription );

  /**
   * @brief Retrieves the font point size of a given font @p id.
   *
   * @param[in] id The font id.
   *
   * @return The point size in 26.6 fractional points.
   */
  PointSize26Dot6 GetPointSize( FontId id );

  /**
   * @brief Find the default font for displaying a UTF-32 character.
   *
   * This is useful when localised strings are provided for multiple languages
   * i.e. when a single default font does not work for all languages.
   * @param[in] charcode The character for which a font is needed.
   * @param[in] pointSize The point size in 26.6 fractional points; the default point size is 12*64.
   * @param[in] preferColor True if a color font is preferred.
   * @return A valid font ID, or zero if the font does not exist.
   */
  FontId FindDefaultFont( Character charcode,
                          PointSize26Dot6 pointSize = DEFAULT_POINT_SIZE,
                          bool preferColor = false );

  /**
   * @brief Find a fallback-font for displaying a UTF-32 character.
   *
   * This is useful when localised strings are provided for multiple languages
   * i.e. when a single default font does not work for all languages.
   * @param[in] preferredFont The preferred font which may not provide a glyph for charcode.
   * The fallback-font will be the closest match to preferredFont, which does support the required glyph.
   * @param[in] charcode The character for which a font is needed.
   * @param[in] pointSize The point size in 26.6 fractional points; the default point size is 12*64.
   * @param[in] preferColor True if a color font is preferred.
   * @return A valid font ID, or zero if the font does not exist.
   */
  FontId FindFallbackFont( FontId preferredFont,
                           Character charcode,
                           PointSize26Dot6 pointSize = DEFAULT_POINT_SIZE,
                           bool preferColor = false );

  /**
   * @brief Retrieve the unique identifier for a font.
   *
   * @param[in] path The path to a font file.
   * @param[in] pointSize The point size in 26.6 fractional points; the default point size is 12*64.
   * @param[in] faceIndex The index of the font face (optional).
   * @return A valid font ID, or zero if the font does not exist.
   */
  FontId GetFontId( const FontPath& path,
                    PointSize26Dot6 pointSize = DEFAULT_POINT_SIZE,
                    FaceIndex faceIndex = 0 );

  /**
   * @brief Retrieve the unique identifier for a font.
   *
   * @note It the font style is not empty, it will be used instead the font weight and font slant slant.
   *
   * @param[in] fontDescription A font description.
   * @param[in] pointSize The point size in 26.6 fractional points; the default point size is 12*64.
   * @param[in] faceIndex The index of the font face (optional).
   * @return A valid font ID, or zero if the font does not exist.
   */
  FontId GetFontId( const FontDescription& fontDescription,
                    PointSize26Dot6 pointSize = DEFAULT_POINT_SIZE,
                    FaceIndex faceIndex = 0 );

  /**
   * @brief Check to see if a font is scalable.
   *
   * @param[in] path The path to a font file.
   * @return true if scalable.
   */
  bool IsScalable( const FontPath& path );

  /**
   * @brief Check to see if a font is scalable.
   *
   * @note It the font style is not empty, it will be used instead the font weight and font slant slant.
   *
   * @param[in] fontDescription A font description.
   *
   * @return true if scalable
   */
  bool IsScalable( const FontDescription& fontDescription );

  /**
   * @brief Get a list of sizes available for a fixed size font.
   *
   * @param[in] path The path to a font file.
   * @param[out] sizes A list of the available sizes, if no sizes available will return empty.
   */
  void GetFixedSizes( const FontPath& path, Dali::Vector< PointSize26Dot6>& sizes );

  /**
   * @brief Get a list of sizes available for a fixed size font.
   *
   * @note It the font style is not empty, it will be used instead the font weight and font slant slant.
   *
   * @param[in] fontDescription A font description.
   * @param[out] sizes A list of the available sizes, if no sizes available will return empty.
   */
  void GetFixedSizes( const FontDescription& fontDescription,
                      Dali::Vector< PointSize26Dot6 >& sizes );

  ////////////////////////////////////////
  // Font metrics, glyphs and bitmaps.
  ////////////////////////////////////////

  /**
   * @brief Query the metrics for a font.
   *
   * @param[in] fontId The ID of the font for the required glyph.
   * @param[out] metrics The font metrics.
   * @param[in] maxFixedSize The metrics for fixed-size fonts will be down-scaled, when exceeding this maximum value in pixels.
   */
  void GetFontMetrics( FontId fontId, FontMetrics& metrics, int maxFixedSize = 0 );

  /**
   * @brief Retrieve the glyph index for a UTF-32 character code.
   *
   * @param[in] fontId The ID of the font for the required glyph.
   * @param[in] charcode The UTF-32 character code.
   * @return The glyph index, or zero if the character code is undefined.
   */
  GlyphIndex GetGlyphIndex( FontId fontId, Character charcode );

  /**
   * @brief Retrieve the metrics for a series of glyphs.
   *
   * @param[in,out] array An array of glyph-info structures with initialized FontId & GlyphIndex values.
   * It may contain the advance and an offset set into the bearing from the shaping tool.
   * On return, the glyph's size value will be initialized. The bearing value will be updated by adding the font's glyph bearing to the one set by the shaping tool.
   * @param[in] size The size of the array.
   * @param[in] horizontal True for horizontal layouts (set to false for vertical layouting).
   * @param[in] maxFixedSize The metrics for fixed-size fonts will be down-scaled, when exceeding this maximum value in pixels.
   * @return True if all of the requested metrics were found.
   */
  bool GetGlyphMetrics( GlyphInfo* array, uint32_t size, bool horizontal = true, int maxFixedSize = 0 );

  /**
   * @brief Render a bitmap representation of a glyph.
   *
   * @param[in] fontId The ID of the font.
   * @param[in] glyphIndex The index of a glyph within the specified font.
   * @return A valid BufferImage, or an empty handle if the glyph could not be rendered.
   */
  BufferImage CreateBitmap( FontId fontId, GlyphIndex glyphIndex );

  /**
   * @brief Retrieves the ellipsis glyph for a requested point size.
   *
   * @param[in] pointSize The requested point size.
   *
   * @return The ellipsis glyph.
   */
  const GlyphInfo& GetEllipsisGlyph( PointSize26Dot6 pointSize );

public: // Not intended for application developers
  /**
   * @brief This constructor is used by FontClient::Get().
   *
   * @param[in] fontClient  A pointer to the internal fontClient object.
   */
  explicit DALI_INTERNAL FontClient( Internal::FontClient* fontClient );
};

} // namespace TextAbstraction

} // namespace Dali

#endif // __DALI_PLATFORM_TEXT_ABSTRACTION_FONT_CLIENT_H__
