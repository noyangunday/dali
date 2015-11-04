/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#include "loader-gif.h"

#include <gif_lib.h>
#include <cstdlib>

#include <dali/integration-api/debug.h>
#include <dali/integration-api/bitmap.h>

namespace Dali
{
using Integration::Bitmap;
using Dali::Integration::PixelBuffer;

namespace TizenPlatform
{

namespace
{

// simple class to enforce clean-up of GIF structures
struct AutoCleanupGif
{
  AutoCleanupGif(GifFileType*& _gifInfo)
  : gifInfo(_gifInfo)
  {
  }

  ~AutoCleanupGif()
  {
    if(NULL != gifInfo)
    {
      // clean up GIF resources
      DGifCloseFile(gifInfo);
    }
  }

  GifFileType*& gifInfo;
};

// Simple class to enforce clean-up of PixelBuffer
struct AutoDeleteBuffer
{
  AutoDeleteBuffer( PixelBuffer* _buffer )
  : buffer( _buffer )
  {
  }

  ~AutoDeleteBuffer()
  {
    delete []buffer;
  }

  PixelBuffer* buffer;
};

// Used in the GIF interlace algorithm to determine the starting byte and the increment required
// for each pass.
struct InterlacePair
{
  unsigned int startingByte;
  unsigned int incrementalByte;
};

// Used in the GIF interlace algorithm to determine the order and which location to read data from
// the file.
const InterlacePair INTERLACE_PAIR_TABLE [] = {
  { 0, 8 }, // Starting at 0, read every 8 bytes.
  { 4, 8 }, // Starting at 4, read every 8 bytes.
  { 2, 4 }, // Starting at 2, read every 4 bytes.
  { 1, 2 }, // Starting at 1, read every 2 bytes.
};
const unsigned int INTERLACE_PAIR_TABLE_SIZE( sizeof( INTERLACE_PAIR_TABLE ) / sizeof( InterlacePair ) );

/// Function used by Gif_Lib to read from the image file.
int ReadDataFromGif(GifFileType *gifInfo, GifByteType *data, int length)
{
  FILE *fp = reinterpret_cast<FILE*>(gifInfo->UserData);
  return fread( data, sizeof( GifByteType ), length, fp);
}

/// Loads the GIF Header.
bool LoadGifHeader(FILE *fp, unsigned int &width, unsigned int &height, GifFileType** gifInfo)
{
  *gifInfo = DGifOpen(reinterpret_cast<void*>(fp), ReadDataFromGif);

  if ( !(*gifInfo) )
  {
    return false;
  }

  width  = (*gifInfo)->SWidth;
  height = (*gifInfo)->SHeight;

  // No proper size in GIF.
  if ( width <= 0 || height <= 0 )
  {
    return false;
  }

  return true;
}

/// Decode the GIF image.
bool DecodeImage( GifFileType* gifInfo, PixelBuffer* decodedData, const unsigned int width, const unsigned int height, const unsigned int bytesPerRow )
{
  if ( gifInfo->Image.Interlace )
  {
    // If the image is interlaced, then use the GIF interlace algorithm to read the file appropriately.

    const InterlacePair* interlacePairPtr( INTERLACE_PAIR_TABLE );
    for ( unsigned int interlacePair = 0; interlacePair < INTERLACE_PAIR_TABLE_SIZE; ++interlacePair, ++interlacePairPtr )
    {
      for( unsigned int currentByte = interlacePairPtr->startingByte; currentByte < height; currentByte += interlacePairPtr->incrementalByte )
      {
        PixelBuffer* row = decodedData + currentByte * bytesPerRow;
        if ( DGifGetLine( gifInfo, row, width ) == GIF_ERROR )
        {
          DALI_LOG_ERROR( "GIF Loader: Error reading Interlaced GIF\n" );
          return false;
        }
      }
    }
  }
  else
  {
    // Non-interlace does not require any erratic reading / jumping.
    PixelBuffer* decodedDataPtr( decodedData );

    for ( unsigned int row = 0; row < height; ++row )
    {
      if ( DGifGetLine( gifInfo, decodedDataPtr, width ) == GIF_ERROR)
      {
        DALI_LOG_ERROR( "GIF Loader: Error reading non-interlaced GIF\n" );
        return false;
      }
      decodedDataPtr += bytesPerRow;
    }
  }
  return true;
}

// Retrieves the colors used in the GIF image.
GifColorType* GetImageColors( SavedImage* image, GifFileType* gifInfo )
{
  GifColorType* color( NULL );
  if ( image->ImageDesc.ColorMap )
  {
    color = image->ImageDesc.ColorMap->Colors;
  }
  else
  {
    // if there is no color map for this image use the default one
    color = gifInfo->SColorMap->Colors;
  }
  return color;
}

/// Called when we want to handle IMAGE_DESC_RECORD_TYPE
bool HandleImageDescriptionRecordType( Bitmap& bitmap, GifFileType* gifInfo, unsigned int width, unsigned int height, bool& finished )
{
  if ( DGifGetImageDesc( gifInfo ) == GIF_ERROR )
  {
    DALI_LOG_ERROR( "GIF Loader: Error getting Image Description\n" );
    return false;
  }

  // Ensure there is at least 1 image in the GIF.
  if ( gifInfo->ImageCount < 1 )
  {
    DALI_LOG_ERROR( "GIF Loader: No Images\n" );
    return false;
  }

  SavedImage* image( &gifInfo->SavedImages[ gifInfo->ImageCount - 1 ] );
  const GifImageDesc& desc( image->ImageDesc );

  // Create a buffer to store the decoded data.
  PixelBuffer* decodedData( new PixelBuffer[ width * height * sizeof( GifPixelType ) ] );
  AutoDeleteBuffer autoDeleteBuffer( decodedData );

  const unsigned int bytesPerRow( width * sizeof( GifPixelType ) );
  const unsigned int actualWidth( desc.Width );
  const unsigned int actualHeight( desc.Height );

  // Decode the GIF Image
  if ( !DecodeImage( gifInfo, decodedData, actualWidth, actualHeight, bytesPerRow ) )
  {
    return false;
  }

  // Get the colormap for the GIF
  GifColorType* color( GetImageColors( image, gifInfo ) );

  // If it's an animated GIF, we still only read the first image

  // Create and populate pixel buffer.

  Pixel::Format pixelFormat( Pixel::RGB888 );
  PixelBuffer *pixels = bitmap.GetPackedPixelsProfile()->ReserveBuffer( pixelFormat, actualWidth, actualHeight );

  for (unsigned int row = 0; row < actualHeight; ++row)
  {
    for (unsigned int column = 0; column < actualWidth; ++column)
    {
      unsigned char index = decodedData[row * width + column];

      pixels[0] = color[index].Red;
      pixels[1] = color[index].Green;
      pixels[2] = color[index].Blue;
      pixels += 3;
    }
  }

  finished = true;

  return true;
}

/// Called when we want to handle EXTENSION_RECORD_TYPE
bool HandleExtensionRecordType( GifFileType* gifInfo )
{
  SavedImage image;
  image.ExtensionBlocks     = NULL;
  image.ExtensionBlockCount = 0;
  GifByteType *extensionByte( NULL );

  // Not really interested in the extensions so just skip them unless there is an error.
  for ( int extRetCode = DGifGetExtension( gifInfo, &image.Function, &extensionByte );
        extensionByte != NULL;
        extRetCode = DGifGetExtensionNext( gifInfo, &extensionByte ) )
  {
    if ( extRetCode == GIF_ERROR )
    {
      DALI_LOG_ERROR( "GIF Loader: Error reading GIF Extension record.\n" );
      return false;
    }
  }

  return true;
}

} // unnamed namespace

bool LoadGifHeader( const ImageLoader::Input& input, unsigned int& width, unsigned int& height )
{
  GifFileType* gifInfo = NULL;
  AutoCleanupGif autoCleanupGif(gifInfo);
  FILE* const fp = input.file;

  return LoadGifHeader(fp, width, height, &gifInfo);
}

bool LoadBitmapFromGif( const ResourceLoadingClient& client, const ImageLoader::Input& input, Integration::Bitmap& bitmap )
{
  FILE* const fp = input.file;
  // Load the GIF Header file.

  GifFileType* gifInfo( NULL );
  unsigned int width( 0 );
  unsigned int height( 0 );
  if ( !LoadGifHeader( fp, width, height, &gifInfo ) )
  {
    return false;
  }
  AutoCleanupGif autoGif( gifInfo );

  // Check each record in the GIF file.

  bool finished( false );
  GifRecordType recordType( UNDEFINED_RECORD_TYPE );
  for ( int returnCode = DGifGetRecordType( gifInfo, &recordType );
        !finished && recordType != TERMINATE_RECORD_TYPE;
        returnCode = DGifGetRecordType( gifInfo, &recordType ) )
  {
    if ( returnCode == GIF_ERROR )
    {
      DALI_LOG_ERROR( "GIF Loader: Error getting Record Type\n" );
      return false;
    }

    if( IMAGE_DESC_RECORD_TYPE == recordType )
    {
      if ( !HandleImageDescriptionRecordType( bitmap, gifInfo, width, height, finished ) )
      {
        return false;
      }
    }
    else if ( EXTENSION_RECORD_TYPE == recordType )
    {
      if ( !HandleExtensionRecordType( gifInfo ))
      {
        return false;
      }
    }
  }

  return true;
}

} // namespace TizenPlatform

} // namespace Dali
