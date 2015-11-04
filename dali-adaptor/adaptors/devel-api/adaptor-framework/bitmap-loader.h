#ifndef __DALI_BITMAP_LOADER_H__
#define __DALI_BITMAP_LOADER_H__

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

// EXTERNAL INCLUDES
#include <string>
#include <dali/public-api/common/dali-common.h>
#include <dali/public-api/images/pixel.h>
#include <dali/public-api/images/image-operations.h>
#include <dali/public-api/object/base-handle.h>

namespace Dali
{
namespace Internal
{
class BitmapLoader;
}

/**
 * @brief The BitmapLoader class is used to load bitmap from the URL synchronously.
 *
 * As the loading is synchronous, it will block the loop whilst executing.
 * Therefore, it should be used sparingly in the main event thread, and better to be called in the worker thread.
 * The Load() API is thread safe, it can be called from any thread without changing the state of DALI.
 */
class DALI_IMPORT_API BitmapLoader : public BaseHandle
{
public:

  /**
   * @brief Create an initialized bitmap loader.
   *
   * By calling Load(), the synchronous loading is started immediately.
   *
   * @param [in] url The URL of the image file to load.
   * @param [in] size The width and height to fit the loaded image to.
   * @param [in] fittingMode The method used to fit the shape of the image before loading to the shape defined by the size parameter.
   * @param [in] samplingMode The filtering method used when sampling pixels from the input image while fitting it to desired size.
   * @param [in] orientationCorrection Reorient the image to respect any orientation metadata in its header.
   */
  static BitmapLoader New( const std::string& url,
                           ImageDimensions size = ImageDimensions( 0, 0 ),
                           FittingMode::Type fittingMode = FittingMode::DEFAULT,
                           SamplingMode::Type samplingMode = SamplingMode::BOX_THEN_LINEAR,
                           bool orientationCorrection = true);

  /**
   * @brief Create an empty handle.
   *
   * Use BitmapLoader::New() to create an initialized object.
   */
  BitmapLoader();

  /**
   * Destructor
   */
  ~BitmapLoader();

  /**
   * @brief This copy constructor is required for (smart) pointer semantics.
   *
   * @param [in] handle A reference to the copied handle
   */
  BitmapLoader(const BitmapLoader& handle);

  /**
   * @brief This assignment operator is required for (smart) pointer semantics.
   *
   * @param [in] rhs  A reference to the copied handle
   * @return A reference to this
   */
  BitmapLoader& operator=(const BitmapLoader& rhs);

public:

  /**
   * @brief Start the synchronous loading.
   */
  void Load();

  /**
   * @brief Query whether the image is loaded.
   *
   * @reture true if the image is loaded, false otherwise.
   */
  bool IsLoaded();

  /**
   * Get the raw pixel data.
   * @return The pixel data. Use the GetHeight(), GetWidth(), GetStride() and GetPixelFormat() methods
   * to decode the data.
   */
  unsigned char* GetPixelData() const;

  /**
   * Get the buffer height in pixels
   * @return the height of the buffer in pixels
   */
  unsigned int GetImageHeight() const;

  /**
   * Get the buffer width in pixels
   * @return the width of the buffer in pixels
   */
  unsigned int GetImageWidth() const;

  /**
   * Get the pixel format of the loaded bitmap.
   */
  Pixel::Format GetPixelFormat() const;

public: // Not intended for application developers

  explicit DALI_INTERNAL BitmapLoader(Internal::BitmapLoader*);
};

} // Dali

#endif // __DALI_BITMAP_LOADER_H__
