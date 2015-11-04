#ifndef __DALI_TIZEN_PLATFORM_CAPABILITIES_H__
#define __DALI_TIZEN_PLATFORM_CAPABILITIES_H__

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

#include <dali/public-api/math/math-utils.h>

namespace Dali
{

namespace TizenPlatform
{

/**
 * Returns true if non power of two textures are supported.
 */
inline bool SupportsNonPowerOfTwoTextures()
{
#ifdef NON_POWER_OF_TWO_TEXTURES
  return true; // All current devices and desktop builds have NPOT
#else
#error "NPOT are standard in GLES 2.0 if mipmaps are not used, they are standard with mipmaps and no restrictions in GLES 3.0, requiring them simplifies image handling code."
  return false;
#endif
}

/**
 * Returns the size that a textures dimension should have for a specific image size.
 */
inline unsigned int GetTextureDimension( unsigned int size )
{
#ifdef NON_POWER_OF_TWO_TEXTURES
  return size;
#else
#error "NPOT are standard in GLES 2.0 if mipmaps are not used, they are standard with mipmaps and no restrictions in GLES 3.0, requiring them simplifies image handling code."
  return NextPowerOfTwo( size );
#endif
}

} // namespace TizenPlatform

} // namespace Dali

#endif // __DALI_TIZEN_PLATFORM_CAPABILITIES_H__
