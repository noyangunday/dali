#ifndef __DALI_WINDOW_EXTENSIONS_H__
#define __DALI_WINDOW_EXTENSIONS_H__

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
#include <dali/public-api/common/dali-common.h>

namespace Dali
{
class Window;

namespace WindowExtensions
{

/**
 * @brief Set whether the effect will enable or not.
 *
 * The effect will be shown when the application is launched, quit, shown and hiden.
 *
 * @note This function is only specified by tizen.
 *
 * @param[in] window The window to set.
 * @param[in] enable True if the effect is enabled.
 */
DALI_IMPORT_API void EnableEffect( Window window, bool enable );

/**
 * @brief Retrieve whether the effect is enabled or not.
 *
 * @note This function is only specified by tizen.
 *
 * @param[in] window The window to set.
 * @return True if the effect is enabled.
 */
DALI_IMPORT_API bool IsEffectEnabled( Window window );

} // namespace WindowExtensions

} // namespace Dali

#endif // __DALI_WINDOW_EXTENSIONS_H__
