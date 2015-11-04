#ifndef __DALI_TIZEN_PLATFORM_LOADER_ICO_H__
#define __DALI_TIZEN_PLATFORM_LOADER_ICO_H__

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

#include <cstdio>
#include "image-loader-input.h"

namespace Dali
{

namespace Integration
{
  class Bitmap;
}

namespace TizenPlatform
{

class ResourceLoadingClient;

namespace Ico
{
//00 00 01 00 01 00 20 20
const unsigned char MAGIC_BYTE_1 = 0x00;
const unsigned char MAGIC_BYTE_2 = 0x00;
}

bool LoadBitmapFromIco( const ResourceLoadingClient& client, const ImageLoader::Input& input, Integration::Bitmap& bitmap );

bool LoadIcoHeader( const ImageLoader::Input& input, unsigned int& width, unsigned int& height );

}

}
#endif
