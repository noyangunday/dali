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
 */

// EXTERNAL INCLUDES
#include <string>

// INTERNAL INCLUDES
#include "bitmap-loader-impl.h"
#include "image-loaders/image-loader.h"

namespace Dali
{
namespace Internal
{

IntrusivePtr<BitmapLoader> BitmapLoader::New(const std::string& url,
                                             ImageDimensions size,
                                             FittingMode::Type fittingMode,
                                             SamplingMode::Type samplingMode,
                                             bool orientationCorrection)
{
  IntrusivePtr<BitmapLoader> internal = new BitmapLoader( url, size, fittingMode, samplingMode, orientationCorrection );
  return internal;
}

BitmapLoader::BitmapLoader(const std::string& url,
             ImageDimensions size,
             FittingMode::Type fittingMode,
             SamplingMode::Type samplingMode,
             bool orientationCorrection)
: mResourceType( size, fittingMode, samplingMode, orientationCorrection ),
  mBitmap(NULL),
  mUrl(url),
  mIsLoaded( false )
{
}

BitmapLoader::~BitmapLoader()
{
}

void BitmapLoader::Load()
{
  IntrusivePtr<Dali::RefObject> resource = TizenPlatform::ImageLoader::LoadResourceSynchronously( mResourceType, mUrl );

  mBitmap = static_cast<Integration::Bitmap*>(resource.Get());
  mIsLoaded = true;
}

bool BitmapLoader::IsLoaded()
{
  return mIsLoaded;
}

unsigned char* BitmapLoader::GetPixelData() const
{
  if( mIsLoaded )
  {
    return mBitmap->GetBuffer();
  }

  return NULL;
}

unsigned int BitmapLoader::GetImageHeight() const
{
  if( mIsLoaded )
  {
    return mBitmap->GetImageHeight();
  }

  return 0u;
}

unsigned int BitmapLoader::GetImageWidth() const
{
  if( mIsLoaded )
  {
    return mBitmap->GetImageWidth();
  }

  return 0u;
}

Pixel::Format BitmapLoader::GetPixelFormat() const
{
  if( mIsLoaded )
  {
    return mBitmap->GetPixelFormat();
  }

  return Pixel::RGBA8888;
}

} // namespace Internal
} // namespace Dali
