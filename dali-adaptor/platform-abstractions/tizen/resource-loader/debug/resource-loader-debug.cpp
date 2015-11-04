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

#include "resource-loader-debug.h"

#if defined(DEBUG_ENABLED)

namespace Dali
{
namespace TizenPlatform
{
using namespace Dali::Integration;

/**
 * Filter for resource loader debug. Change levels here to turn on debugging
 */
Debug::Filter* gLoaderFilter = Debug::Filter::New(Debug::Concise, false, "LOG_RESOURCE_LOADER");

} //TizenPlatform
} //Dali

#endif
