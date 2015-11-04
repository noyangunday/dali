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

#ifndef __DALI_ADAPTOR_TET_IMAGE_LOADERS_H_
#define __DALI_ADAPTOR_TET_IMAGE_LOADERS_H_

#include <dali/dali.h>
#include <dali/integration-api/bitmap.h>
#include "platform-abstractions/tizen/resource-loader/resource-loading-client.h"
#include "platform-abstractions/tizen/image-loaders/image-loader-input.h"

// Simple structure to close the file when finished with it.
struct AutoCloseFile
{
  AutoCloseFile( FILE *fp );
  ~AutoCloseFile();
  FILE* filePtr;
};

/// Structure to hold image details and the reference buffer.
struct ImageDetails
{
  /**
   * Normal Constructor.
   *
   * @param[in]  _name    The name of the image to open.  The reference buffer file should have the same name appended with ".buffer".
   * @param[in]  _width   The width of the image.
   * @param[in]  _height  The height of the image.
   */
  ImageDetails( const char * const _name, unsigned int _width, unsigned int _height );

  /**
   * Sometimes an image reports an incorrect size in the header than what it actually is. In such a
   * scenario, this constructor should be used.
   *
   * @param[in]  _name            The name of the image to open.  The reference buffer file should have the same name appended with ".buffer".
   * @param[in]  _width           The width of the image.
   * @param[in]  _height          The height of the image.
   * @param[in]  _reportedWidth   The reported width of the image by reading the header.
   * @param[in]  _reportedHeight  The reported height of the image by reading the header.
   */
  ImageDetails( const char * const _name, unsigned int _width, unsigned int _height, unsigned int _reportedWidth, unsigned int _reportedHeight );

  /**
   * Destructor
   */
  ~ImageDetails();


  std::string name;
  unsigned int width;
  unsigned int height;
  unsigned int reportedWidth;
  unsigned int reportedHeight;
  unsigned int refBufferSize;
  Dali::PixelBuffer* const refBuffer;

private:

  /**
   * Loads the reference buffer file.
   */
  void LoadBuffer();
};

/**
 * A structure storing the methods that should be called when reading an image's header and when
 * reading the bitmap from the image file.
 */
struct LoadFunctions
{
  typedef bool (*LoadBitmapFunction)( const Dali::TizenPlatform::ResourceLoadingClient& client, const Dali::TizenPlatform::ImageLoader::Input& input, Dali::Integration::Bitmap& );
  typedef bool (*LoadBitmapHeaderFunction)( const Dali::TizenPlatform::ImageLoader::Input& input, unsigned int& width, unsigned int& height );

  LoadFunctions( LoadBitmapHeaderFunction _header, LoadBitmapFunction _loader );
  LoadBitmapHeaderFunction header;
  LoadBitmapFunction loader;
};

// Helper method to test each image file.
/**
 * Use this method to test the header and and bitmap loading of each image.
 * The loaded bitmap is then checked with the reference bitmap in ImageDetails.
 *
 * @param[in]  image      The image details.
 * @param[in]  functions  The loader functions that need to be called.
 */
void TestImageLoading( const ImageDetails& image, const LoadFunctions& functions );

/**
 * Helper function which should be used when first creating a reference buffer file.
 * Set output file to a file in the /tmp/ directory e.g:
 *   DumpImageBufferToTempFile( "images/pattern.gif" , "/tmp/pattern.gif.buffer" );
 *
 * @param[in]  filename        The path of the image file.
 * @param[in]  targetFilename  The path of where the buffer should be written to.  This should ideally be in the "/tmp" folder.
 * @param[in]  functions       The loader functions to call.
 */
void DumpImageBufferToTempFile( std::string filename, std::string targetFilename, const LoadFunctions& functions );

#endif // __DALI_ADAPTOR_TET_IMAGE_LOADERS_H_
