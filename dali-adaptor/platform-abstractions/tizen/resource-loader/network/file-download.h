#ifndef __DALI_TIZEN_PLATFORM_NETWORK_FILE_DOWNLOAD_H__
#define __DALI_TIZEN_PLATFORM_NETWORK_FILE_DOWNLOAD_H__

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
#include <dali/public-api/common/dali-vector.h>
#include <string>
#include <stdint.h> // uint8

namespace Dali
{

namespace TizenPlatform
{

namespace Network
{

/**
 * Download a requested file into a memory buffer.
 * Threading notes: This function can be called from multiple threads, however l
 * we must explicitly call curl_global_init() from a single thread before using curl
 * as the global function calls are not thread safe.
 *
 * @param[in] url The requested file url
 * @param[out] dataBuffer  A memory buffer object to be written with downloaded file data.
 * @param[out] dataSize  The size of the memory buffer.
 * @param[in] maximumAllowedSize The maxmimum allowed file size in bytes to download. E.g. for an Image file this may be 50 MB
 * @return true on success, false on failure
 *
 */
bool DownloadRemoteFileIntoMemory( const std::string& url,
                                   Dali::Vector<uint8_t>& dataBuffer,
                                   size_t& dataSize,
                                   size_t maximumAllowedSizeBytes );

} // namespace Network

} // namespace TizenPlatform

} // namespace Dali

#endif // __DALI_TIZEN_PLATFORM_RESOURCE_THREAD_IMAGE_H__
