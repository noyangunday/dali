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

// FUNCTION HEADER
#include "tizen-logging.h"

// EXTERNAL INCLUDES
#ifndef DALI_PROFILE_UBUNTU
#include <dlog.h>
#else // DALI_PROFILE_UBUNTU
#include <cstdio>
#endif

namespace Dali
{

namespace TizenPlatform
{

void LogMessage(Dali::Integration::Log::DebugPriority level, std::string& message)
{
  const char* DALI_TAG = "DALI";

#ifndef DALI_PROFILE_UBUNTU
  switch(level)
  {
    case Dali::Integration::Log::DebugInfo:
      LOG(LOG_INFO, DALI_TAG, "%s", message.c_str());
      break;
    case Dali::Integration::Log::DebugWarning:
      LOG(LOG_WARN, DALI_TAG, "%s", message.c_str());
      break;
    case Dali::Integration::Log::DebugError:
      LOG(LOG_ERROR, DALI_TAG, "%s", message.c_str());
      break;
    default:
      LOG(LOG_DEFAULT, DALI_TAG, "%s", message.c_str());
      break;
  }
#else // DALI_PROFILE_UBUNTU
  const char *format = NULL;
  switch(level)
  {
    case Dali::Integration::Log::DebugInfo:
      format = "\e[1;34mINFO:\e[21m %s: %s\e[0m";
      break;
    case Dali::Integration::Log::DebugWarning:
      format = "\e[1;33mWARN:\e[21m %s: %s\e[0m";
      break;
    case Dali::Integration::Log::DebugError:
      format = "\e[1;91mERROR:\e[21m %s: %s\e[0m";
      break;
    default:
      format = ":\e[21m %s: %s\e[0m";
      break;
  }
  printf(format, DALI_TAG, message.c_str());
#endif // DALI_PROFILE_UBUNTU
}

} // namespace TizenPlatform

} // namespace Dali
