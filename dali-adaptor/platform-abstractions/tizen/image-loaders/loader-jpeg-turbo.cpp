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

// INTERNAL HEADERS
#include "loader-jpeg.h"
#include "resource-loading-client.h"
#include <dali/integration-api/bitmap.h>
#include <resource-loader/debug/resource-loader-debug.h>
#include "platform-capabilities.h"
#include "image-operations.h"

// EXTERNAL HEADERS
#include <libexif/exif-data.h>
#include <libexif/exif-loader.h>
#include <libexif/exif-tag.h>
#include <turbojpeg.h>
#include <jpeglib.h>
#include <cstring>
#include <setjmp.h>

namespace Dali
{
using Integration::Bitmap;

namespace TizenPlatform
{

namespace
{
  const unsigned DECODED_PIXEL_SIZE = 3;
  const TJPF DECODED_PIXEL_LIBJPEG_TYPE = TJPF_RGB;

  /** Transformations that can be applied to decoded pixels to respect exif orientation
   *  codes in image headers */
  enum JPGFORM_CODE
  {
    JPGFORM_NONE = 1, /* no transformation 0th-Row = top & 0th-Column = left */
    JPGFORM_FLIP_H,   /* horizontal flip 0th-Row = top & 0th-Column = right */
    JPGFORM_FLIP_V,   /* vertical flip   0th-Row = bottom & 0th-Column = right*/
    JPGFORM_TRANSPOSE, /* transpose across UL-to-LR axis  0th-Row = bottom & 0th-Column = left*/
    JPGFORM_TRANSVERSE,/* transpose across UR-to-LL axis  0th-Row = left   & 0th-Column = top*/
    JPGFORM_ROT_90,    /* 90-degree clockwise rotation  0th-Row = right  & 0th-Column = top*/
    JPGFORM_ROT_180,   /* 180-degree rotation  0th-Row = right  & 0th-Column = bottom*/
    JPGFORM_ROT_270    /* 270-degree clockwise (or 90 ccw) 0th-Row = left  & 0th-Column = bottom*/
  };

  struct RGB888Type
  {
     char R;
     char G;
     char B;
  };

  /**
   * @brief Error handling bookeeping for the JPEG Turbo library's
   * setjmp/longjmp simulated exceptions.
   */
  struct JpegErrorState {
    struct jpeg_error_mgr errorManager;
    jmp_buf jumpBuffer;
  };

  /**
   * @brief Called by the JPEG library when it hits an error.
   * We jump out of the library so our loader code can return an error.
   */
  void  JpegErrorHandler ( j_common_ptr cinfo )
  {
    DALI_LOG_ERROR( "JpegErrorHandler(): libjpeg-turbo fatal error in JPEG decoding.\n" );
    /* cinfo->err really points to a JpegErrorState struct, so coerce pointer */
    JpegErrorState * myerr = reinterpret_cast<JpegErrorState *>( cinfo->err );

    /* Return control to the setjmp point */
    longjmp( myerr->jumpBuffer, 1 );
  }

  void JpegOutputMessageHandler( j_common_ptr cinfo )
  {
    /* Stop libjpeg from printing to stderr - Do Nothing */
  }

  /** Simple struct to ensure xif data is deleted. */
  struct ExifAutoPtr
  {
    ExifAutoPtr( ExifData* data)
    :mData( data )
    {}

    ~ExifAutoPtr()
    {
      exif_data_free( mData);
    }
    ExifData *mData;
  };

  /** simple class to enforce clean-up of JPEG structures. */
  struct AutoJpg
  {
    AutoJpg(const tjhandle jpgHandle)
    : mHnd(jpgHandle)
    {
    }

    ~AutoJpg()
    {
      // clean up JPG resources
      tjDestroy( mHnd );
    }

    tjhandle GetHandle() const
    {
      return mHnd ;
    }

  private:
    AutoJpg( const AutoJpg& ); //< not defined
    AutoJpg& operator= ( const AutoJpg& ); //< not defined

    tjhandle mHnd;
  }; // struct AutoJpg;

  /** RAII wrapper to free memory allocated by the jpeg-turbo library. */
  struct AutoJpgMem
  {
    AutoJpgMem(unsigned char * const tjMem)
    : mTjMem(tjMem)
    {
    }

    ~AutoJpgMem()
    {
      tjFree(mTjMem);
    }

    unsigned char * Get() const
    {
      return mTjMem;
    }

  private:
    AutoJpgMem( const AutoJpgMem& ); //< not defined
    AutoJpgMem& operator= ( const AutoJpgMem& ); //< not defined

    unsigned char * const mTjMem;
  };

  // Workaround to avoid exceeding the maximum texture size
  const int MAX_TEXTURE_WIDTH  = 4096;
  const int MAX_TEXTURE_HEIGHT = 4096;

} // namespace

bool JpegRotate90 (unsigned char *buffer, int width, int height, int bpp);
bool JpegRotate180(unsigned char *buffer, int width, int height, int bpp);
bool JpegRotate270(unsigned char *buffer, int width, int height, int bpp);
JPGFORM_CODE ConvertExifOrientation(ExifData* exifData);
bool TransformSize( int requiredWidth, int requiredHeight,
                    FittingMode::Type fittingMode, SamplingMode::Type samplingMode,
                    JPGFORM_CODE transform,
                    int& preXformImageWidth, int& preXformImageHeight,
                    int& postXformImageWidth, int& postXformImageHeight );

bool LoadJpegHeader( FILE *fp, unsigned int &width, unsigned int &height )
{
  // using libjpeg API to avoid having to read the whole file in a buffer
  struct jpeg_decompress_struct cinfo;
  struct JpegErrorState jerr;
  cinfo.err = jpeg_std_error( &jerr.errorManager );

  jerr.errorManager.output_message = JpegOutputMessageHandler;
  jerr.errorManager.error_exit = JpegErrorHandler;

  // On error exit from the JPEG lib, control will pass via JpegErrorHandler
  // into this branch body for cleanup and error return:
  if(setjmp(jerr.jumpBuffer))
  {
    jpeg_destroy_decompress(&cinfo);
    return false;
  }

  jpeg_create_decompress( &cinfo );

  jpeg_stdio_src( &cinfo, fp );

  // Check header to see if it is  JPEG file
  if( jpeg_read_header( &cinfo, TRUE ) != JPEG_HEADER_OK )
  {
    width = height = 0;
    jpeg_destroy_decompress( &cinfo );
    return false;
  }

  width = cinfo.image_width;
  height = cinfo.image_height;

  jpeg_destroy_decompress( &cinfo );
  return true;
}

bool LoadBitmapFromJpeg( const ResourceLoadingClient& client, const ImageLoader::Input& input, Integration::Bitmap& bitmap )
{
  const int flags= 0;
  FILE* const fp = input.file;

  if( fseek(fp,0,SEEK_END) )
  {
    DALI_LOG_ERROR("Error seeking to end of file\n");
    return false;
  }

  long positionIndicator = ftell(fp);
  unsigned int jpegBufferSize = 0u;
  if( positionIndicator > -1L )
  {
    jpegBufferSize = static_cast<unsigned int>(positionIndicator);
  }

  if( 0u == jpegBufferSize )
  {
    return false;
  }

  if( fseek(fp, 0, SEEK_SET) )
  {
    DALI_LOG_ERROR("Error seeking to start of file\n");
    return false;
  }

  Vector<unsigned char> jpegBuffer;
  try
  {
    jpegBuffer.Reserve( jpegBufferSize );
  }
  catch(...)
  {
    DALI_LOG_ERROR( "Could not allocate temporary memory to hold JPEG file of size %uMB.\n", jpegBufferSize / 1048576U );
    return false;
  }
  unsigned char * const jpegBufferPtr = jpegBuffer.Begin();

  // Pull the compressed JPEG image bytes out of a file and into memory:
  if( fread( jpegBufferPtr, 1, jpegBufferSize, fp ) != jpegBufferSize )
  {
    DALI_LOG_WARNING("Error on image file read.");
    return false;
  }

  if( fseek(fp, 0, SEEK_SET) )
  {
    DALI_LOG_ERROR("Error seeking to start of file\n");
  }

  // Allow early cancellation between the load and the decompress:
  client.InterruptionPoint();

  AutoJpg autoJpg(tjInitDecompress());

  if(autoJpg.GetHandle() == NULL)
  {
    DALI_LOG_ERROR("%s\n", tjGetErrorStr());
    return false;
  }

  JPGFORM_CODE transform = JPGFORM_NONE;

  if( input.reorientationRequested )
  {
    ExifAutoPtr exifData( exif_data_new_from_data(jpegBufferPtr, jpegBufferSize) );
    if( exifData.mData )
    {
      transform = ConvertExifOrientation(exifData.mData);
    }
  }

  // Push jpeg data in memory buffer through TurboJPEG decoder to make a raw pixel array:
  int chrominanceSubsampling = -1;
  int preXformImageWidth = 0, preXformImageHeight = 0;
  if( tjDecompressHeader2( autoJpg.GetHandle(), jpegBufferPtr, jpegBufferSize, &preXformImageWidth, &preXformImageHeight, &chrominanceSubsampling ) == -1 )
  {
    DALI_LOG_ERROR("%s\n", tjGetErrorStr());
    // Do not set width and height to 0 or return early as this sometimes fails only on determining subsampling type.
  }

  if(preXformImageWidth == 0 || preXformImageHeight == 0)
  {
    DALI_LOG_WARNING("Invalid Image!");
    return false;
  }

  int requiredWidth  = input.scalingParameters.dimensions.GetWidth();
  int requiredHeight = input.scalingParameters.dimensions.GetHeight();

  // If transform is a 90 or 270 degree rotation, the logical width and height
  // request from the client needs to be adjusted to account by effectively
  // rotating that too, and the final width and height need to be swapped:
  int postXformImageWidth = preXformImageWidth;
  int postXformImageHeight = preXformImageHeight;


  int scaledPreXformWidth   = preXformImageWidth;
  int scaledPreXformHeight  = preXformImageHeight;
  int scaledPostXformWidth  = postXformImageWidth;
  int scaledPostXformHeight = postXformImageHeight;

  TransformSize( requiredWidth, requiredHeight,
                 input.scalingParameters.scalingMode,
                 input.scalingParameters.samplingMode,
                 transform,
                 scaledPreXformWidth, scaledPreXformHeight,
                 scaledPostXformWidth, scaledPostXformHeight );

  // Allocate a bitmap and decompress the jpeg buffer into its pixel buffer:

  unsigned char * const bitmapPixelBuffer =  bitmap.GetPackedPixelsProfile()->ReserveBuffer(Pixel::RGB888, scaledPostXformWidth, scaledPostXformHeight);

  // Allow early cancellation before decoding:
  client.InterruptionPoint();

  if( tjDecompress2( autoJpg.GetHandle(), jpegBufferPtr, jpegBufferSize, bitmapPixelBuffer, scaledPreXformWidth, 0, scaledPreXformHeight, DECODED_PIXEL_LIBJPEG_TYPE, flags ) == -1 )
  {
    DALI_LOG_ERROR("%s\n", tjGetErrorStr());
    return false;
  }

  const unsigned int  bufferWidth  = GetTextureDimension( scaledPreXformWidth );
  const unsigned int  bufferHeight = GetTextureDimension( scaledPreXformHeight );

  if( transform != JPGFORM_NONE )
  {
    // Allow early cancellation before shuffling pixels around on the CPU:
    client.InterruptionPoint();
  }

  bool result = false;
  switch(transform)
  {
    case JPGFORM_NONE:
    {
      result = true;
      break;
    }
    // 3 orientation changes for a camera held perpendicular to the ground or upside-down:
    case JPGFORM_ROT_180:
    {
      result = JpegRotate180(bitmapPixelBuffer, bufferWidth, bufferHeight, DECODED_PIXEL_SIZE);
      break;
    }
    case JPGFORM_ROT_270:
    {
      result = JpegRotate270(bitmapPixelBuffer, bufferWidth, bufferHeight, DECODED_PIXEL_SIZE);
      break;
    }
    case JPGFORM_ROT_90:
    {
      result = JpegRotate90(bitmapPixelBuffer, bufferWidth, bufferHeight, DECODED_PIXEL_SIZE);
      break;
    }
    /// Less-common orientation changes, since they don't correspond to a camera's
    // physical orientation:
    case JPGFORM_FLIP_H:
    case JPGFORM_FLIP_V:
    case JPGFORM_TRANSPOSE:
    case JPGFORM_TRANSVERSE:
    {
      DALI_LOG_WARNING( "Unsupported JPEG Orientation transformation: %x.\n", transform );
      break;
    }
  }
  return result;
}

///@Todo: Move all these rotation functions to portable/image-operations and take "Jpeg" out of their names.
bool JpegRotate90(unsigned char *buffer, int width, int height, int bpp)
{
  int  w, iw, ih, hw = 0;
  int ix, iy = 0;
  iw = width;
  ih = height;
  Vector<unsigned char> data;
  data.Reserve(width * height * bpp);
  unsigned char *dataPtr = data.Begin();
  memcpy(dataPtr, buffer, width * height * bpp);
  w = ih;
  ih = iw;
  iw = w;
  hw = iw * ih;
  hw = - hw - 1;
  switch(bpp)
  {
    case 3:
    {
      RGB888Type* to = reinterpret_cast<RGB888Type*>(buffer) + iw - 1;
      RGB888Type* from = reinterpret_cast<RGB888Type*>( dataPtr );

      for(ix = iw; -- ix >= 0;)
      {
        for(iy = ih; -- iy >= 0; ++from )
        {
          *to = *from;
          to += iw;
        }
        to += hw;
      }
      break;
    }

    default:
    {
      return false;
    }
  }

  return true;
}

bool JpegRotate180(unsigned char *buffer, int width, int height, int bpp)
{
  int  ix, iw, ih, hw = 0;
  iw = width;
  ih = height;
  hw = iw * ih;
  ix = hw;

  switch(bpp)
  {
    case 3:
    {
      RGB888Type tmp;
      RGB888Type* to = reinterpret_cast<RGB888Type*>(buffer) ;
      RGB888Type* from = reinterpret_cast<RGB888Type*>( buffer ) + hw - 1;
      for(; --ix >= (hw / 2); ++to, --from)
      {
        tmp = *to;
        *to = *from;
        *from = tmp;
      }
      break;
    }

    default:
    {
      return false;
    }
  }

  return true;
}

bool JpegRotate270(unsigned char *buffer, int width, int height, int bpp)
{
  int  w, iw, ih, hw = 0;
  int ix, iy = 0;

  iw = width;
  ih = height;
  Vector<unsigned char> data;
  data.Reserve(width * height * bpp);
  unsigned char *dataPtr = data.Begin();
  memcpy(dataPtr, buffer, width * height * bpp);
  w = ih;
  ih = iw;
  iw = w;
  hw = iw * ih;

  switch(bpp)
  {
    case 3:
    {
      RGB888Type* to = reinterpret_cast<RGB888Type*>(buffer) + hw  - iw;
      RGB888Type* from = reinterpret_cast<RGB888Type*>( dataPtr );

      w = -w;
      hw =  hw + 1;
      for(ix = iw; -- ix >= 0;)
      {
        for(iy = ih; -- iy >= 0;)
        {
          *to = *from;
          from += 1;
          to += w;
        }
        to += hw;
      }
      break;
    }
    default:
    {
      return false;
    }
  }

  return true;
}

bool EncodeToJpeg( const unsigned char* const pixelBuffer, Vector< unsigned char >& encodedPixels,
                   const std::size_t width, const std::size_t height, const Pixel::Format pixelFormat, unsigned quality )
{
  if( !pixelBuffer )
  {
    DALI_LOG_ERROR("Null input buffer\n");
    return false;
  }

  // Translate pixel format enum:
  int jpegPixelFormat = -1;

  switch( pixelFormat )
  {
    case Pixel::RGB888:
    {
      jpegPixelFormat = TJPF_RGB;
      break;
    }
    case Pixel::RGBA8888:
    {
      // Ignore the alpha:
      jpegPixelFormat = TJPF_RGBX;
      break;
    }
    case Pixel::BGRA8888:
    {
      // Ignore the alpha:
      jpegPixelFormat = TJPF_BGRX;
      break;
    }
    default:
    {
      DALI_LOG_ERROR( "Unsupported pixel format for encoding to JPEG." );
      return false;
    }
  }

  // Assert quality is in the documented allowable range of the jpeg-turbo lib:
  DALI_ASSERT_DEBUG( quality >= 1 );
  DALI_ASSERT_DEBUG( quality <= 100 );
  if( quality < 1 )
  {
    quality = 1;
  }
  if( quality > 100 )
  {
    quality = 100;
  }

  // Initialise a JPEG codec:
  AutoJpg autoJpg( tjInitCompress() );
  {
    if( autoJpg.GetHandle() == NULL )
    {
      DALI_LOG_ERROR( "JPEG Compressor init failed: %s\n", tjGetErrorStr() );
      return false;
    }

    // Run the compressor:
    unsigned char* dstBuffer = NULL;
    unsigned long dstBufferSize = 0;
    const int flags = 0;

    if( tjCompress2( autoJpg.GetHandle(), const_cast<unsigned char*>(pixelBuffer), width, 0, height, jpegPixelFormat, &dstBuffer, &dstBufferSize, TJSAMP_444, quality, flags ) )
    {
      DALI_LOG_ERROR("JPEG Compression failed: %s\n", tjGetErrorStr());
      return false;
    }

    // Safely wrap the jpeg codec's buffer in case we are about to throw, then
    // save the pixels to a persistent buffer that we own and let our cleaner
    // class clean up the buffer as it goes out of scope:
    AutoJpgMem cleaner( dstBuffer );
    encodedPixels.Reserve( dstBufferSize );
    memcpy( encodedPixels.Begin(), dstBuffer, dstBufferSize );
  }
  return true;
}


JPGFORM_CODE ConvertExifOrientation(ExifData* exifData)
{
  JPGFORM_CODE transform = JPGFORM_NONE;
  ExifEntry * const entry = exif_data_get_entry(exifData, EXIF_TAG_ORIENTATION);
  int orientation = 0;
  if( entry )
  {
    orientation = exif_get_short(entry->data, exif_data_get_byte_order(entry->parent->parent));
    switch( orientation )
    {
      case 1:
      {
        transform = JPGFORM_NONE;
        break;
      }
      case 2:
      {
        transform = JPGFORM_FLIP_H;
        break;
      }
      case 3:
      {
        transform = JPGFORM_FLIP_V;
        break;
      }
      case 4:
      {
        transform = JPGFORM_TRANSPOSE;
        break;
      }
      case 5:
      {
        transform = JPGFORM_TRANSVERSE;
        break;
      }
      case 6:
      {
        transform = JPGFORM_ROT_90;
        break;
      }
      case 7:
      {
        transform = JPGFORM_ROT_180;
        break;
      }
      case 8:
      {
        transform = JPGFORM_ROT_270;
        break;
      }
      default:
      {
        // Try to keep loading the file, but let app developer know there was something fishy:
        DALI_LOG_WARNING( "Incorrect/Unknown Orientation setting found in EXIF header of JPEG image (%x). Orientation setting will be ignored.", entry );
        break;
      }
    }
  }
  return transform;
}

bool TransformSize( int requiredWidth, int requiredHeight,
                    FittingMode::Type fittingMode, SamplingMode::Type samplingMode,
                    JPGFORM_CODE transform,
                    int& preXformImageWidth, int& preXformImageHeight,
                    int& postXformImageWidth, int& postXformImageHeight )
{
  bool success = true;

  if( transform == JPGFORM_ROT_90 || transform == JPGFORM_ROT_270 )
  {
    std::swap( requiredWidth, requiredHeight );
    std::swap( postXformImageWidth, postXformImageHeight );
  }

  // Apply the special rules for when there are one or two zeros in requested dimensions:
  const ImageDimensions correctedDesired = Internal::Platform::CalculateDesiredDimensions( ImageDimensions( postXformImageWidth, postXformImageHeight), ImageDimensions( requiredWidth, requiredHeight ) );
  requiredWidth = correctedDesired.GetWidth();
  requiredHeight = correctedDesired.GetHeight();

  // Rescale image during decode using one of the decoder's built-in rescaling
  // ratios (expected to be powers of 2), keeping the final image at least as
  // wide and high as was requested:

  int numFactors = 0;
  tjscalingfactor* factors = tjGetScalingFactors( &numFactors );
  if( factors == NULL )
  {
    DALI_LOG_WARNING("TurboJpeg tjGetScalingFactors error!");
    success = false;
  }
  else
  {
    // Internal jpeg downscaling is the same as our BOX_X sampling modes so only
    // apply it if the application requested one of those:
    // (use a switch case here so this code will fail to compile if other modes are added)
    bool downscale = true;
    switch( samplingMode )
    {
      case SamplingMode::BOX:
      case SamplingMode::BOX_THEN_NEAREST:
      case SamplingMode::BOX_THEN_LINEAR:
      case SamplingMode::DONT_CARE:
      {
        downscale = true;
        break;
      }
      case SamplingMode::NO_FILTER:
      case SamplingMode::NEAREST:
      case SamplingMode::LINEAR:
      {
        downscale = false;
        break;
      }
    }

    int scaleFactorIndex( 0 );
    if( downscale )
    {
      // Find nearest supported scaling factor (factors are in sequential order, getting smaller)
      for( int i = 1; i < numFactors; ++i )
      {
        bool widthLessRequired  = TJSCALED( postXformImageWidth,  factors[i]) < requiredWidth;
        bool heightLessRequired = TJSCALED( postXformImageHeight, factors[i]) < requiredHeight;
        // If either scaled dimension is smaller than the desired one, we were done at the last iteration
        if ( (fittingMode == FittingMode::SCALE_TO_FILL) && (widthLessRequired || heightLessRequired) )
        {
          break;
        }
        // If both dimensions are smaller than the desired one, we were done at the last iteration:
        if ( (fittingMode == FittingMode::SHRINK_TO_FIT) && ( widthLessRequired && heightLessRequired ) )
        {
          break;
        }
        // If the width is smaller than the desired one, we were done at the last iteration:
        if ( fittingMode == FittingMode::FIT_WIDTH && widthLessRequired )
        {
          break;
        }
        // If the width is smaller than the desired one, we were done at the last iteration:
        if ( fittingMode == FittingMode::FIT_HEIGHT && heightLessRequired )
        {
          break;
        }
        // This factor stays is within our fitting mode constraint so remember it:
        scaleFactorIndex = i;
      }
    }

    // Regardless of requested size, downscale to avoid exceeding the maximum texture size:
    for( int i = scaleFactorIndex; i < numFactors; ++i )
    {
      // Continue downscaling to below maximum texture size (if possible)
      scaleFactorIndex = i;

      if( TJSCALED(postXformImageWidth,  (factors[i])) < MAX_TEXTURE_WIDTH &&
          TJSCALED(postXformImageHeight, (factors[i])) < MAX_TEXTURE_HEIGHT )
      {
        // Current scale-factor downscales to below maximum texture size
        break;
      }
    }

    // We have finally chosen the scale-factor, return width/height values
    if( scaleFactorIndex > 0 )
    {
      preXformImageWidth   = TJSCALED(preXformImageWidth,   (factors[scaleFactorIndex]));
      preXformImageHeight  = TJSCALED(preXformImageHeight,  (factors[scaleFactorIndex]));
      postXformImageWidth  = TJSCALED(postXformImageWidth,  (factors[scaleFactorIndex]));
      postXformImageHeight = TJSCALED(postXformImageHeight, (factors[scaleFactorIndex]));
    }
  }

  return success;
}

ExifData* LoadExifData( FILE* fp )
{
  ExifData*     exifData=NULL;
  ExifLoader*   exifLoader;
  unsigned char dataBuffer[1024];

  if( fseek( fp, 0, SEEK_SET ) )
  {
    DALI_LOG_ERROR("Error seeking to start of file\n");
  }
  else
  {
    exifLoader = exif_loader_new ();

    while( !feof(fp) )
    {
      int size = fread( dataBuffer, 1, sizeof( dataBuffer ), fp );
      if( size <= 0 )
      {
        break;
      }
      if( ! exif_loader_write( exifLoader, dataBuffer, size ) )
      {
        break;
      }
    }

    exifData = exif_loader_get_data( exifLoader );
    exif_loader_unref( exifLoader );
  }

  return exifData;
}

bool LoadJpegHeader( const ImageLoader::Input& input, unsigned int& width, unsigned int& height )
{
  unsigned int requiredWidth  = input.scalingParameters.dimensions.GetWidth();
  unsigned int requiredHeight = input.scalingParameters.dimensions.GetHeight();
  FILE* const fp = input.file;

  bool success = false;
  if( requiredWidth == 0 && requiredHeight == 0 )
  {
    success = LoadJpegHeader( fp, width, height );
  }
  else
  {
    // Double check we get the same width/height from the header
    unsigned int headerWidth;
    unsigned int headerHeight;
    if( LoadJpegHeader( fp, headerWidth, headerHeight ) )
    {
      JPGFORM_CODE transform = JPGFORM_NONE;

      if( input.reorientationRequested )
      {
        ExifAutoPtr exifData( LoadExifData( fp ) );
        if( exifData.mData )
        {
          transform = ConvertExifOrientation(exifData.mData);
        }

        int preXformImageWidth = headerWidth;
        int preXformImageHeight = headerHeight;
        int postXformImageWidth = headerWidth;
        int postXformImageHeight = headerHeight;

        success = TransformSize( requiredWidth, requiredHeight, input.scalingParameters.scalingMode, input.scalingParameters.samplingMode, transform, preXformImageWidth, preXformImageHeight, postXformImageWidth, postXformImageHeight );
        if(success)
        {
          width = postXformImageWidth;
          height = postXformImageHeight;
        }
      }
      else
      {
        success = true;
        width = headerWidth;
        height = headerHeight;
      }
    }
  }
  return success;
}


} // namespace TizenPlatform

} // namespace Dali
