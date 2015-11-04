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

// CLASS HEADER
#include "indicator-buffer.h"

namespace Dali
{
namespace Internal
{
namespace Adaptor
{

IndicatorBuffer::IndicatorBuffer( Adaptor* adaptor, unsigned int width, unsigned int height, Pixel::Format pixelFormat )
: mAdaptor(adaptor),
  mImageWidth(width),
  mImageHeight(height),
  mPixelFormat(pixelFormat)
{
  DALI_ASSERT_ALWAYS( adaptor );

  // Use BitmapImage when SharedGlBuffer extension is unavailable
  mBitmapBuffer = new NativeBitmapBuffer( adaptor, mImageWidth, mImageHeight, mPixelFormat );
  mNativeImage = mBitmapBuffer;
}

bool IndicatorBuffer::UpdatePixels( const unsigned char *src, size_t size )
{
  // Use double buffered bitmap when SharedGlBuffer extension is unavailable
  mBitmapBuffer->Write( src, size );
  return true;
}

NativeImageInterface& IndicatorBuffer::GetNativeImage() const
{
  DALI_ASSERT_DEBUG(mNativeImage.Get());
  return *mNativeImage;
}

void IndicatorBuffer::SetAdaptor( Adaptor* adaptor )
{
  mAdaptor = adaptor;
}

} // Adaptor
} // Internal
} // Dali
