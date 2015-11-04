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

#ifndef __DALI_TEST_SUITE_IMAGE_LOADING_COMMON_H__
#define __DALI_TEST_SUITE_IMAGE_LOADING_COMMON_H__
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <dali/dali.h>
#include <dali-test-suite-utils.h>
#include "tizen-platform-abstraction.h"
#include "resource-collector.h"

using namespace Dali;
using namespace Dali::Integration;
using namespace Dali::Internal::Platform;

namespace
{
/**
 * The number of loads issued in test cases is a multiple of this. The higher it
 * is, the more the tests stress the system but the longer they take to run.
 * A value of 1000 is enough to make load tests take tens of seconds each
 * on desktop. */
const unsigned NUM_LOAD_GROUPS_TO_ISSUE = 158;

/**
 * The number of loads to issue when they will be cancelled.
 * Cancelled loads are cheap so we do a lot.
 */
const unsigned NUM_CANCELLED_LOAD_GROUPS_TO_ISSUE = NUM_LOAD_GROUPS_TO_ISSUE * 10;

/** The number of times to ask for resource load status. */
const unsigned MAX_NUM_RESOURCE_TRIES = 10;

/** The maximum time to wait for loads to complete when the number of expected loads is known. */
const unsigned MAX_MILLIS_TO_WAIT_FOR_KNOWN_LOADS = 1000 * 60;

/** Images that should load without issue. */
const char* const VALID_IMAGES[] = {
  TEST_IMAGE_DIR "/frac.jpg",
  TEST_IMAGE_DIR "/frac.24.bmp",
  TEST_IMAGE_DIR "/frac.png",
  TEST_IMAGE_DIR "/interlaced.gif",
  TEST_IMAGE_DIR "/pattern.gif"
};
const unsigned NUM_VALID_IMAGES = sizeof(VALID_IMAGES) / sizeof(VALID_IMAGES[0]);

///@ToDo: Add valid ktx, ico, and wbmp image examples.

/** Returns elapsed milliseconds. */
double GetTimeMilliseconds( Integration::PlatformAbstraction& abstraction )
{
  unsigned int seconds;
  unsigned int microseconds;
  abstraction.GetTimeMicroseconds( seconds, microseconds );
  double milliseconds = seconds * 1000.0 + microseconds / 1000.0;
  return milliseconds;
}

} // anon namespace

/** Live platform abstraction recreated for each test case. */
extern Integration::PlatformAbstraction * gAbstraction;

/** A variety of parameters to reach different code paths in the image loading. */
typedef std::pair<ImageDimensions, std::pair<FittingMode::Type, std::pair<SamplingMode::Type, bool> > > ImageParameters;
extern std::vector<ImageParameters> gCancelAttributes;


void utc_dali_loading_startup(void);
void utc_dali_loading_cleanup(void);

#endif // __DALI_TEST_SUITE_IMAGE_LOADING_COMMON_H__
