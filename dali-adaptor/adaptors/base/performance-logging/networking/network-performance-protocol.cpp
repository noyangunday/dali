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

// INTERNAL INCLUDES

// CLASS HEADER
#include "network-performance-protocol.h"

// EXTERNAL INCLUDES
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

namespace Dali
{

namespace PerformanceProtocol
{

namespace
{

/**
 * Command parameter type
 */
enum PARAMETER_TYPE
{
  NO_PARAMS,
  UNSIGNED_INT,
  STRING
};

/**
 * Command information structure
 */
struct CommandInfo
{
  CommandId     cmdId;
  CommandString cmdString;
  PARAMETER_TYPE paramType;
};

/**
 * Command lookup table
 */
CommandInfo CommandLookup[]=
{
  {  HELP_MESSAGE               , "help"               ,NO_PARAMS     },
  {  ENABLE_METRIC              , "enable_metric"      ,UNSIGNED_INT  },
  {  DISABLE_METRIC             , "disable_metric"     ,UNSIGNED_INT  },
  {  LIST_METRICS_AVAILABLE     , "list_metrics"       ,NO_PARAMS     },
  {  ENABLE_TIME_MARKER_BIT_MASK, "set_marker",         UNSIGNED_INT  },
  {  DUMP_SCENE_GRAPH           , "dump_scene"         ,NO_PARAMS     },
  {  SET_PROPERTIES             , "set_properties"     ,STRING        },
  {  UNKNOWN_COMMAND            , "unknown"            ,NO_PARAMS     }
};
const unsigned int CommandLookupLength = sizeof( CommandLookup ) /sizeof( CommandInfo );

#define GREEN  "\033[01;32m"
#define NORMAL  "\e[m"
#define PARAM "\033[22;32m"
#define YELLOW "\033[01;33m"

const char* const helpMsg =
    YELLOW
    "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
    "  Dali performance console                           \n"
    "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" NORMAL
    GREEN " list_metrics " NORMAL " - list available metrics\n"
    GREEN " enable_metric " PARAM " metricId" NORMAL " - enable a metric \n"
    GREEN " disable_metric "PARAM " metricId" NORMAL " - disable a metric\n\n"
    GREEN " set_marker " PARAM " value " NORMAL "-output Dali markers\n"
    "            : Bit 0  = V_SYNC (1)\n"
    "            : Bit 1  = Update task (2)\n"
    "            : Bit 2  = Render task (4) \n"
    "            : Bit 3  = Event Processing task (8)\n"
    "            : Bit 4  = SwapBuffers (16)\n"
    "            : Bit 5  = Life cycle events  (32)\n"
    "            : Bit 6  = Resource event (64)\n"
    "\n"
    GREEN" set_properties "NORMAL " - set an actor property command. Format:\n\n"
    GREEN" set_properties "PARAM"|ActorIndex;Property;Value|" NORMAL ", e.g: \n"
    GREEN" set_properties " PARAM "|178;Size;[ 144.0, 144.0, 144.0 ]|178;Color;[ 1.0, 1,0, 1.0 ]|\n"
    "\n"
    GREEN " dump_scene" NORMAL " - dump the current scene in json format\n";

} // un-named namespace

bool GetCommandId( const char* const commandString, unsigned int lengthInBytes, CommandId& commandId, unsigned int& intParam, std::string& stringParam  )
{
  commandId = UNKNOWN_COMMAND;
  intParam = 0;

  // the command list is small so just do a O(n) search for the commandID.
  for( unsigned int i = 0 ; i < CommandLookupLength; ++i )
  {
    if( strncmp( commandString, CommandLookup[i].cmdString ,strlen(CommandLookup[i].cmdString  )) == 0 )
    {
      commandId = CommandLookup[i].cmdId;

      // if the command has a parameter read it
      if( CommandLookup[i].paramType ==  UNSIGNED_INT)
      {
        int count = sscanf(commandString,"%*s %d",&intParam);
        if( count != 1 )
        {
          // missing parameter
          return false;
        }
      }
      else if (CommandLookup[i].paramType == STRING )
      {
        char* charParam( NULL );
        // allocates the character array
        int count = sscanf(commandString,"%*s %ms",&charParam);
        if( count != 1 )
        {
          // missing parameter
          return false;
        }
        stringParam = std::string( charParam);
        free(charParam);
      }
      return true;
    }
  }
  // not found
  return false;
}

bool GetCommandString( CommandId commandId, CommandString& commandString )
{
  for( unsigned int i = 0; i < CommandLookupLength; ++i)
  {
    if( CommandLookup[ i ].cmdId == commandId )
    {
      strncpy( commandString,  CommandLookup[ i ].cmdString, strlen(CommandLookup[ i ].cmdString) );
      return true;
    }
  }
  strncpy( commandString, CommandLookup[ UNKNOWN_COMMAND ].cmdString, MAX_COMMAND_STRING_LENGTH);
  return false;
}

const char* const GetHelpMessage()
{
  return helpMsg;
}


} // namespace PerformanceProtocol

} // namespace Dali
