#ifndef _DALI_INTERNAL_PLATFORM_ATOMICS_H_
#define _DALI_INTERNAL_PLATFORM_ATOMICS_H_

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

/*
 * atomics.h
 *
 * Interface for atomic memory operations.
 * There may be platform-specific versions of this file in other directories.
 */

namespace Dali
{
namespace Internal
{

/**
 * @brief Atomic write to an aligned memory location in cacheable memory.
 *
 * For common platforms with coherent caches such as ARM mpcore and Intel CPUs,
 * a cacheline can be in a writeable state in the L1 cache of exactly one core
 * at a time. Therefore, a write to a location that does not require a read /
 * modify / write cycle or cross a cacheline boundary is  automatically
 * atomic.
 *
 * @param address A pointer to a location in a cacheable memory region that is
 *        aligned to a 4 byte boundary.
 * @param value A value to assign to the memory location in address.
 */
inline void AtomicWriteToCacheableAlignedAddress( volatile uint32_t * const address, const uint32_t value )
{
  DALI_ASSERT_DEBUG( ptrdiff_t(address) % 4 == 0 && "Address must be 32 bit aligned" );
  *address = value;
}

/**
 * @brief Atomic read from an aligned memory location in cacheable memory.
 *
 * For common platforms with coherent caches such as ARM mpcore and Intel CPUs,
 * a cacheline can be in a writeable state in the L1 cache of exactly one core
 * at a time. Therefore, a read a location that does not cross a cacheline
 * boundary is automatically atomic.
 *
 * @param  address A pointer to a location in a cacheable memory region that is
 *         aligned to a 4 byte boundary.
 * @return The value stored at the memory location in address.
 */
inline uint32_t AtomicReadFromCacheableAlignedAddress( const volatile uint32_t * const address )
{
  DALI_ASSERT_DEBUG( ptrdiff_t(address) % 4 == 0 && "Address must be 32 bit aligned" );
  return *address;
}

} /* namespace Internal */
} /* namespace Dali */

#endif /* _DALI_INTERNAL_PLATFORM_ATOMICS_H_ */
