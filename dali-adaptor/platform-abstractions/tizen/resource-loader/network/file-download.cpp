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

// HEADER
#include "file-download.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <curl/curl.h>

// INTERNAL INCLUDES
#include "portable/file-closer.h"


using namespace Dali::Integration;

namespace Dali
{

namespace TizenPlatform
{

namespace // unnamed namespace
{

const int CONNECTION_TIMEOUT_SECONDS( 30L );
const long VERBOSE_MODE = 0L;                // 0 == off, 1 == on
const long CLOSE_CONNECTION_ON_ERROR = 1L;   // 0 == off, 1 == on
const long EXCLUDE_HEADER = 0L;
const long INCLUDE_HEADER = 1L;
const long INCLUDE_BODY = 0L;
const long EXCLUDE_BODY = 1L;

void ConfigureCurlOptions( CURL* curl_handle, const std::string& url )
{
  curl_easy_setopt( curl_handle, CURLOPT_URL, url.c_str() );
  curl_easy_setopt( curl_handle, CURLOPT_VERBOSE, VERBOSE_MODE );

  // CURLOPT_FAILONERROR is not fail-safe especially when authentication is involved ( see manual )
  curl_easy_setopt( curl_handle, CURLOPT_FAILONERROR, CLOSE_CONNECTION_ON_ERROR );
  curl_easy_setopt( curl_handle, CURLOPT_CONNECTTIMEOUT, CONNECTION_TIMEOUT_SECONDS );
  curl_easy_setopt( curl_handle, CURLOPT_HEADER, INCLUDE_HEADER );
  curl_easy_setopt( curl_handle, CURLOPT_NOBODY, EXCLUDE_BODY );
}

// Without a write function or a buffer (file descriptor) to write to, curl will pump out
// header/body contents to stdout
size_t DummyWrite(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  return size * nmemb;
}


bool DownloadFile( CURL* curl_handle,
                   const std::string& url,
                   Dali::Vector<uint8_t>& dataBuffer,
                   size_t& dataSize,
                   size_t maximumAllowedSizeBytes )
{
  CURLcode res( CURLE_OK );
  double size(0);

  // setup curl to download just the header so we can extract the content length
  ConfigureCurlOptions( curl_handle, url );

  curl_easy_setopt( curl_handle, CURLOPT_WRITEFUNCTION, DummyWrite);

  // perform the request to get the header
  res = curl_easy_perform( curl_handle );

  if( res != CURLE_OK)
  {
    DALI_LOG_WARNING( "Failed to download http header for \"%s\" with error code %d\n", url.c_str(), res );
    return false;
  }

  // get the content length, -1 == size is not known
  curl_easy_getinfo( curl_handle,CURLINFO_CONTENT_LENGTH_DOWNLOAD , &size );

  if( size < 1 )
  {
    DALI_LOG_WARNING( "Header missing content length \"%s\" \n", url.c_str() );
    return false;
  }
  if( size >= maximumAllowedSizeBytes )
  {
    DALI_LOG_WARNING( "File content length %f > max allowed %zu \"%s\" \n", size, maximumAllowedSizeBytes, url.c_str() );
    return false;
  }

  dataSize = static_cast<size_t>( size );

  dataBuffer.Resize( dataSize );

  // create
  Dali::Internal::Platform::FileCloser fileCloser( static_cast<void*>(&dataBuffer[0]), dataSize, "wb" );
  FILE* dataBufferFilePointer = fileCloser.GetFile();
  if( NULL != dataBufferFilePointer )
  {
    // we only want the body which contains the file data
    curl_easy_setopt( curl_handle, CURLOPT_HEADER, EXCLUDE_HEADER );
    curl_easy_setopt( curl_handle, CURLOPT_NOBODY, INCLUDE_BODY );

    // disable the write callback, and get curl to write directly into our data buffer
    curl_easy_setopt( curl_handle, CURLOPT_WRITEFUNCTION, NULL );
    curl_easy_setopt( curl_handle, CURLOPT_WRITEDATA, dataBufferFilePointer );

    // synchronous request of the body data
    res = curl_easy_perform( curl_handle );

    if( CURLE_OK != res )
    {
      DALI_LOG_WARNING( "Failed to download image file \"%s\" with error code %d\n", url.c_str(), res );
      return false;
    }
  }
  return true;
}
} // unnamed namespace



bool Network::DownloadRemoteFileIntoMemory( const std::string& url,
                                            Dali::Vector<uint8_t>& dataBuffer,
                                            size_t& dataSize,
                                            size_t maximumAllowedSizeBytes )
{
  if( url.empty() )
  {
    DALI_LOG_WARNING("empty url requested \n");
    return false;
  }

  // start a libcurl easy session, this internally calls curl_global_init, if we ever have more than one download
  // thread we need to explicity call curl_global_init() on startup from a single thread.

  CURL* curl_handle = curl_easy_init();

  bool result = DownloadFile( curl_handle, url, dataBuffer,  dataSize, maximumAllowedSizeBytes);

  // clean up session
  curl_easy_cleanup( curl_handle );

  return result;
}


} // namespace TizenPlatform

} // namespace Dali
