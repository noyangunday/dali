#ifndef __DALI_TIZEN_PLATFORM_NETWORK_UTILS_H__
#define __DALI_TIZEN_PLATFORM_NETWORK_UTILS_H__

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
#include <string>

namespace Dali
{

namespace TizenPlatform
{

namespace Network
{

/**
 * @brief Tests if a string starts with either http:// or https://
 * @param[in] path string
 * @return true if the path is a http url
 */
bool IsHttpUrl( const std::string& path );

} // Network

} // namespace TizenPlatform

} // namespace Dali

#endif // __DALI_TIZEN_PLATFORM_NETWORK_UTILS_H__
