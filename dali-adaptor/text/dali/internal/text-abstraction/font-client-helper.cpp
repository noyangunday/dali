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

// CLASS  HEADER
#include "font-client-helper.h"

// INTERNAL INCLUDES

#include <dali/integration-api/debug.h>

namespace
{
#if defined(DEBUG_ENABLED)
Dali::Integration::Log::Filter* gLogFilter = Dali::Integration::Log::Filter::New(Debug::NoLogging, false, "LOG_FONT_CLIENT");
#endif
}

namespace Dali
{

namespace TextAbstraction
{

namespace Internal
{

int ValueToIndex( int value, const int* const table, unsigned int maxIndex )
{
  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "-->FontClient::Plugin::ValueToIndex value(%d)\n", value);

  if( ( NULL == table ) ||
      ( value <= table[0] ) )
  {
    return 0;
  }

  if( value >= table[maxIndex] )
  {
    return maxIndex;
  }

  for( unsigned int index = 0u; index < maxIndex; )
  {
    const int v1 = table[index];
    const unsigned int indexPlus = ++index;
    const int v2 = table[indexPlus];
    if( ( v1 < value ) && ( value <= v2 ) )
    {
      const int result = ( ( value - v1 ) < ( v2 - value ) ) ? index : indexPlus;
      DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::ValueToIndex result(%d)\n",  result );
      return result;
    }
  }

  DALI_LOG_INFO( gLogFilter, Debug::Verbose, "FontClient::Plugin::ValueToIndex exit 0 <-- \n");

  return 0;
}

} // namespace Internal

} // namespace TextAbstraction

} // namespace Dali
