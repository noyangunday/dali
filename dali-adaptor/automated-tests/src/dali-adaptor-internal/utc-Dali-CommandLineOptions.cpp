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

#include <iostream>

#include <stdlib.h>
#include <getopt.h>
#include <dali/dali.h>
#include <command-line-options.h>
#include <dali-test-suite-utils.h>

using namespace Dali;
using namespace Dali::Internal::Adaptor;


// Called only once before first test is run.
void command_line_options_startup(void)
{
  test_return_value = TET_UNDEF;
  optind = 0; // Reset opt for test
}

// Called only once after last test is run
void command_line_options_cleanup(void)
{
  test_return_value = TET_PASS;
}

int UtcDaliCommandLineOptionsNoArgs(void)
{
  optind=0;

  int argc( 1 );
  const char* argList[1] = { "program" };
  char** argv = const_cast<char**>(argList);

  CommandLineOptions options( &argc, &argv );

  DALI_TEST_EQUALS( argc, 1, TEST_LOCATION );

  // Check values
  DALI_TEST_EQUALS( options.noVSyncOnRender, 0, TEST_LOCATION );
  DALI_TEST_EQUALS( options.stageWidth, 0, TEST_LOCATION );
  DALI_TEST_EQUALS( options.stageHeight, 0, TEST_LOCATION );
  DALI_TEST_EQUALS( options.stageDPI, "", TEST_LOCATION );

  END_TEST;
}

int UtcDaliCommandLineOptionsDaliShortArgs(void)
{
  optind=0;

  const char* argList[] =
  {
      "program",
      "-w", "800",
      "-h", "1000",
      "-d", "4x5",
  };
  int argc( sizeof( argList ) / sizeof( argList[0] ) );
  char** argv = const_cast<char**>(argList);

  CommandLineOptions options( &argc, &argv );

  // Should strip out the height and width
  DALI_TEST_EQUALS( argc, 1, TEST_LOCATION );

  // Check values
  DALI_TEST_EQUALS( options.noVSyncOnRender, 0, TEST_LOCATION );
  DALI_TEST_EQUALS( options.stageWidth, 800, TEST_LOCATION );
  DALI_TEST_EQUALS( options.stageHeight, 1000, TEST_LOCATION );
  DALI_TEST_EQUALS( options.stageDPI, "4x5", TEST_LOCATION );

  END_TEST;
}

int UtcDaliCommandLineOptionsDaliLongArgsEqualsSign(void)
{
  optind=0;

  const char* argList[] =
  {
      "program",
      "--width=800",
      "--height=1000",
      "--dpi=3x4",
      "--no-vsync",
      "--help"
  };
  int argc( sizeof( argList ) / sizeof( argList[0] ) );
  char** argv = const_cast<char**>(argList);

  CommandLineOptions options( &argc, &argv );

  // Should strip out the height and width
  DALI_TEST_EQUALS( argc, 1, TEST_LOCATION );

  // Check values
  DALI_TEST_EQUALS( options.noVSyncOnRender, 1, TEST_LOCATION );
  DALI_TEST_EQUALS( options.stageWidth, 800, TEST_LOCATION );
  DALI_TEST_EQUALS( options.stageHeight, 1000, TEST_LOCATION );
  DALI_TEST_EQUALS( options.stageDPI, "3x4", TEST_LOCATION );

  END_TEST;
}

int UtcDaliCommandLineOptionsDaliLongArgsSpaces(void)
{
  optind=0;

  const char* argList[] =
  {
      "program",
      "--width", "800",
      "--height", "1000",
      "--dpi", "3x4",
      "--no-vsync",
      "--help"
  };
  int argc( sizeof( argList ) / sizeof( argList[0] ) );
  char** argv = const_cast<char**>(argList);

  CommandLineOptions options( &argc, &argv );

  // Should strip out the height and width
  DALI_TEST_EQUALS( argc, 1, TEST_LOCATION );

  // Check values
  DALI_TEST_EQUALS( options.noVSyncOnRender, 1, TEST_LOCATION );
  DALI_TEST_EQUALS( options.stageWidth, 800, TEST_LOCATION );
  DALI_TEST_EQUALS( options.stageHeight, 1000, TEST_LOCATION );
  DALI_TEST_EQUALS( options.stageDPI, "3x4", TEST_LOCATION );

  END_TEST;
}

int UtcDaliCommandLineOptionsNonDaliArgs(void)
{
  optind=0;

  const char* argList[] =
  {
      "program",
      "hello-world",
      "-y", "600",
  };
  int argc( sizeof( argList ) / sizeof( argList[0] ) );
  char** argv = const_cast<char**>(argList);

  CommandLineOptions options( &argc, &argv );

  // Should still be the same
  DALI_TEST_EQUALS( argc, 4, TEST_LOCATION );

  // Ensure order has not changed
  DALI_TEST_EQUALS( argList[0], "program", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[1], "hello-world", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[2], "-y", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[3], "600", TEST_LOCATION );

  END_TEST;
}

int UtcDaliCommandLineOptionsMixture(void)
{
  optind = 0; // Reset opt for test

  const char* argList[] =
  {
    "program",
    "--width=800",
    "hello-world",
    "-y", "600",
    "--height", "1000",
    "-r",
  };
  int argc( sizeof( argList ) / sizeof( argList[0] ) );
  char** argv = const_cast<char**>( argList );

  CommandLineOptions options( &argc, &argv );

  // Should still be the same
  DALI_TEST_EQUALS( argc, 5, TEST_LOCATION );

  // Ensure order of program name and unhandled options has not changed
  DALI_TEST_EQUALS( argList[0], "program", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[1], "hello-world", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[2], "-y", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[3], "600", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[4], "-r", TEST_LOCATION );

  END_TEST;
}

int UtcDaliCommandLineOptionsMixtureDaliOpsAtStart(void)
{
  optind=0;

  const char* argList[] =
  {
      "program",
      "--width=800",
      "--height", "1000",
      "-r",
      "hello-world",
      "-y", "600",
  };
  int argc( sizeof( argList ) / sizeof( argList[0] ) );
  char** argv = const_cast<char**>( argList );

  CommandLineOptions options( &argc, &argv );

  // Should still be the same
  DALI_TEST_EQUALS( argc, 5, TEST_LOCATION );

  // Ensure order of program name and unhandled options has not changed
  DALI_TEST_EQUALS( argList[0], "program", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[1], "-r", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[2], "hello-world", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[3], "-y", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[4], "600", TEST_LOCATION );

  END_TEST;
}

int UtcDaliCommandLineOptionsMixtureDaliOpsAtEnd(void)
{
  optind=0;

  const char* argList[] =
  {
      "program",
      "hello-world",
      "-y", "600",
      "-r",
      "--width=800",
      "--height", "1000",
  };
  int argc( sizeof( argList ) / sizeof( argList[0] ) );
  char** argv = const_cast<char**>( argList );

  CommandLineOptions options( &argc, &argv );

  // Should still be the same
  DALI_TEST_EQUALS( argc, 5, TEST_LOCATION );

  // Ensure order of program name and unhandled options has not changed
  DALI_TEST_EQUALS( argList[0], "program", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[1], "hello-world", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[2], "-y", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[3], "600", TEST_LOCATION );
  DALI_TEST_EQUALS( argList[4], "-r", TEST_LOCATION );

  END_TEST;
}
