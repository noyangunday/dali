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
#include "command-line-options.h"

// EXTERNAL INCLUDES
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <dali/public-api/common/dali-vector.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{
struct Argument
{
  const char * const opt;
  const char * const optDescription;

  void Print()
  {
    const std::ios_base::fmtflags flags = std::cout.flags();
    std::cout << std::left << "  --";
    std::cout.width( 18 );
    std::cout << opt;
    std::cout << optDescription;
    std::cout << std::endl;
    std::cout.flags( flags );
  }
};

Argument EXPECTED_ARGS[] =
{
  { "no-vsync",    "Disable VSync on Render" },
  { "width",       "Stage Width"             },
  { "height",      "Stage Height"            },
  { "dpi",         "Emulated DPI"            },
  { "view",        "Stereocopic 3D view mode ([0]=MONO, 1=STEREO_HORZ, 2=STEREO_VERT, 3=STEREO_INTERLACED)" },
  { "stereo-base", "Distance in millimeters between left/right cameras [65.0]" },
  { "help",        "Help"                    },
  { NULL,          NULL                      }
};

enum Option
{
  OPTION_NO_VSYNC = 0,
  OPTION_STAGE_WIDTH,
  OPTION_STAGE_HEIGHT,
  OPTION_DPI,
  OPTION_STEREO_MODE,
  OPTION_STEREO_BASE,
  OPTION_HELP
};

typedef Dali::Vector< int > UnhandledContainer;

void ShowHelp()
{
  std::cout << "Available options:" << std::endl;
  Argument* arg = EXPECTED_ARGS;
  while ( arg->opt )
  {
    arg->Print();
    ++arg;
  }
}

} // unnamed namespace

CommandLineOptions::CommandLineOptions(int *argc, char **argv[])
: noVSyncOnRender(0),
  stageWidth(0), stageHeight(0),
  viewMode(0),
  stereoBase(65)
{
  // Exit gracefully if no arguments provided
  if ( !argc || !argv )
  {
    return;
  }

  if ( *argc > 1 )
  {
    // We do not want to print out errors.
    int origOptErrValue( opterr );
    opterr = 0;

    int help( 0 );

    const struct option options[]=
    {
      { EXPECTED_ARGS[OPTION_NO_VSYNC].opt,     no_argument,       &noVSyncOnRender, 1   },  // "--no-vsync"
      { EXPECTED_ARGS[OPTION_STAGE_WIDTH].opt,  required_argument, NULL,             'w' },  // "--width"
      { EXPECTED_ARGS[OPTION_STAGE_HEIGHT].opt, required_argument, NULL,             'h' },  // "--height"
      { EXPECTED_ARGS[OPTION_DPI].opt,          required_argument, NULL,             'd' },  // "--dpi"
      { EXPECTED_ARGS[OPTION_STEREO_MODE].opt,  required_argument, NULL,             'v' },  // "--view"
      { EXPECTED_ARGS[OPTION_STEREO_BASE].opt,  required_argument, NULL,             's' },  // "--stereo-base"
      { EXPECTED_ARGS[OPTION_HELP].opt,         no_argument,       &help,            '?' },  // "--help"
      { 0, 0, 0, 0 } // end of options
    };

    int shortOption( 0 );
    int optionIndex( 0 );

    const char* optString = "-w:h:d:v:s:"; // The '-' ensures that argv is NOT permuted
    bool optionProcessed( false );

    UnhandledContainer unhandledOptions; // We store indices of options we do not handle here

    do
    {
      shortOption = getopt_long( *argc, *argv, optString, options, &optionIndex );

      switch ( shortOption )
      {
        case 0:
        {
          // Check if we want help
          if ( help )
          {
            ShowHelp();
            optionProcessed = true;
          }
          break;
        }

        case 'w':
        {
          if ( optarg )
          {
            stageWidth = atoi( optarg );
            optionProcessed = true;
          }
          break;
        }

        case 'h':
        {
          if ( optarg )
          {
            stageHeight = atoi( optarg );
            optionProcessed = true;
          }
          break;
        }

        case 'd':
        {
          if ( optarg )
          {
            stageDPI.assign( optarg );
            optionProcessed = true;
          }
          break;
        }

        case 'v':
        {
          if ( optarg )
          {
            viewMode = atoi(optarg);
            optionProcessed = true;
          }
          break;
        }

        case 's':
        {
          if ( optarg )
          {
            stereoBase = atoi(optarg);
            optionProcessed = true;
          }
          break;
        }

        case -1:
        {
          // All command-line options have been parsed.
          break;
        }

        default:
        {
          unhandledOptions.PushBack( optind - 1 );
          break;
        }
      }
    } while ( shortOption != -1 );

    // Take out the options we have processed
    if ( optionProcessed )
    {
      if ( unhandledOptions.Count() > 0 )
      {
        int index( 1 );

        // Overwrite the argv with the values from the unhandled indices
        const UnhandledContainer::ConstIterator endIter = unhandledOptions.End();
        for ( UnhandledContainer::Iterator iter = unhandledOptions.Begin(); iter != endIter; ++iter )
        {
          (*argv)[ index++ ] = (*argv)[ *iter ];
        }
        *argc = unhandledOptions.Count() + 1; // +1 for the program name
      }
      else
      {
        // There are no unhandled options, so we should just have the program name
        *argc = 1;
      }

      optind = 1; // Reset to start
    }

    opterr = origOptErrValue; // Reset opterr value.
  }
}

CommandLineOptions::~CommandLineOptions()
{
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
