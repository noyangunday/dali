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

#include "loader-ktx.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include <dali/public-api/common/compile-time-assert.h>
#include <dali/integration-api/debug.h>
#include <dali/integration-api/bitmap.h>
#include <dali/public-api/images/pixel.h>

namespace Dali
{
using Integration::Bitmap;
using Dali::Integration::PixelBuffer;

namespace TizenPlatform
{

namespace
{

/** Max width or height of an image. */
const unsigned MAX_TEXTURE_DIMENSION = 4096;
/** Max bytes of image data allowed. Not a precise number, just a sanity check. */
const unsigned MAX_IMAGE_DATA_SIZE = MAX_TEXTURE_DIMENSION * MAX_TEXTURE_DIMENSION;
/** We don't read any of this but limit it to a resonable amount in order to be
 * friendly to files from random tools. */
const unsigned MAX_BYTES_OF_KEYVALUE_DATA = 65536U;

typedef uint8_t Byte;

const Byte FileIdentifier[] = {
   0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A
};

/** The formats we support inside a KTX file container.
 *  Currently only compressed formats are allowed as we'd rather
 *  use a PNG or JPEG with their own compression for the general
 *  cases. */
enum KtxInternalFormat
{
  KTX_NOTEXIST = 0,
  // GLES 3 Standard compressed formats (values same as in gl3.h):
  KTX_COMPRESSED_R11_EAC                          = 0x9270,
  KTX_COMPRESSED_SIGNED_R11_EAC                   = 0x9271,
  KTX_COMPRESSED_RG11_EAC                         = 0x9272,
  KTX_COMPRESSED_SIGNED_RG11_EAC                  = 0x9273,
  KTX_COMPRESSED_RGB8_ETC2                        = 0x9274,
  KTX_COMPRESSED_SRGB8_ETC2                       = 0x9275,
  KTX_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2    = 0x9276,
  KTX_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2   = 0x9277,
  KTX_COMPRESSED_RGBA8_ETC2_EAC                   = 0x9278,
  KTX_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC            = 0x9279,

  // GLES 2 EXTENSION FORMATS:
  KTX_ETC1_RGB8_OES                               = 0x8D64,
  KTX_COMPRESSED_RGB_PVRTC_4BPPV1_IMG             = 0x8C00,
  KTX_SENTINEL = ~0u,
};

const unsigned KtxInternalFormats[] =
{
  // GLES 3 Standard compressed formats:
  KTX_COMPRESSED_R11_EAC,
  KTX_COMPRESSED_SIGNED_R11_EAC,
  KTX_COMPRESSED_RG11_EAC,
  KTX_COMPRESSED_SIGNED_RG11_EAC,
  KTX_COMPRESSED_RGB8_ETC2,
  KTX_COMPRESSED_SRGB8_ETC2,
  KTX_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2,
  KTX_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2,
  KTX_COMPRESSED_RGBA8_ETC2_EAC,
  KTX_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC,
  // GLES 2 EXTENSION FORMATS:
  KTX_ETC1_RGB8_OES,
  KTX_COMPRESSED_RGB_PVRTC_4BPPV1_IMG,
  KTX_SENTINEL
};

struct KtxFileHeader
{
  Byte   identifier[12];
  uint32_t endianness;
  uint32_t glType;
  uint32_t glTypeSize;
  uint32_t glFormat;
  uint32_t glInternalFormat;
  uint32_t glBaseInternalFormat;
  uint32_t pixelWidth;
  uint32_t pixelHeight;
  uint32_t pixelDepth;
  uint32_t numberOfArrayElements;
  uint32_t numberOfFaces;
  uint32_t numberOfMipmapLevels;
  uint32_t bytesOfKeyValueData;
} __attribute__ ( (__packed__));
// Packed attribute stops the structure from being aligned to compiler defaults
// so we can be sure of reading the whole thing from file in one call to fread.

/**
 * Template function to read from the file directly into our structure.
 * @param[in]  fp     The file to read from
 * @param[out] header The structure we want to store our information in
 * @return true, if read successful, false otherwise
 */
template<typename T>
inline bool ReadHeader(FILE* fp, T& header)
{
  unsigned int readLength = sizeof(T);

  // Load the information directly into our structure
  if (fread((void*)&header, 1, readLength, fp) != readLength)
  {
    return false;
  }

  return true;
}

/** Check whether the array passed in is the right size and matches the magic
 *  values defined to be at the start of a KTX file by the specification.*/
template<int BYTES_IN_SIGNATURE>
bool CheckFileIdentifier(const Byte * const signature)
{
  const unsigned signatureSize = BYTES_IN_SIGNATURE;
  const unsigned identifierSize = sizeof(FileIdentifier);
  DALI_COMPILE_TIME_ASSERT(signatureSize == identifierSize);
  const bool signatureGood = 0 == memcmp( signature, FileIdentifier, std::min( signatureSize, identifierSize ) );
  return signatureGood;
}

/**
 * @returns True if the argument is a GLES compressed texture format that we support.
 */
bool ValidInternalFormat(const unsigned format)
{
  unsigned candidateFormat = 0;
  for(unsigned iFormat = 0; (candidateFormat = KtxInternalFormats[iFormat]) != KTX_SENTINEL; ++iFormat)
  {
    if(format == candidateFormat)
    {
      return true;
    }
  }
  DALI_LOG_ERROR("Rejecting unsupported compressed format when loading compressed texture from KTX file: 0x%x.\n", format);
  return false;
}

/**
 * @returns The Pixel::Format Dali enum corresponding to the KTX internal format
 *          passed in, or Pixel::INVALID_PIXEL_FORMAT if the format is not valid.
 **/
bool ConvertPixelFormat(const uint32_t ktxPixelFormat, Dali::Pixel::Format& format)
{
  using namespace Dali::Pixel;
  switch(ktxPixelFormat)
  {
    case KTX_COMPRESSED_R11_EAC:
    {
      format = Dali::Pixel::COMPRESSED_R11_EAC;
      break;
    }
    case KTX_COMPRESSED_SIGNED_R11_EAC:
    {
      format = COMPRESSED_SIGNED_R11_EAC;
      break;
    }
    case KTX_COMPRESSED_RG11_EAC:
    {
      format = COMPRESSED_RG11_EAC;
      break;
    }
    case KTX_COMPRESSED_SIGNED_RG11_EAC:
    {
      format = COMPRESSED_SIGNED_RG11_EAC;
      break;
    }
    case KTX_COMPRESSED_RGB8_ETC2:
    {
      format = COMPRESSED_RGB8_ETC2;
      break;
    }
    case KTX_COMPRESSED_SRGB8_ETC2:
    {
      format = COMPRESSED_SRGB8_ETC2;
      break;
    }
    case KTX_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
    {
      format = COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2;
      break;
    }
    case KTX_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2:
    {
      format = COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2;
      break;
    }
    case KTX_COMPRESSED_RGBA8_ETC2_EAC:
    {
      format = COMPRESSED_RGBA8_ETC2_EAC;
      break;
    }
    case KTX_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
    {
      format = COMPRESSED_SRGB8_ALPHA8_ETC2_EAC;
      break;
    }
    // GLES 2 extension compressed formats:
    case KTX_ETC1_RGB8_OES:
    {
      format = COMPRESSED_RGB8_ETC1;
      break;
    }
    case KTX_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
    {
      format = COMPRESSED_RGB_PVRTC_4BPPV1;
      break;
    }

    default:
    {
       return false;
    }
  }
  return true;
}

bool LoadKtxHeader(FILE * const fp, unsigned int &width, unsigned int &height, KtxFileHeader &fileHeader)
{
  // Pull the bytes of the file header in as a block:
  if ( !ReadHeader(fp, fileHeader) )
  {
    return false;
  }
  width = fileHeader.pixelWidth;
  height = fileHeader.pixelHeight;

  if ( width > MAX_TEXTURE_DIMENSION || height > MAX_TEXTURE_DIMENSION )
  {
    return false;
  }

  // Validate file header contents meet our minimal subset:
  const bool signatureGood                            = CheckFileIdentifier<sizeof(fileHeader.identifier)>(fileHeader.identifier);
  const bool fileEndiannessMatchesSystemEndianness    = fileHeader.endianness == 0x04030201; // Magic number from KTX spec.
  const bool glTypeIsCompressed                       = fileHeader.glType == 0;
  const bool glTypeSizeCompatibleWithCompressedTex    = fileHeader.glTypeSize == 1;
  const bool glFormatCompatibleWithCompressedTex      = fileHeader.glFormat == 0;
  const bool glInternalFormatIsSupportedCompressedTex = ValidInternalFormat(fileHeader.glInternalFormat);
  // Ignore glBaseInternalFormat
  const bool textureIsNot3D                           = fileHeader.pixelDepth == 0 || fileHeader.pixelDepth == 1;
  const bool textureIsNotAnArray                      = fileHeader.numberOfArrayElements == 0 || fileHeader.numberOfArrayElements == 1;
  const bool textureIsNotACubemap                     = fileHeader.numberOfFaces == 0 || fileHeader.numberOfFaces == 1;
  const bool textureHasNoMipmapLevels                 = fileHeader.numberOfMipmapLevels == 0 || fileHeader.numberOfMipmapLevels == 1;
  const bool keyValueDataNotTooLarge                  = fileHeader.bytesOfKeyValueData <= MAX_BYTES_OF_KEYVALUE_DATA;

  const bool headerIsValid = signatureGood && fileEndiannessMatchesSystemEndianness && glTypeIsCompressed &&
                           glTypeSizeCompatibleWithCompressedTex && glFormatCompatibleWithCompressedTex &&
                           textureIsNot3D && textureIsNotAnArray && textureIsNotACubemap && textureHasNoMipmapLevels &&
                           glInternalFormatIsSupportedCompressedTex & keyValueDataNotTooLarge;
  if( !headerIsValid )
  {
     DALI_LOG_ERROR( "KTX file invalid or using unsupported features. Header tests: sig: %d, endian: %d, gl_type: %d, gl_type_size: %d, gl_format: %d, internal_format: %d, depth: %d, array: %d, faces: %d, mipmap: %d, vey-vals: %d.\n", 0+signatureGood, 0+fileEndiannessMatchesSystemEndianness, 0+glTypeIsCompressed, 0+glTypeSizeCompatibleWithCompressedTex, 0+glFormatCompatibleWithCompressedTex, 0+glInternalFormatIsSupportedCompressedTex, 0+textureIsNot3D, 0+textureIsNotAnArray, 0+textureIsNotACubemap, 0+textureHasNoMipmapLevels, 0+keyValueDataNotTooLarge);
  }

  // Warn if there is space wasted in the file:
  if( fileHeader.bytesOfKeyValueData > 0U )
  {
    DALI_LOG_WARNING("Loading of KTX file with key/value header data requested. This should be stripped in application asset/resource build.\n");
  }

  return headerIsValid;
}


} // unnamed namespace

// File loading API entry-point:
bool LoadKtxHeader( const ImageLoader::Input& input, unsigned int& width, unsigned int& height )
{
  KtxFileHeader fileHeader;
  FILE* const fp = input.file;

  bool ret = LoadKtxHeader(fp, width, height, fileHeader);
  return ret;
}

// File loading API entry-point:
bool LoadBitmapFromKtx( const ResourceLoadingClient& client, const ImageLoader::Input& input, Integration::Bitmap& bitmap )
{
  DALI_COMPILE_TIME_ASSERT( sizeof(Byte) == 1);
  DALI_COMPILE_TIME_ASSERT( sizeof(uint32_t) == 4);

  FILE* const fp = input.file;
  if( fp == NULL )
  {
    DALI_LOG_ERROR( "Null file handle passed to KTX compressed bitmap file loader.\n" );
    return false;
  }
  KtxFileHeader fileHeader;

  // Load the header info
  unsigned int width, height;

  if (!LoadKtxHeader(fp, width, height, fileHeader))
  {
      return false;
  }

  // Skip the key-values:
  const long int imageSizeOffset = sizeof(KtxFileHeader) + fileHeader.bytesOfKeyValueData;
  if(fseek(fp, imageSizeOffset, SEEK_SET))
  {
    DALI_LOG_ERROR( "Seek past key/vals in KTX compressed bitmap file failed.\n" );
    return false;
  }

  // Load the size of the image data:
  uint32_t imageByteCount = 0;
  if (fread((void*)&imageByteCount, 1, 4, fp) != 4)
  {
    DALI_LOG_ERROR( "Read of image size failed.\n" );
    return false;
  }
  // Sanity-check the image size:
  if( imageByteCount > MAX_IMAGE_DATA_SIZE ||
      // A compressed texture should certainly be less than 2 bytes per texel:
      imageByteCount > width * height * 2)
  {
    DALI_LOG_ERROR( "KTX file with too-large image-data field.\n" );
    return false;
  }

  Pixel::Format pixelFormat;
  const bool pixelFormatKnown = ConvertPixelFormat(fileHeader.glInternalFormat, pixelFormat);
  if(!pixelFormatKnown)
  {
    DALI_LOG_ERROR( "No internal pixel format supported for KTX file pixel format.\n" );
    return false;
  }

  // Load up the image bytes:
  PixelBuffer * const pixels = bitmap.GetCompressedProfile()->ReserveBufferOfSize( pixelFormat, width, height, (size_t) imageByteCount );
  if(!pixels)
  {
    DALI_LOG_ERROR( "Unable to reserve a pixel buffer to load the requested bitmap into.\n" );
    return false;
  }
  const size_t bytesRead = fread(pixels, 1, imageByteCount, fp);
  if(bytesRead != imageByteCount)
  {
    DALI_LOG_ERROR( "Read of image pixel data failed.\n" );
    return false;
  }

  return true;
}

} // namespace TizenPlatform

} // namespace Dali
