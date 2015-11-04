#ifndef __DALI_STYLE_CHANGE_H__
#define __DALI_STYLE_CHANGE_H__

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

// EXTERNAL INCLUDES
#include <string>
#include <dali/public-api/common/dali-common.h>

namespace Dali
{
/**
 * @addtogroup dali_adaptor_framework
 * @{
 */

namespace StyleChange
{

enum Type
{
  DEFAULT_FONT_CHANGE,      ///< Denotes that the default font has changed.
  DEFAULT_FONT_SIZE_CHANGE, ///< Denotes that the default font size has changed.
  THEME_CHANGE              ///< Denotes that the theme has changed.
};

} // namespace StyleChange

/**
 * @}
 */
} // namespace Dali

#endif // __DALI_STYLE_CHANGE_H__
