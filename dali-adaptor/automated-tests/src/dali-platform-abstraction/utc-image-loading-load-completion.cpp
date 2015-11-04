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

#include "utc-image-loading-common.h"

void utc_image_loading_load_completion_startup(void)
{
  utc_dali_loading_startup();
}

void utc_image_loading_load_completion_cleanup(void)
{
  utc_dali_loading_cleanup();
}

// Positive test case for loading. Load lots and be sure it has succeeded.
int UtcDaliLoadCompletion(void)
{
  tet_printf("Running load completion test \n");

  DALI_ASSERT_ALWAYS( gAbstraction != 0 );

  // Start a bunch of loads that should work:

  Dali::Integration::BitmapResourceType bitmapResourceType;
  Dali::Integration::LoadResourcePriority priority = Dali::Integration::LoadPriorityNormal;
  unsigned loadsLaunched = 0;

  for( unsigned loadGroup = 0; loadGroup < NUM_LOAD_GROUPS_TO_ISSUE; ++loadGroup )
  {
    for( unsigned validImage = 0; validImage < NUM_VALID_IMAGES; ++validImage )
    {
      Dali::Integration::ResourceRequest request( loadGroup * NUM_VALID_IMAGES + validImage + 1, bitmapResourceType, VALID_IMAGES[validImage], priority );
      gAbstraction->LoadResource( request );
    }
    loadsLaunched += NUM_VALID_IMAGES;
  }

  // Drain the completed loads:
  Dali::Internal::Platform::ResourceCollector resourceSink;
  gAbstraction->GetResources( resourceSink );
  usleep( 500 * 1000 );
  gAbstraction->GetResources( resourceSink );

  const double startDrainTime = GetTimeMilliseconds( *gAbstraction );
  while( resourceSink.mGrandTotalCompletions < loadsLaunched && GetTimeMilliseconds( *gAbstraction ) - startDrainTime < MAX_MILLIS_TO_WAIT_FOR_KNOWN_LOADS )
  {
    usleep( 100 * 40 );
    gAbstraction->GetResources( resourceSink );
  }

  // Check the loads completed as expected:

  tet_printf( "Issued Loads: %u, Completed Loads: %u, Successful Loads: %u, Failed Loads: %u \n", loadsLaunched, resourceSink.mGrandTotalCompletions, unsigned(resourceSink.mSuccessCounts.size()), unsigned(resourceSink.mFailureCounts.size()) );
  DALI_TEST_CHECK( loadsLaunched == resourceSink.mGrandTotalCompletions );
  DALI_TEST_CHECK( loadsLaunched == resourceSink.mSuccessCounts.size() );
  DALI_TEST_CHECK( 0 == resourceSink.mFailureCounts.size() );

  // Check that each success was reported exactly once:
  for( ResourceCounterMap::const_iterator it = resourceSink.mSuccessCounts.begin(), end = resourceSink.mSuccessCounts.end(); it != end; ++it )
  {
    DALI_TEST_CHECK( it->second == 1u );
  }

  END_TEST;
}
