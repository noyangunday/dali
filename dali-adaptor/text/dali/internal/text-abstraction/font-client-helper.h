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

#ifndef __DALI_INTERNAL_TEXT_ABSTRACTION_FONT_CLIENT_HELPER_H__
#define __DALI_INTERNAL_TEXT_ABSTRACTION_FONT_CLIENT_HELPER_H__

namespace Dali
{

namespace TextAbstraction
{

namespace Internal
{

/**
 * @brief Retrieves a table index for a given value.
 *
 * @param[in] value The value.
 * @param[in] table The table.
 * @param[in] maxIndex The maximum valid index of the table.
 *
 * @return The index to the closest available value
 */
int ValueToIndex( int value, const int* const table, unsigned int maxIndex );

} // Internal

} // TextAbstraction

} // Dali

#endif // __DALI_INTERNAL_TEXT_ABSTRACTION_FONT_CLIENT_HELPER_H__
