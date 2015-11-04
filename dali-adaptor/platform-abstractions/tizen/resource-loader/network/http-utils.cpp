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

// CLASS HEADER
#include "http-utils.h"

// EXTERNAL INCLUDES
#include <cstring>

namespace Dali
{

namespace TizenPlatform
{

namespace
{
const unsigned int MIN_HTTP_URL_LENGTH = 12; // assume we have a least http://xx/yy
const char HTTP_URL[] = "http://";
const char HTTPS_URL[] = "https://";
}

bool Network::IsHttpUrl( const std::string& path )
{
 if( path.size() <= MIN_HTTP_URL_LENGTH )
 {
   return false;
 }

 if( ( strncasecmp( path.c_str(), HTTP_URL,  sizeof(HTTP_URL)  -1 ) == 0 )  ||
     ( strncasecmp( path.c_str(), HTTPS_URL, sizeof(HTTPS_URL) -1 ) == 0 ) )
 {
   return true;
 }

 return false;
}

} // TizenPlatform

} // Dali
