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
 */

// CLASS HEADER
#include "bitmap-saver.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <dali/public-api/common/dali-vector.h>

// INTERNAL INCLUDES
#include <platform-abstractions/tizen/resource-loader/resource-loader.h>
#include <platform-abstractions/tizen/image-loaders/loader-jpeg.h>
#include <platform-abstractions/tizen/image-loaders/loader-png.h>
#include <image-encoder.h>

namespace Dali
{

// Pieces needed to save compressed images (temporary location while plumbing):
namespace
{

/**
 * Simple function to tell intended image file format from filename
 */
FileFormat GetFormatFromFileName( const std::string& filename )
{
  if (filename.length() < 5)
  {
    DALI_LOG_WARNING("Invalid (short) filename.");
  }
  FileFormat format(INVALID_FORMAT);

  const std::size_t filenameSize = filename.length();

  if(filenameSize >= 4){ // Avoid throwing out_of_range or failing silently if exceptions are turned-off on the compare(). (http://www.cplusplus.com/reference/string/string/compare/)
    if( !filename.compare( filenameSize - 4, 4, ".jpg" )
        || !filename.compare( filenameSize - 4, 4, ".JPG" ) )
    {
      format = JPG_FORMAT;
    }
    else if( !filename.compare( filenameSize - 4, 4, ".png" )
             || !filename.compare( filenameSize - 4, 4, ".PNG" ) )
    {
      format = PNG_FORMAT;
    }
    else if( !filename.compare( filenameSize - 4, 4, ".bmp" )
             || !filename.compare( filenameSize - 4, 4, ".BMP" ) )
    {
      format = BMP_FORMAT;
    }
    else if( !filename.compare( filenameSize - 4, 4, ".gif" )
             || !filename.compare( filenameSize - 4, 4, ".GIF" ) )
    {
      format = GIF_FORMAT;
    }
    else if( !filename.compare( filenameSize - 4, 4, ".ico" )
             || !filename.compare( filenameSize - 4, 4, ".ICO" ) )
    {
      format = ICO_FORMAT;
    }
    else if(filenameSize >= 5){
      if( !filename.compare( filenameSize - 5, 5, ".jpeg" )
          || !filename.compare( filenameSize - 5, 5, ".JPEG" ) )
      {
        format = JPG_FORMAT;
      }
    }
  }

  return format;
}

bool EncodeToFormat( const unsigned char* pixelBuffer,
                     Vector< unsigned char >& encodedPixels,
                     FileFormat formatEncoding,
                     std::size_t width,
                     std::size_t height,
                     Pixel::Format pixelFormat )
{
  switch( formatEncoding )
  {
    case JPG_FORMAT:
    {
      return TizenPlatform::EncodeToJpeg( pixelBuffer, encodedPixels, width, height, pixelFormat );
      break;
    }
    case PNG_FORMAT:
    {
      return TizenPlatform::EncodeToPng( pixelBuffer, encodedPixels, width, height, pixelFormat );
      break;
    }
    default:
    {
      DALI_LOG_ERROR("Format not supported for image encoding (supported formats are PNG and JPEG)");
      break;
    }
  }
  return false;
}
} // anonymous namespace


bool EncodeToFile(const unsigned char* const pixelBuffer,
                  const std::string& filename,
                  const Pixel::Format pixelFormat,
                  const std::size_t width,
                  const std::size_t height )
{
  DALI_ASSERT_DEBUG(pixelBuffer != 0 && filename.size() > 4 && width > 0 && height > 0);
  Vector< unsigned char > pixbufEncoded;
  const FileFormat format = GetFormatFromFileName( filename );
  const bool encodeResult = EncodeToFormat( pixelBuffer, pixbufEncoded, format, width, height, pixelFormat );
  if(!encodeResult)
  {
    DALI_LOG_ERROR("Encoding pixels failed");
    return false;
  }
  return TizenPlatform::ResourceLoader::SaveFile( filename, pixbufEncoded.Begin(), pixbufEncoded.Count() );
}

} // namespace Dali
