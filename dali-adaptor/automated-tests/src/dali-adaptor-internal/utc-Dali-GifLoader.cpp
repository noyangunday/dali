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

#include <iostream>
#include <stdlib.h>
#include <dali-test-suite-utils.h>

#include "platform-abstractions/tizen/image-loaders/loader-gif.h"
#include "image-loaders.h"

using namespace Dali;

namespace {
static const LoadFunctions GifLoaders( TizenPlatform::LoadGifHeader, TizenPlatform::LoadBitmapFromGif );
}


void gif_loader_startup(void)
{
}

void gif_loader_cleanup(void)
{
}

int UtcDaliGifLoaderInterlaced(void)
{
  ImageDetails interlaced( TEST_IMAGE_DIR "/interlaced.gif", 365u, 227u );
  TestImageLoading( interlaced, GifLoaders );
  END_TEST;
}

int UtcDaliGifLoaderErrorBits(void)
{
  ImageDetails errorBits( TEST_IMAGE_DIR "/error-bits.gif", 534u, 749u, 1280u, 1024u );
  TestImageLoading( errorBits, GifLoaders );
  END_TEST;
}

int UtcDaliGifLoaderPattern(void)
{
  ImageDetails pattern( TEST_IMAGE_DIR "/pattern.gif", 600u, 600u );
  TestImageLoading( pattern, GifLoaders );
  END_TEST;
}

int UtcDaliGifLoaderTransparency(void)
{
  ImageDetails transparency( TEST_IMAGE_DIR "/transparency.gif", 320u, 280u );
  TestImageLoading( transparency, GifLoaders );
  END_TEST;
}
