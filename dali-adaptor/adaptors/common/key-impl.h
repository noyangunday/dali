#ifndef __DALI_KEY_IMPL_H__
#define __DALI_KEY_IMPL_H__

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

// INTERNAL INCLUDES
#include <key.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * Implementation of the Key matching
 */
namespace KeyLookup
{

struct KeyLookup
{
  const char* keyName;          ///< XF86 key name
  const Dali::KEY daliKeyCode;  ///< Dali key code
  const bool  deviceButton;     ///< Whether the key is from a button on the device
};

extern KeyLookup KeyLookupTable[];
extern const std::size_t KEY_LOOKUP_COUNT;

/**
 * @copydoc Dali::IsKey()
 */
bool IsKey( const Dali::KeyEvent& keyEvent, Dali::KEY daliKey );

/**
 * Check if a the given key name string is a button on the device itself.
 * @param keyName A pointer to the key name
 * @return true if the key is matched, false if not
 */
bool IsDeviceButton( const char* keyName );

/**
 * Get a key name from a dali key code.
 * @param daliKey The dali key code
 * @return The key name. NULL if the daliKey does not exist in the supported key lookup table.
 */
const char* GetKeyName( Dali::KEY daliKey );

} // namespace KeyLookup

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_KEY_IMPL_H__
