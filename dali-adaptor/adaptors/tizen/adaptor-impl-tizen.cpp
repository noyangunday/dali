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
#include <adaptor-impl.h>

// EXTERNAL INCLUDES
#ifdef OVER_TIZEN_SDK_2_2
#include <app.h>
#endif

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

void Adaptor::GetDataStoragePath( std::string& path)
{
  path = "";
#ifdef OVER_TIZEN_SDK_2_2
  char *pathInt = app_get_data_path();
  if ( pathInt )
  {
    path = pathInt;
    free( pathInt );
  }
#endif
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
