#ifndef __DALI_INTERNAL_ADAPTOR_NETWORK_PERFORMANCE_PROTOCOL_H__
#define __DALI_INTERNAL_ADAPTOR_NETWORK_PERFORMANCE_PROTOCOL_H__

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

namespace Dali
{

namespace PerformanceProtocol
{

const unsigned int MAX_COMMAND_STRING_LENGTH = 256;  ///< maximum length of a command including null terminator

/**
 * @brief List of commands id's
 */
enum CommandId
{
  HELP_MESSAGE            = 0, ///<  help message
  ENABLE_METRIC           = 1, ///< enable metric
  DISABLE_METRIC          = 2, ///< disable metric
  LIST_METRICS_AVAILABLE  = 3, ///< list  metrics that are available
  ENABLE_TIME_MARKER_BIT_MASK = 4, ///< bit mask of time markers to enable
  SET_PROPERTIES            = 5, ///< set property
  DUMP_SCENE_GRAPH          = 6, ///< dump the scene graph
  UNKNOWN_COMMAND           = 4096
};


typedef char CommandString[ MAX_COMMAND_STRING_LENGTH ];

/**
 * @brief given a command id, get the command string
 * @param[in] commandId command id
 * @param[out] commandString command string
 * @return true on success, false on failure
 */
bool GetCommandString( CommandId commandId, CommandString& commandString );

/**
 * @brief given a command string, get the command id and an integer parameter if it exists
 * @param[in] commandString command string
 * @param[in] lengthInBytes length of the string
 * @param[out] commandId command id
 * @param[out] intParam integer parameter
 * @param[out] stringParam string parameter
 * @param true on success, false on failure
 */
bool GetCommandId( const char* const commandString,
                   unsigned int lengthInBytes,
                   CommandId& commandId,
                   unsigned int& intParam,
                   std::string& stringParam);

/**
 * @return the protocol help message for console users
 */
const char* const GetHelpMessage();

} // namespace PerformanceProtocol

} // namespace Dali

#endif //__DALI_INTERNAL_ADAPTOR_NETWORK_PERFORMANCE_PROTOCOL_H__
