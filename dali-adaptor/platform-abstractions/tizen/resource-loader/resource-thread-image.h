#ifndef __DALI_TIZEN_PLATFORM_RESOURCE_THREAD_IMAGE_H__
#define __DALI_TIZEN_PLATFORM_RESOURCE_THREAD_IMAGE_H__

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
// EXTERNAL INCLUDES
#include <dali/integration-api/resource-cache.h>
#include <dali/integration-api/resource-types.h>

// INTERNAL INCLUDES
#include "resource-thread-base.h"

namespace Dali
{

namespace TizenPlatform
{

class ResourceThreadImage : public ResourceThreadBase
{
public:
  /**
   * Constructor
   * @param[in] resourceLoader A reference to the ResourceLoader
   */
  ResourceThreadImage( ResourceLoader& resourceLoader );

  /**
   * Destructor
   */
  virtual ~ResourceThreadImage();


private:
  /**
   * @copydoc ResourceThreadBase::Load
   */
  virtual void Load(const Integration::ResourceRequest& request);

  /**
   * @copydoc ResourceThreadBase::Download
   */
  virtual void Download(const Integration::ResourceRequest& request);

  /**
   * @copydoc ResourceThreadBase::Decode
   */
  virtual void Decode(const Integration::ResourceRequest& request);

  /**
   * Download a requested image into a memory buffer.
   * @param[in] request  The requested resource/file url and attributes
   * @param[out] dataBuffer  A memory buffer object to be written with downloaded image data.
   * @param[out] dataSize  The size of the memory buffer.
   */
  bool DownloadRemoteImageIntoMemory(const Integration::ResourceRequest& request, Dali::Vector<uint8_t>& dataBuffer, size_t& dataSize);

  /**
   * Load a requested image from a local file.
   * @param[in] request  The requested resource/file url and attributes
   */
  void LoadImageFromLocalFile(const Integration::ResourceRequest& request);

  /**
   * Decode a requested image from a memory buffer.
   * @param[in] blobBytes  A pointer to the memory buffer containig the requested image data.
   * @param[in] blobSize  The size of the memory buffer containing the requested image data.
   * @param[in] request  The requested resource/file url and attributes
   */
  void DecodeImageFromMemory(void* blobBytes, size_t blobSize, const Integration::ResourceRequest& request);
}; // class ResourceThreadImage

} // namespace TizenPlatform

} // namespace Dali

#endif // __DALI_TIZEN_PLATFORM_RESOURCE_THREAD_IMAGE_H__
